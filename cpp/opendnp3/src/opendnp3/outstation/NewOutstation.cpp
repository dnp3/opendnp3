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

#include "NewOutstation.h"

#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/app/APDUParser.h"

#include "opendnp3/outstation/ReadHandler.h"
#include "opendnp3/outstation/WriteHandler.h"
#include "opendnp3/outstation/IINHelpers.h"
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/CommandResponseHandler.h"
#include "opendnp3/outstation/ConstantCommandAction.h"

#include <openpal/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

NewOutstation::NewOutstation(
		const NewOutstationConfig& config,
		IExecutor& executor,
		openpal::LogRoot& root,
		openpal::ILowerLayer& lower,
		ICommandHandler& commandHandler,
		ITimeWriteHandler& timeWriteHandler,
		Database& database,
		const EventBufferFacade& buffers) :
		context(config, executor, root, lower, commandHandler, timeWriteHandler, database, buffers)
{
	
}
	
void NewOutstation::OnLowerLayerUp()
{
	if (context.isOnline)
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "already online");		
	}
	else
	{
		context.SetOnline();
	}
}
	
void NewOutstation::OnLowerLayerDown()
{
	if (context.isOnline)
	{
		context.SetOffline();
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "not online");
	}
}

void NewOutstation::OnReceive(const openpal::ReadOnlyBuffer& fragment)
{
	if (context.isOnline)
	{
		++context.rxFragCount;
		APDURecord request;
		auto result = APDUHeaderParser::ParseRequest(fragment, request);
		if (result == APDUHeaderParser::Result::OK)
		{	
			// outstations only have to process single fragment messages
			if ((request.control.FIR && request.control.FIN) && !request.control.CON)
			{
				if (request.control.UNS)
				{
					if (request.function == FunctionCode::CONFIRM)
					{
						this->OnReceiveUnsolConfirm(request);
					}
					else
					{
						SIMPLE_LOG_BLOCK(context.logger, flags::WARN, "Receive non-confirm unsol message");
					}					
				}
				else
				{
					if (request.function == FunctionCode::CONFIRM)
					{
						this->OnReceiveSolConfirm(request);
					}
					else
					{
						this->OnReceiveSolRequest(request, fragment);
					}					
				}
			}
			else
			{
				FORMAT_LOG_BLOCK(context.logger, flags::WARN,
					"Ignoring fragment with FIR: %u FIN: %u CON: %u", 
					request.control.FIN, 
					request.control.FIN,
					request.control.CON);
			}
		}
		else
		{
			SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "ignoring malformed request header");
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "ignoring received data while offline");
	}
}

void NewOutstation::OnReceiveSolConfirm(const APDURecord& request)
{
	if (context.IsExpectingSolConfirm())
	{
		if (request.control.SEQ == context.expectedConfirmSeq)
		{
			context.CancelConfirmTimer();
			context.solConfirmWait = false;
			context.eventBuffer.Clear(); // clear selected events
			if (context.rspContext.IsComplete())
			{
				this->EnterIdleState();
			}
			else // Continue response
			{								
				auto response = context.StartNewResponse();
				response.SetFunction(FunctionCode::RESPONSE);				
				openpal::Transaction tx(context.pDatabase);
				context.pDatabase->DoubleBuffer();
				auto control = context.rspContext.Load(response);
				control.SEQ = OutstationContext::NextSeq(request.control.SEQ);
				response.SetControl(control);
				response.SetIIN(context.staticIIN | context.GetDynamicIIN());
				this->BeginTransmission(control.SEQ, control.CON, response.ToReadOnly());
			}			
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "Received unexpected solicited confirm");
	}
}

void NewOutstation::OnSendResult(bool isSucccess)
{
	if (context.isOnline && context.isSending)
	{
		context.isSending = false;
		if(context.solConfirmWait)
		{
			auto onTimeout = [this](){ this->OnSolConfirmTimeout();  };
			context.StartConfirmTimer(onTimeout);
		}
		else
		{
			this->EnterIdleState();
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "Unexpected send callback");
	}
}

