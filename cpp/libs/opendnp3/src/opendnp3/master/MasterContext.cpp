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

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

MasterContext::MasterContext(
	IExecutor& executor,
	LogRoot& root,
	ILowerLayer& lower,
	ISOEHandler* pSOEHandler_,
	IUTCTimeSource& timeSource,
	const MasterParams& params_,
	ITaskLock& taskLock
	) :

	logger(root.GetLogger()),
	pExecutor(&executor),
	pLower(&lower),
	params(params_),
	pSOEHandler(pSOEHandler_),
	pTaskLock(&taskLock),
	isOnline(false),
	isSending(false),
	solSeq(0),
	unsolSeq(0),
	pActiveTask(nullptr),
	pState(&MasterStateIdle::Instance()),
	pResponseTimer(nullptr),
	staticTasks(&logger, pSOEHandler, &timeSource),
	scheduler(&logger, staticTasks, executor, *this)
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
		scheduler.OnLowerLayerUp(params);		
		return true;
	}
}

bool MasterContext::OnLayerDown()
{
	if (isOnline)
	{
		if (pActiveTask)
		{
			pActiveTask->OnLowerLayerClose();
			pActiveTask = nullptr;
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

void MasterContext::StartTask(IMasterTask* pTask)
{		
	APDURequest request(txBuffer.GetWriteBuffer());	
	pTask->BuildRequest(request, params, solSeq);
	this->StartResponseTimer();
	this->Transmit(request.ToReadOnly());
}

void MasterContext::OnPendingTask()
{
	this->PostCheckForTask();
}

bool MasterContext::QueueUserTask(const openpal::Function0<IMasterTask*>& action)
{
	if (isOnline)
	{
		return scheduler.ScheduleUserTask(action);
	}
	else
	{
		return false;
	}
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
				header.control.FIN,
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
	else
	{
		
	}
}

void MasterContext::OnReceiveIIN(const IINField& iin)
{
	scheduler.ProcessRxIIN(iin, params);
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
	this->confirmQueue.Enqueue(header);
	this->CheckConfirmTransmit();
}

bool MasterContext::CheckConfirmTransmit()
{
	if (!isSending && confirmQueue.IsNotEmpty())
	{
		auto pConfirm = confirmQueue.Pop();
		APDUWrapper wrapper(txBuffer.GetWriteBuffer());
		wrapper.SetFunction(pConfirm->function);
		wrapper.SetControl(pConfirm->control);
		this->Transmit(wrapper.ToReadOnly());
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

bool MasterContext::CanConfirmResponse(TaskStatus status)
{
	switch (status)
	{
		case(TaskStatus::SUCCESS) :
		case(TaskStatus::CONTINUE) :
			return true;
		default:
			return false;
	}
}

void MasterContext::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
{	
	this->SelectAndOperateT(command, index, callback);
}

void MasterContext::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback);
}

void MasterContext::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback);
}

void MasterContext::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback);
}

void MasterContext::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback);
}

void MasterContext::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback);
}

void MasterContext::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback);
}

void MasterContext::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback);
}

void MasterContext::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
{
	this->SelectAndOperateT(command, index, callback);
}

void MasterContext::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
{
	this->DirectOperateT(command, index, callback);
}

}
