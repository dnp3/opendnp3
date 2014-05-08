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
#include "Outstation.h"

#include "opendnp3/app/AnalogOutput.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/APDUParser.h"

#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/LogLevels.h"

#include <openpal/LogMacros.h>
#include <openpal/IExecutor.h>

#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/IINHelpers.h"
#include "opendnp3/outstation/WriteHandler.h"
#include "opendnp3/outstation/ReadHandler.h"
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/ConstantCommandAction.h"
#include "opendnp3/outstation/CommandResponseHandler.h"

using namespace openpal;

/*
namespace opendnp3
{

Outstation::Outstation(openpal::LogRoot& root,
                        IAppLayer* pAppLayer,
                        IExecutor* pExecutor,
                        ITimeWriteHandler* pTimeWriteHandler,
                        Database* pDatabase,
                        const EventBufferFacade& buffers,
                        ICommandHandler* pCmdHandler,
                        const OutstationConfig& config) :
	IAppUser(root),
	selectBuffer(pExecutor, config.selectTimeout),
	lastResponse(responseBuffer.GetWriteBuffer()),
	pExecutor(pExecutor),
	mpAppLayer(pAppLayer),
	mpDatabase(pDatabase),
	mpCmdHandler(pCmdHandler),
	mpState(AS_Closed::Inst()),
	mConfig(config),
	mpUnsolTimer(nullptr),
	mpTimeWriteHandler(pTimeWriteHandler),
	eventBuffer(buffers),
	rspContext(pDatabase, &eventBuffer, StaticResponseTypes(config.staticDefaults), config.eventDefaults),
	mDeferredUnsol(false),
	mStartupNullUnsol(false),
	mpTimeTimer(nullptr)
{
	// Link the event buffer to the database
	mpDatabase->SetEventBuffer(eventBuffer);

	staticIIN.Set(IINBit::DEVICE_RESTART); // Always set on restart

	// Cause the Outstation to go through the null-unsol startup sequence
	if (!mConfig.disableUnsol)
	{
		mDeferredUnsol = true;
	}
}

Outstation::~Outstation()
{
	if (mpUnsolTimer)
	{
		mpUnsolTimer->Cancel();
	}
	if (mpTimeTimer)
	{
		mpTimeTimer->Cancel();
	}
}

void Outstation::SetNeedTimeIIN()
{
	staticIIN.Set(IINBit::NEED_TIME);
}

// Implement IAppUser - external callbacks from the app layer

void Outstation::OnLowerLayerUp()
{
	mpState->OnLowerLayerUp(this);
}

void Outstation::OnLowerLayerDown()
{
	mpState->OnLowerLayerDown(this);
}

void Outstation::OnSolSendSuccess()
{
	mpState->OnSolSendSuccess(this);
}

void Outstation::OnSolFailure()
{
	mpState->OnSolFailure(this);
	SIMPLE_LOG_BLOCK(logger, flags::WARN, "Response failure");
}

void Outstation::OnUnsolSendSuccess()
{
	mpState->OnUnsolSendSuccess(this);
}

void Outstation::OnUnsolFailure()
{
	mpState->OnUnsolFailure(this);
	SIMPLE_LOG_BLOCK(logger, flags::WARN, "Unsol response failure");
}

void Outstation::OnRequest(const APDURecord& record, SequenceInfo aSeqInfo)
{
	mpState->OnRequest(this, record, aSeqInfo);
}

/ --- Internally generated events --

void Outstation::OnDataUpdate()
{
	// let the current state decide how to handle the change buffer
	mpState->OnDataUpdate(this);
}

void Outstation::OnUnsolTimerExpiration()
{
	// let the current state decide how to handle the timer expiration
	mpUnsolTimer = nullptr;
	mpState->OnUnsolExpiration(this);
}


void Outstation::ChangeState(OutstationStateBase* apState)
{
	FORMAT_LOG_BLOCK(logger, flags::DBG, "State changed from %s to %s", mpState->Name(), apState->Name());
	mpState = apState;
	mpState->Enter(this);
}

void Outstation::RespondToRequest(const APDURecord& record, SequenceInfo sequence)
{
	if (!(record.function == FunctionCode::SELECT || record.function == FunctionCode::OPERATE))
	{
		selectBuffer.Clear();
	}

	APDUResponse response(responseBuffer.GetWriteBuffer(mConfig.maxFragSize));
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField::DEFAULT.ToByte());
	auto indications = ConfigureResponse(record, sequence, response);
	lastResponse = response;
	this->SendResponse(response, indications);
}

IINField Outstation::ConfigureResponse(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	switch(request.function)
	{
	case(FunctionCode::READ) :
		return HandleRead(request, sequence, response);
	case(FunctionCode::WRITE):
		return HandleWrite(request, sequence);
	case(FunctionCode::SELECT) :
		return HandleSelect(request, sequence, response);
	case(FunctionCode::OPERATE) :
		return HandleOperate(request, sequence, response);
	case(FunctionCode::DIRECT_OPERATE) :
		return HandleDirectOperate(request, sequence, response);
	case(FunctionCode::DELAY_MEASURE):
		return HandleDelayMeasure(request, sequence, response);
	default:
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, SERR_FUNC_NOT_SUPPORTED, "Function not supported: %s", FunctionCodeToString(request.function));
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField Outstation::HandleWrite(const APDURecord& request, SequenceInfo sequence)
{
	WriteHandler handler(logger, mpTimeWriteHandler, &staticIIN);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger);
	if (result == APDUParser::Result::OK)
	{
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField Outstation::HandleRead(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	rspContext.Reset();
	ReadHandler handler(logger, rspContext);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger, APDUParser::Context(false)); // don't expect range/count context on a READ
	if(result == APDUParser::Result::OK)
	{
		// Do a transaction on the database (lock) for multi-threaded environments
		// if the request contained static variations, we double buffer (copy) the entire static database.
		// this ensures that an multi-fragmented responses see a consistent snapshot
		openpal::Transaction tx(mpDatabase);
		mpDatabase->DoubleBuffer(); // todo, make this optional?
		auto control = rspContext.Load(response);
		response.SetControl(control.ToByte());
		return handler.Errors();
	}
	else
	{
		rspContext.Reset();
		return IINFromParseResult(result);
	}
}

IINField Outstation::HandleSelect(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	// Outstations require 2 extra bytes for IIN bits. Therefore it's possible
	// there are some requests you cannot possible answer since responses are
	// an exact echo the requests with status fields changed.
	if(request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());
		selectBuffer.Clear();
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(this->mpCmdHandler, true);
		CommandResponseHandler handler(logger, mConfig.maxControls, &adapter, response);
		auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger);
		if (result == APDUParser::Result::OK)
		{
			if(handler.AllCommandsSuccessful())
			{
				auto result = selectBuffer.Select(request.control.SEQ, request.objects);
				switch (result)
				{
				case(SelectBuffer::SelectResult::OK) :
				case(SelectBuffer::SelectResult::REPEAT):
					return IINField::Empty;
				default:
					return IINField(IINBit::PARAM_ERROR);
				}
			}
			else
			{
				return IINField::Empty;
			}
		}
		else return IINFromParseResult(result);
	}
}

IINField Outstation::HandleOperate(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	if (request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());
		selectBuffer.Clear();
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		auto result = selectBuffer.Operate(request.control.SEQ, request.objects);
		switch (result)
		{
		case(SelectBuffer::OperateResult::TIMEOUT):
			return HandleCommandWithConstant(request, response, CommandStatus::TIMEOUT);
		case(SelectBuffer::OperateResult::REPEAT):
			{
				// respond with the last response
				response = lastResponse;
				return lastResponse.GetIIN();
			}
		case(SelectBuffer::OperateResult::OK) :
			{
				CommandActionAdapter adapter(this->mpCmdHandler, false);
				CommandResponseHandler handler(logger, mConfig.maxControls, &adapter, response);
				auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger);
				return IINFromParseResult(result);
			}
		default:
			return HandleCommandWithConstant(request, response, CommandStatus::NO_SELECT);
		}
	}
}

IINField Outstation::HandleDirectOperate(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	if (request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(this->mpCmdHandler, false); // do the operation
		CommandResponseHandler handler(logger, mConfig.maxControls, &adapter, response);
		auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger);
		return IINFromParseResult(result);
	}
}

void Outstation::ContinueResponse()
{
	APDUResponse response(responseBuffer.GetWriteBuffer(mConfig.maxFragSize));
	response.SetFunction(FunctionCode::RESPONSE);
	
	{
		// perform a transaction (lock) the database
		openpal::Transaction tx(mpDatabase);
		auto control = this->rspContext.Load(response);
		response.SetControl(control.ToByte());
	}
	this->SendResponse(response);
}

IINField Outstation::HandleDelayMeasure(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	if(request.objects.IsEmpty())
	{
		auto writer = response.GetWriter();
		Group52Var2 value = { 0 }; 	// respond with 0 time delay
		writer.WriteSingleValue<UInt8, Group52Var2>(QualifierCode::UINT8_CNT, value);
		return IINField::Empty;
	}
	else
	{
		// there shouldn't be any trailing headers in delay measure request, no need to even parse
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField Outstation::HandleCommandWithConstant(const APDURecord& request, APDUResponse& response, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(logger, mConfig.maxControls, &constant, response);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger);
	return IINFromParseResult(result);
}

void Outstation::SendResponse(APDUResponse& response, const IINField& indications)
{
	IINField responseIIN(staticIIN | GetDynamicIIN() | indications);
	response.SetIIN(responseIIN);
	mpAppLayer->SendResponse(response);
}

IINField Outstation::GetDynamicIIN()
{
	IINField ret;
	auto tracker = eventBuffer.UnselectedEvents();
	if (tracker.class1.HasEvents())
	{
		ret.Set(IINBit::CLASS1_EVENTS);
	}
	if (tracker.class2.HasEvents())
	{
		ret.Set(IINBit::CLASS2_EVENTS);
	}
	if (tracker.class3.HasEvents())
	{
		ret.Set(IINBit::CLASS3_EVENTS);
	}
	if (eventBuffer.IsOverflown())
	{
		ret.Set(IINBit::EVENT_BUFFER_OVERFLOW);
	}
	return ret;
}

void Outstation::StartUnsolTimer(openpal::TimeDuration aTimeout)
{
	assert(mpUnsolTimer == nullptr);
	auto lambda = [this]() { this->OnUnsolTimerExpiration(); };
	mpUnsolTimer = pExecutor->Start(aTimeout, Bind(lambda));
}

void Outstation::ResetTimeIIN()
{
	mpTimeTimer = nullptr;
	staticIIN.Set(IINBit::NEED_TIME);
	auto lambda = [this]() { this->ResetTimeIIN(); };
	mpTimeTimer = pExecutor->Start(mConfig.timeSyncPeriod, Bind(lambda));
}

} //end ns

*/