void NewOutstation::OnSolConfirmTimeout()
{
	if (context.solConfirmWait && context.pConfirmTimer)
	{
		context.pConfirmTimer = nullptr;
		context.solConfirmWait = false;
		context.eventBuffer.Reset();
		context.rspContext.Reset();
	}	
}

void NewOutstation::EnterIdleState()
{
	// post these calls so the stack can unwind
	auto lambda = [this]() { this->CheckForIdleState(); };
	context.pExecutor->PostLambda(lambda);
}

void NewOutstation::CheckForIdleState()
{
	if (context.IsIdle())
	{
		// TODO - check for idle actions
	}
}

void NewOutstation::OnReceiveSolRequest(const APDURecord& request, const openpal::ReadOnlyBuffer& fragment)
{
	if (context.IsIdle())
	{
		if (context.firstValidRequestAccepted)
		{
			if (context.solSeqN == request.control.SEQ)
			{
				if (context.lastValidRequest.Equals(fragment))
				{
					// duplicate message so just send the same response without processing
					context.isSending = true;
					context.pLower->BeginTransmit(context.lastResponse);					
				}
				else // new operation with same SEQ
				{
					if (request.function != FunctionCode::SELECT) // TODO - Ask why select is special?
					{
						this->ProcessRequest(request, fragment);
					}
				}
			}
			else  // completely new sequence #
			{				
				context.solSeqN = request.control.SEQ;
				this->ProcessRequest(request, fragment);
			}
		}
		else
		{
			context.solSeqN = request.control.SEQ;
			context.firstValidRequestAccepted = true;		
			this->ProcessRequest(request, fragment);
		}	
	}
	else
	{
		if (context.isSending)
		{
			// since we can't answer right now, store the request for later processing
			context.deferredRequest.Set(context.RecordLastRequest(fragment));
		}
		else
		{
			if (context.solConfirmWait)
			{

			}
		}		
	}		
}

void NewOutstation::ProcessRequest(const APDURecord& request, const openpal::ReadOnlyBuffer& fragment)
{		
	auto response = context.StartNewResponse();
	response.SetFunction(FunctionCode::RESPONSE);	
	response.SetControl(request.control);
	IINField iin = BuildResponse(request, response);
	context.RecordLastRequest(fragment);
	response.SetIIN(iin | context.staticIIN | context.GetDynamicIIN());
	this->BeginTransmission(request.control.SEQ, response.GetControl().CON, response.ToReadOnly());	
}

void NewOutstation::BeginTransmission(uint8_t seq, bool confirm, const ReadOnlyBuffer& response)
{
	context.isSending = true;	
	if (confirm)
	{
		context.expectedConfirmSeq = seq;
		context.solConfirmWait = true;
	}	
	context.lastResponse = response;
	context.pLower->BeginTransmit(response);
	/*
	auto output = response.ToReadOnly();
	auto lambda = [this, output]() { this->context.pLower->BeginTransmit(output); };
	context.pExecutor->PostLambda(lambda);
	*/
}

void NewOutstation::OnReceiveUnsolConfirm(const APDURecord& record)
{
	// can only be a confirm? ignore for now
	SIMPLE_LOG_BLOCK(context.logger, flags::WARN, "Unexpected unsolicited confirm");
}

