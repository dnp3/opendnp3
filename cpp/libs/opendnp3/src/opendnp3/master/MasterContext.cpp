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
#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/app/APDULogging.h"
#include "opendnp3/master/MeasurementHandler.h"

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
	const MasterParams& params,
	ITaskLock& taskLock
	) :

	mstate(executor, root, lower, SOEHandler, application, *this, params, taskLock)
{
	
}

bool MasterContext::OnLayerUp()
{
	if (mstate.isOnline)
	{		
		return false;
	}
	else
	{
		mstate.isOnline = true;
		mstate.pTaskLock->OnLayerUp();
		mstate.tasks.Initialize(mstate.scheduler);
		mstate.scheduler.OnLowerLayerUp();
		return true;
	}
}

bool MasterContext::OnLayerDown()
{
	return mstate.GoOffline();
}

void MasterContext::PostCheckForTask()
{
	auto callback = [this]() { this->CheckForTask(); };
	mstate.pExecutor->PostLambda(callback);
}

void MasterContext::CheckForTask()
{
	if (mstate.isOnline)
	{
		mstate.pState = mstate.pState->OnStart(this);
	}
}

void MasterContext::StartTask(IMasterTask& task)
{				
	APDURequest request(mstate.txBuffer.GetWriteBufferView());
	task.BuildRequest(request, mstate.solSeq);
	this->StartResponseTimer();
	this->Transmit(request.ToReadOnly());	
}

void MasterContext::ReleaseActiveTask()
{
	if (mstate.pActiveTask.IsDefined())
	{
		if (mstate.pActiveTask->IsRecurring())
		{
			mstate.scheduler.Schedule(std::move(mstate.pActiveTask));
		}
		else
		{
			mstate.pActiveTask.Release();
		}
	}
}

void MasterContext::ScheduleRecurringPollTask(IMasterTask* pTask)
{
	mstate.tasks.BindTask(pTask);

	if (mstate.isOnline)
	{
		mstate.scheduler.Schedule(ManagedPtr<IMasterTask>::WrapperOnly(pTask));
		this->PostCheckForTask();
	}	
}

void MasterContext::ScheduleAdhocTask(IMasterTask* pTask)
{
	auto task = ManagedPtr<IMasterTask>::Deleted(pTask);
	if (mstate.isOnline)
	{
		mstate.scheduler.Schedule(std::move(task));
		this->PostCheckForTask();
	}
	else
	{
		// can't run this task since we're offline so fail it immediately
		pTask->OnLowerLayerClose(mstate.pExecutor->GetTime());
	}
}

void MasterContext::OnPendingTask()
{
	this->PostCheckForTask();
}

void MasterContext::OnResponseTimeout()
{
	mstate.pState = mstate.pState->OnResponseTimeout(this);
}

void MasterContext::OnSendResult(bool isSucccess)
{
	if (mstate.isOnline && mstate.isSending)
	{
		mstate.isSending = false;

		this->CheckConfirmTransmit();
		this->CheckForTask();						
	}
}

void MasterContext::OnReceive(const ReadBufferView& apdu)
{
	if (mstate.isOnline)
	{
		APDUResponseHeader header;		
		if (APDUHeaderParser::ParseResponse(apdu, header, &mstate.logger))
		{
			FORMAT_LOG_BLOCK(mstate.logger, flags::APP_HEADER_RX,
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
					FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Ignoring unsupported function with UNS bit set: %s", FunctionCodeToString(header.function));
				}
			}
			else
			{
				if (header.function == FunctionCode::RESPONSE)
				{
					this->mstate.pState = mstate.pState->OnResponse(this, header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Ignoring unsupported solicited function code: %s", FunctionCodeToString(header.function));
				}
			}						
		}
	}
}


void MasterContext::OnUnsolicitedResponse(const APDUResponseHeader& header, const ReadBufferView& objects)
{
	if (header.control.UNS)
	{		
		auto result = MeasurementHandler::ProcessMeasurements(objects, mstate.logger, mstate.pSOEHandler);

		if ((result == ParseResult::OK) && header.control.CON)
		{
			this->QueueConfirm(APDUHeader::UnsolicitedConfirm(header.control.SEQ));
		}
	}	
}

void MasterContext::OnReceiveIIN(const IINField& iin)
{
	mstate.pApplication->OnReceiveIIN(iin);
	
	if (iin.IsSet(IINBit::DEVICE_RESTART))
	{
		mstate.tasks.clearRestart.Demand();
		mstate.tasks.assignClass.Demand();
		mstate.tasks.startupIntegrity.Demand();
		mstate.tasks.enableUnsol.Demand();
	}

	if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && mstate.params.integrityOnEventOverflowIIN)
	{		
		mstate.tasks.startupIntegrity.Demand();
	}

	if (iin.IsSet(IINBit::NEED_TIME))
	{
		mstate.tasks.timeSync.Demand();
	}	

	if ((iin.IsSet(IINBit::CLASS1_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass1()) ||
		(iin.IsSet(IINBit::CLASS2_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass2()) ||
		(iin.IsSet(IINBit::CLASS3_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass3()))
	{
		mstate.tasks.eventScan.Demand();
	}
}

void MasterContext::StartResponseTimer()
{	
	auto timeout = [this](){ this->OnResponseTimeout(); };
	mstate.responseTimer.Start(mstate.params.responseTimeout, timeout);	
}

void MasterContext::QueueConfirm(const APDUHeader& header)
{
	this->mstate.confirmQueue.push_back(header);
	this->CheckConfirmTransmit();
}

bool MasterContext::CheckConfirmTransmit()
{
	if (!mstate.isSending && !mstate.confirmQueue.empty())
	{
		auto pConfirm = mstate.confirmQueue.front();
		APDUWrapper wrapper(mstate.txBuffer.GetWriteBufferView());
		wrapper.SetFunction(pConfirm.function);
		wrapper.SetControl(pConfirm.control);
		this->Transmit(wrapper.ToReadOnly());
		mstate.confirmQueue.pop_front();
		return true;
	}
	else
	{
		return false;
	}
}

void MasterContext::Transmit(const ReadBufferView& output)
{
	logging::ParseAndLogRequestTx(mstate.logger, output);
	assert(!mstate.isSending);
	mstate.isSending = true;
	mstate.pLower->BeginTransmit(output);
}

void MasterContext::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
{	
	this->SelectAndOperateT(command, index, callback, Group12Var1::Inst());
}

void MasterContext::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback, Group12Var1::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var2::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var2::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var1::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var1::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var3::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var3::Inst());
}

void MasterContext::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback, Group41Var4::Inst());
}

void MasterContext::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback, Group41Var4::Inst());
}

}
