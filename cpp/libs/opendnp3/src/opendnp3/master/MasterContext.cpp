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

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/APDUWrapper.h"
#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDULogging.h"
#include "opendnp3/master/MeasurementHandler.h"
#include "opendnp3/master/ConstantCommandProcessor.h"

#include "opendnp3/objects/Group12.h"
#include "opendnp3/objects/Group41.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

MasterContext::MasterContext(
	IExecutor& executor,
	LogRoot& root,
	ILowerLayer& lower,
	ISOEHandler& SOEHandler,	
	IMasterApplication& application,
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
	isOnline(false),
	isSending(false),
	solSeq(0),
	unsolSeq(0),	
	pState(&MasterStateIdle::Instance()),
	pResponseTimer(nullptr),
	tasks(params, logger, application, SOEHandler, application),
	scheduler(executor, *this),
	txBuffer(params.maxTxFragSize)
{
	
}

bool MasterContext::OnLayerUp()
{
	if (isOnline)
	{		
		return false;
	}
	else
	{
		isOnline = true;
		pTaskLock->OnLayerUp();
		tasks.Initialize(scheduler);
		scheduler.OnLowerLayerUp();
		return true;
	}
}

bool MasterContext::OnLayerDown()
{
	if (isOnline)
	{
		auto now = pExecutor->GetTime();

		if (pActiveTask.IsDefined())
		{			
			pActiveTask->OnLowerLayerClose(now);
			this->NotifyCurrentTask(TaskState::FAILURE);			
			pActiveTask.Release();
		}

		pState = &MasterStateIdle::Instance();

		pTaskLock->OnLayerDown();

		scheduler.OnLowerLayerDown();

		this->CancelResponseTimer();

		solSeq = unsolSeq = 0;
		isOnline = isSending = false;		 		
		return true;
	}
	else
	{
		return false;
	}
}

void MasterContext::PostCheckForTask()
{
	auto callback = [this]() { this->CheckForTask(); };
	pExecutor->PostLambda(callback);
}

void MasterContext::CheckForTask()
{
	if (isOnline)
	{
		pState = pState->OnStart(this);		
	}
}

void MasterContext::StartTask(IMasterTask& task)
{		
	APDURequest request(txBuffer.GetWriteBuffer());
	task.BuildRequest(request, solSeq);
	this->StartResponseTimer();
	this->Transmit(request.ToReadOnly());	
}

void MasterContext::ReleaseActiveTask()
{
	if (pActiveTask.IsDefined())
	{
		if (pActiveTask->IsRecurring())
		{
			scheduler.Schedule(std::move(pActiveTask));
		}
		else
		{
			pActiveTask.Release();
		}
	}
}

void MasterContext::NotifyCurrentTask(TaskState state)
{
	if (pActiveTask.IsDefined() && pActiveTask->Id().IsDefined())
	{
		pApplication->OnTaskStateChange(pActiveTask->Id(), state);
	}
}

void MasterContext::ScheduleRecurringPollTask(IMasterTask* pTask)
{
	tasks.BindTask(pTask);

	if (isOnline)
	{
		scheduler.Schedule(ManagedPtr<IMasterTask>::WrapperOnly(pTask));
		this->PostCheckForTask();
	}	
}

void MasterContext::ScheduleAdhocTask(IMasterTask* pTask)
{
	auto task = ManagedPtr<IMasterTask>::Deleted(pTask);
	if (isOnline)
	{
		scheduler.Schedule(std::move(task));
		this->PostCheckForTask();
	}
	else
	{
		// can't run this task since we're offline so fail it immediately
		pApplication->OnTaskStateChange(task->Id(), TaskState::FAILURE);
	}
}

void MasterContext::OnPendingTask()
{
	this->PostCheckForTask();
}

void MasterContext::OnResponseTimeout()
{
	pState = pState->OnResponseTimeout(this);
}

void MasterContext::OnSendResult(bool isSucccess)
{
	if (isOnline && isSending)
	{
		isSending = false;

		this->CheckConfirmTransmit();
		this->CheckForTask();						
	}
}

void MasterContext::OnReceive(const ReadOnlyBuffer& apdu)
{
	if (isOnline)
	{
		APDUResponseHeader header;		
		if (APDUHeaderParser::ParseResponse(apdu, header, &logger))
		{
			FORMAT_LOG_BLOCK(logger, flags::APP_HEADER_RX,
				"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
				header.control.FIR,
				header.control.FIN,
				header.control.CON,
				header.control.UNS,
				header.control.SEQ,
				FunctionCodeToString(header.function),
				header.IIN.LSB,
				header.IIN.MSB);

			if (header.control.UNS)
			{
				if (header.function == FunctionCode::UNSOLICITED_RESPONSE)
				{
					this->OnUnsolicitedResponse(header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(logger, flags::WARN, "Ignoring unsupported function with UNS bit set: %s", FunctionCodeToString(header.function));
				}
			}
			else
			{
				if (header.function == FunctionCode::RESPONSE)
				{
					this->pState = pState->OnResponse(this, header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(logger, flags::WARN, "Ignoring unsupported solicited function code: %s", FunctionCodeToString(header.function));
				}
			}						
		}
	}
}


void MasterContext::OnUnsolicitedResponse(const APDUResponseHeader& header, const ReadOnlyBuffer& objects)
{
	if (header.control.UNS)
	{		
		auto success = MeasurementHandler::ProcessMeasurements(objects, &logger, pSOEHandler);

		if (success && header.control.CON)
		{
			this->QueueConfirm(APDUHeader::UnsolicitedConfirm(header.control.SEQ));
		}
	}	
}

void MasterContext::OnReceiveIIN(const IINField& iin)
{
	pApplication->OnReceiveIIN(iin);
	
	if (iin.IsSet(IINBit::DEVICE_RESTART))
	{
		tasks.clearRestart.Demand();
		tasks.assignClass.Demand();
		tasks.startupIntegrity.Demand();
		tasks.enableUnsol.Demand();		
	}

	if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && params.integrityOnEventOverflowIIN)
	{		
		tasks.startupIntegrity.Demand();		
	}

	if (iin.IsSet(IINBit::NEED_TIME))
	{
		tasks.timeSync.Demand();
	}	

	if (params.eventScanOnEventsAvailableIIN && (iin.IsSet(IINBit::CLASS1_EVENTS) || iin.IsSet(IINBit::CLASS2_EVENTS) || iin.IsSet(IINBit::CLASS3_EVENTS)))
	{
		tasks.eventScan.Demand();
	}
}

void MasterContext::StartResponseTimer()
{
	if (pResponseTimer == nullptr)
	{
		auto timeout = [this](){ this->OnResponseTimeout(); };
		pResponseTimer = pExecutor->Start(params.responseTimeout, Action0::Bind(timeout));
	}	
}

bool MasterContext::CancelResponseTimer()
{
	if (pResponseTimer)
	{
		pResponseTimer->Cancel();
		pResponseTimer = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}	

void MasterContext::QueueConfirm(const APDUHeader& header)
{
	this->confirmQueue.push_back(header);
	this->CheckConfirmTransmit();
}

bool MasterContext::CheckConfirmTransmit()
{
	if (!isSending && !confirmQueue.empty())
	{
		auto pConfirm = confirmQueue.front();
		APDUWrapper wrapper(txBuffer.GetWriteBuffer());
		wrapper.SetFunction(pConfirm.function);
		wrapper.SetControl(pConfirm.control);
		this->Transmit(wrapper.ToReadOnly());
		confirmQueue.pop_front();
		return true;
	}
	else
	{
		return false;
	}
}

void MasterContext::Transmit(const ReadOnlyBuffer& output)
{
	logging::ParseAndLogRequestTx(&logger, output);	
	assert(!isSending);
	isSending = true;
	pLower->BeginTransmit(output);	
}

void MasterContext::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{	
	this->SelectAndOperateT(command, index, callback, Group12Var1::Inst());
}

void MasterContext::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->DirectOperateT(command, index, callback, Group12Var1::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var2::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var2::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var1::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var1::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var3::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var3::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var4::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ITaskCallback<CommandResponse>& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var4::Inst());
}

}