IINField NewOutstation::BuildResponse(const APDURecord& request, APDUResponse& response)
{
	switch (request.function)
	{
		case(FunctionCode::READ):
			return HandleRead(request, response);
		case(FunctionCode::WRITE) :
			return HandleWrite(request);
		case(FunctionCode::SELECT) :
			return HandleSelect(request, response);
		case(FunctionCode::OPERATE) :
			return HandleOperate(request, response);
		case(FunctionCode::DIRECT_OPERATE) :
			return HandleDirectOperate(request, response);
		case(FunctionCode::DELAY_MEASURE) :
			return HandleDelayMeasure(request, response);
		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField NewOutstation::HandleRead(const APDURecord& request, APDUResponse& response)
{
	context.rspContext.Reset();
	ReadHandler handler(context.logger, context.rspContext);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger, APDUParser::Context(false)); // don't expect range/count context on a READ
	if (result == APDUParser::Result::OK)
	{
		// Do a transaction on the database (lock) for multi-threaded environments
		// if the request contained static variations, we double buffer (copy) the entire static database.
		// this ensures that an multi-fragmented responses see a consistent snapshot
		openpal::Transaction tx(context.pDatabase);
		context.pDatabase->DoubleBuffer();
		auto control = context.rspContext.Load(response);
		control.SEQ = request.control.SEQ;
		response.SetControl(control);
		return handler.Errors();
	}
	else
	{
		context.rspContext.Reset();
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField NewOutstation::HandleWrite(const APDURecord& request)
{
	WriteHandler handler(context.logger, context.pTimeWriteHandler, &context.staticIIN);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger);
	if (result == APDUParser::Result::OK)
	{
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField NewOutstation::HandleDirectOperate(const APDURecord& request, APDUResponse& response)
{
	// since we're echoing, make sure there's enough size before beginning
	if (request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(context.logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(context.pCommandHandler, false);
		CommandResponseHandler handler(context.logger, context.params.maxControlsPerRequest, &adapter, response);
		auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger);
		return IINFromParseResult(result);
	}
}

IINField NewOutstation::HandleSelect(const APDURecord& request, APDUResponse& response)
{
	// since we're echoing, make sure there's enough size before beginning
	if(request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(context.logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());		
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{		
		CommandActionAdapter adapter(context.pCommandHandler, true);		
		CommandResponseHandler handler(context.logger, context.params.maxControlsPerRequest, &adapter, response);
		auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger);
		if (result == APDUParser::Result::OK)
		{
			if(handler.AllCommandsSuccessful())
			{
				context.Select();
				return IINField::Empty;
			}
			else
			{
				return IINField::Empty;
			}
		}
		else
		{
			return IINFromParseResult(result);
		}
	}
}

IINField NewOutstation::HandleOperate(const APDURecord& request, APDUResponse& response)
{
	// since we're echoing, make sure there's enough size before beginning
	if (request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(context.logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());		
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		if (context.IsOperateSequenceValid())
		{				
			auto elapsed = context.pExecutor->GetTime().milliseconds - context.selectTime.milliseconds;
			if (elapsed < context.params.selectTimeout.GetMilliseconds())
			{
				if (context.lastValidRequest.Size() >= 2)
				{
					ReadOnlyBuffer copy(context.lastValidRequest);
					copy.Advance(2);
					if (copy.Equals(request.objects))
					{
						CommandActionAdapter adapter(context.pCommandHandler, false);
						CommandResponseHandler handler(context.logger, context.params.maxControlsPerRequest, &adapter, response);
						auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger);
						return IINFromParseResult(result);
					}
					else
					{
						return HandleCommandWithConstant(request, response, CommandStatus::NO_SELECT);
					}
				}
				else
				{
					return HandleCommandWithConstant(request, response, CommandStatus::NO_SELECT);
				}
			}
			else
			{
				return HandleCommandWithConstant(request, response, CommandStatus::TIMEOUT);
				
			}
		}
		else
		{
			return HandleCommandWithConstant(request, response, CommandStatus::NO_SELECT);
		}		
	}
}

IINField NewOutstation::HandleDelayMeasure(const APDURecord& request, APDUResponse& response)
{
	if (request.objects.IsEmpty())
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

IINField NewOutstation::HandleCommandWithConstant(const APDURecord& request, APDUResponse& response, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(context.logger, context.params.maxControlsPerRequest, &constant, response);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger);
	return IINFromParseResult(result);
}
	
}


