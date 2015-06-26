/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "MasterContext.h"

#include "opendnp3/app/APDULogging.h"
#include "opendnp3/LogLevels.h"
#include "opendnp3/master/MeasurementHandler.h"

#include <openpal/logging/LogMacros.h>


#include <assert.h>

namespace opendnp3
{
	MContext::MContext(
		openpal::IExecutor& executor,
		openpal::LogRoot& root,
		ILowerLayer& lower,
		ISOEHandler& SOEHandler,
		opendnp3::IMasterApplication& application,
		IScheduleCallback& scheduleCallback,
		const MasterParams& params_,
		ITaskLock& taskLock
		) :

		logger(root.GetLogger()),
		pExecutor(&executor),
		pLower(&lower),
		params(params_),
		pSOEHandler(&SOEHandler),
		pTaskLock(&taskLock),
		pApplication(&application),
		pScheduleCallback(&scheduleCallback),
		isOnline(false),
		isSending(false),		
		responseTimer(executor),
		tasks(params, logger, application, SOEHandler, application),
		scheduler(executor, scheduleCallback),
		txBuffer(params.maxTxFragSize),
		pState(&MasterStateIdle::Instance())
	{}

	void MContext::CheckForTask()
	{
		if (isOnline)
		{
			this->pState = pState->OnStart(*this);
		}
	}

	void MContext::OnResponseTimeout()
	{
		if (isOnline)
		{
			this->pState = pState->OnResponseTimeout(*this);
		}
	}

	void MContext::CompleteActiveTask()
	{
		if (this->pActiveTask.IsDefined())
		{
			if (this->pActiveTask->IsRecurring())
			{
				this->scheduler.Schedule(std::move(this->pActiveTask));
			}
			else
			{
				this->pActiveTask.Release();
			}

			pTaskLock->Release(*pScheduleCallback);
			this->PostCheckForTask();
		}		
	}

	void MContext::OnReceive(const openpal::ReadBufferView& apdu, const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
	{
		auth.OnReceive(*this, apdu, header, objects);
	}

	void MContext::ProcessAPDU(const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
	{
		switch (header.function)
		{
		case(FunctionCode::UNSOLICITED_RESPONSE) :
			this->ProcessUnsolicitedResponse(header, objects);
			break;
		case(FunctionCode::RESPONSE) :
			this->ProcessResponse(header, objects);
			break;
		default:
			FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring unsupported function code: %s", FunctionCodeToString(header.function));
			break;
		}
	}

	void MContext::ProcessIIN(const IINField& iin)
	{
		if (iin.IsSet(IINBit::DEVICE_RESTART))
		{
			this->tasks.clearRestart.Demand();
			this->tasks.assignClass.Demand();
			this->tasks.startupIntegrity.Demand();
			this->tasks.enableUnsol.Demand();
		}

		if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && this->params.integrityOnEventOverflowIIN)
		{
			this->tasks.startupIntegrity.Demand();
		}

		if (iin.IsSet(IINBit::NEED_TIME))
		{
			this->tasks.timeSync.Demand();
		}

		if ((iin.IsSet(IINBit::CLASS1_EVENTS) && this->params.eventScanOnEventsAvailableClassMask.HasClass1()) ||
			(iin.IsSet(IINBit::CLASS2_EVENTS) && this->params.eventScanOnEventsAvailableClassMask.HasClass2()) ||
			(iin.IsSet(IINBit::CLASS3_EVENTS) && this->params.eventScanOnEventsAvailableClassMask.HasClass3()))
		{
			this->tasks.eventScan.Demand();
		}

		this->pApplication->OnReceiveIIN(iin);
	}

	void MContext::ProcessUnsolicitedResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
	{
		if (!header.control.UNS)
		{
			SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unsolicited response without UNS bit set");
			return;
		}

		auto result = MeasurementHandler::ProcessMeasurements(objects, logger, pSOEHandler);

		if ((result == ParseResult::OK) && header.control.CON)
		{
			this->QueueConfirm(APDUHeader::UnsolicitedConfirm(header.control.SEQ));
		}

		this->ProcessIIN(header.IIN);
	}

	void MContext::ProcessResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
	{
		this->pState = pState->OnResponse(*this, header, objects);
		this->ProcessIIN(header.IIN);
	}

	void MContext::QueueConfirm(const APDUHeader& header)
	{
		this->confirmQueue.push_back(header);
		this->CheckConfirmTransmit();
	}

	bool MContext::CheckConfirmTransmit()
	{
		if (this->isSending || this->confirmQueue.empty())
		{
			return false;
		}

		auto confirm = this->confirmQueue.front();
		APDUWrapper wrapper(this->txBuffer.GetWriteBufferView());
		wrapper.SetFunction(confirm.function);
		wrapper.SetControl(confirm.control);
		this->Transmit(wrapper.ToReadOnly());	
		this->confirmQueue.pop_front();
		return true;
	}

	void MContext::Transmit(const openpal::ReadBufferView& data)
	{
		logging::ParseAndLogRequestTx(this->logger, data);
		assert(!this->isSending);
		this->isSending = true;
		this->pLower->BeginTransmit(data);
	}

	void MContext::StartResponseTimer()
	{
		auto timeout = [this](){ this->OnResponseTimeout(); };
		this->responseTimer.Start(this->params.responseTimeout, timeout);
	}

	void MContext::PostCheckForTask()
	{
		auto callback = [this]() { this->CheckForTask(); };
		this->pExecutor->PostLambda(callback);
	}

	bool MContext::GoOffline()
	{
		if (isOnline)
		{
			auto now = pExecutor->GetTime();

			if (pActiveTask.IsDefined())
			{
				pActiveTask->OnLowerLayerClose(now);
				pActiveTask.Release();
			}

			pState = &MasterStateIdle::Instance();

			pTaskLock->OnLayerDown();

			auth.GoOffline();

			scheduler.OnLowerLayerDown();

			responseTimer.Cancel();

			solSeq = unsolSeq = 0;
			isOnline = isSending = false;

			return true;			
		}
		else
		{
			return false;
		}		
	}

	bool MContext::GoOnline()
	{
		if (isOnline)
		{
			return false;
		}
		else
		{
			isOnline = true;
			pTaskLock->OnLayerUp();
			auth.GoOnline();
			tasks.Initialize(scheduler);
			scheduler.OnLowerLayerUp();
			return true;
		}
	}

	bool MContext::BeginNewTask(openpal::ManagedPtr<IMasterTask>& task)
	{
		this->pActiveTask = std::move(task);				
		this->pActiveTask->OnStart();
		FORMAT_LOG_BLOCK(logger, flags::INFO, "Begining task: %s", this->pActiveTask->Name());
		return this->ResumeActiveTask();
	}

	bool MContext::ResumeActiveTask()
	{		
		if (this->pTaskLock->Acquire(*pScheduleCallback))
		{
			APDURequest request(this->txBuffer.GetWriteBufferView());
			this->pActiveTask->BuildRequest(request, this->solSeq);
			this->StartResponseTimer();
			auto apdu = request.ToReadOnly();
			auth.RecordLastRequest(apdu);
			this->Transmit(apdu);
			return true;
		}
		else
		{
			return false;
		}
	}
}


