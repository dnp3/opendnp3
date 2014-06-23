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
	IUTCTimeSource* pTimeSource,
	const MasterParams& params_
	) :

	logger(root.GetLogger()),
	pExecutor(&executor),
	pLower(&lower),
	params(params_),
	pSOEHandler(pSOEHandler_),
	isOnline(false),
	isSending(false),
	solSeq(0),
	unsolSeq(0),
	pActiveTask(nullptr),
	pResponseTimer(nullptr),
	scheduler(&logger, pSOEHandler_, pTimeSource, executor)		
{
	auto callback = [this](){ PostCheckForTask(); };
	scheduler.SetExpirationHandler(Runnable::Bind(callback));
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

		scheduler.OnLowerLayerDown();
		this->CancelResponseTimer();
		isOnline = false;
		isSending = false;
		solSeq = 0;
		unsolSeq = 0;
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
	if (isOnline && !pActiveTask && !isSending)
	{
		auto pTask = scheduler.Start();
		if (pTask)
		{
			FORMAT_LOG_BLOCK(logger, flags::INFO, "Begining task: %s", pTask->Name());
			pActiveTask = pTask;
			this->StartTask(pTask);
		}
	}
}

void MasterContext::StartTask(IMasterTask* pTask)
{	
	APDURequest request(txBuffer.GetWriteBuffer());	
	pTask->BuildRequest(request, params, solSeq);
	this->StartResponseTimer();
	this->Transmit(request.ToReadOnly());
}

void MasterContext::QueueCommandAction(const Function1<ICommandProcessor*>& action)
{
	if (isOnline)
	{
		scheduler.ScheduleCommand(action);
	}
	else
	{
		ConstantCommandProcessor processor(CommandResponse(CommandResult::NO_COMMS), nullptr);
		action.Apply(&processor);
	}
}

void MasterContext::OnResponseTimeout()
{
	if (pResponseTimer)
	{
		pResponseTimer = nullptr;
		if (pActiveTask)
		{
			pActiveTask->OnResponseTimeout(params, scheduler);
			pActiveTask = nullptr;
			solSeq = AppControlField::NextSeq(solSeq);
			this->PostCheckForTask();
		}
	}
}

void MasterContext::OnSendResult(bool isSucccess)
{
	if (isOnline && isSending)
	{
		isSending = false;

		if (!(this->CheckConfirmTransmit() || pActiveTask))
		{
			this->PostCheckForTask();
		}				
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

			switch (header.function)
			{
				case(FunctionCode::RESPONSE) :
					this->OnResponse(header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					break;
				case(FunctionCode::UNSOLICITED_RESPONSE) :
					this->OnUnsolicitedResponse(header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					break;
				default:
					FORMAT_LOG_BLOCK(logger, flags::WARN, "unsupported function code: %s", FunctionCodeToString(header.function));
					break;
			}

			// process the IIN bits after we've handeled the frame
			this->OnReceiveIIN(header.IIN);
		}
	}
}


void MasterContext::OnResponse(const APDUResponseHeader& header, const ReadOnlyBuffer& objects)
{	
	if (header.control.UNS)
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring response with UNS bit");
	}
	else
	{
		if (pActiveTask && (header.control.SEQ == this->solSeq))
		{
			this->CancelResponseTimer();
			
			this->solSeq = AppControlField::NextSeq(solSeq);

			auto result = pActiveTask->OnResponse(header, objects, params, scheduler);

			if (header.control.CON && CanConfirmResponse(result))
			{
				this->QueueConfirm(APDUHeader::SolicitedConfirm(header.control.SEQ));
			}

			switch (result)
			{
				case(TaskStatus::CONTINUE) :					
					this->StartResponseTimer();
					break;
				case(TaskStatus::REPEAT) :					
					this->StartTask(pActiveTask);
					break;
				default:
					pActiveTask = nullptr;
					this->PostCheckForTask();
					break;
			}			
		}
		else
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN, "Unexpected response with sequence: %u", header.control.SEQ);
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
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unsolicited response with UNS bit not set");
	}
}

void MasterContext::OnReceiveIIN(const IINField& iin)
{
	if (iin.IsSet(IINBit::DEVICE_RESTART))
	{
		scheduler.OnRestartDetected(params);
	}

	if (iin.IsSet(IINBit::NEED_TIME))
	{
		scheduler.OnNeedTimeDetected(params);
	}
}

void MasterContext::StartResponseTimer()
{
	if (pResponseTimer == nullptr)
	{
		auto timeout = [this](){ this->OnResponseTimeout(); };
		pResponseTimer = pExecutor->Start(params.responseTimeout, Runnable::Bind(timeout));
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

void MasterContext::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback)
{	
	this->SelectAndOperateT(command, index, pCallback);
}

void MasterContext::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback)
{
	this->DirectOperateT(command, index, pCallback);
}

void MasterContext::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback)
{
	this->SelectAndOperateT(command, index, pCallback);
}

void MasterContext::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback)
{
	this->DirectOperateT(command, index, pCallback);
}

void MasterContext::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->SelectAndOperateT(command, index, pCallback);
}

void MasterContext::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->DirectOperateT(command, index, pCallback);
}

void MasterContext::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->SelectAndOperateT(command, index, pCallback);
}

void MasterContext::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->DirectOperateT(command, index, pCallback);
}

void MasterContext::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback)
{
	this->SelectAndOperateT(command, index, pCallback);
}

void MasterContext::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback)
{
	this->DirectOperateT(command, index, pCallback);
}

}
