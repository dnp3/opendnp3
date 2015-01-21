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

#include "OutstationContext.h"

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/APDULogging.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/app/APDUParser.h"
#include "opendnp3/app/APDUHeaderParser.h"

#include "opendnp3/outstation/ReadHandler.h"
#include "opendnp3/outstation/WriteHandler.h"
#include "opendnp3/outstation/IINHelpers.h"
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/CommandResponseHandler.h"
#include "opendnp3/outstation/ConstantCommandAction.h"
#include "opendnp3/outstation/EventWriter.h"

#include "opendnp3/outstation/ClassBasedRequestHandler.h"
#include "opendnp3/outstation/AssignClassHandler.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

void OutstationContext::OnNewEventData(OState& ostate)
{
	if (!ostate.pendingTaskCheckFlag)
	{
		ostate.pendingTaskCheckFlag = true;
		// post these calls so the stack can unwind
		auto lambda = [&]() 
		{ 
			ostate.pendingTaskCheckFlag = false;
			OutstationContext::CheckForTaskStart(ostate);
		};
		ostate.pExecutor->PostLambda(lambda);
	}
}

IINField OutstationContext::GetDynamicIIN(OState& ostate)
{	
	auto classField = ostate.eventBuffer.UnwrittenClassField();

	IINField ret;
	ret.SetBitToValue(IINBit::CLASS1_EVENTS, classField.HasClass1());
	ret.SetBitToValue(IINBit::CLASS2_EVENTS, classField.HasClass2());
	ret.SetBitToValue(IINBit::CLASS3_EVENTS, classField.HasClass3());
	ret.SetBitToValue(IINBit::EVENT_BUFFER_OVERFLOW, ostate.eventBuffer.IsOverflown());
	
	return ret;
}

IINField OutstationContext::GetResponseIIN(OState& ostate)
{
	return ostate.staticIIN | GetDynamicIIN(ostate) | ostate.pApplication->GetApplicationIIN().ToIIN();
}

void OutstationContext::ConfigureUnsolHeader(OState& ostate, APDUResponse& unsol)
{	
	build::NullUnsolicited(unsol, ostate.unsol.seqN, GetResponseIIN(ostate));
}

void OutstationContext::OnReceiveAPDU(OState& ostate, const openpal::ReadBufferView& apdu)
{	
	APDUHeader header;	
	if (APDUHeaderParser::ParseRequest(apdu, header, &ostate.logger))
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::APP_HEADER_RX,
			"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s",
			header.control.FIR,
			header.control.FIN,
			header.control.CON,
			header.control.UNS,
			header.control.SEQ,
			FunctionCodeToString(header.function));

		// outstations should only process single fragment messages that don't request confirmation
		if (header.control.IsFirAndFin() && !header.control.CON)
		{
			auto objects = apdu.Skip(APDU_REQUEST_HEADER_SIZE);
			OutstationContext::ExamineASDU(ostate, header, objects);			
			OutstationContext::CheckForTaskStart(ostate);
		}
		else
		{
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring fragment. Requst must be FIR/FIN/!CON");
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(ostate.logger, flags::ERR, "ignoring malformed request header");
	}	
}

void OutstationContext::OnSendResult(OState& ostate, bool isSuccess)
{
	if (ostate.isOnline && ostate.isTransmitting)
	{
		ostate.isTransmitting = false;
		OutstationContext::CheckForTaskStart(ostate);
	}	
}

void OutstationContext::ExamineASDU(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		if (header.function == FunctionCode::CONFIRM)
		{
			ostate.unsol.pState = ostate.unsol.pState->OnConfirm(ostate, header);
		}
		else
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring unsol with invalid function code: %s", FunctionCodeToString(header.function));
		}
	}
	else
	{
		if (header.function == FunctionCode::CONFIRM)
		{
			ostate.sol.pState = ostate.sol.pState->OnConfirm(ostate, header);
		}
		else
		{
			ostate.sol.pState = OutstationContext::OnReceiveSolRequest(ostate, header, objects);
			ostate.history.RecordLastRequest(header, objects);
		}
	}
}

OutstationSolicitedStateBase* OutstationContext::OnReceiveSolRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{			
	// analyze this request to see how it compares to the last request
	if (ostate.history.HasLastRequest())
	{		
		if (ostate.sol.seqN == header.control.SEQ)
		{			
			if (ostate.history.FullyEqualsLastRequest(header, objects))
			{
				if (header.function == FunctionCode::READ)
				{
					SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring repeat read request");
					return ostate.sol.pState;
				}
				else
				{
					return ostate.sol.pState->OnRepeatNonReadRequest(ostate, header, objects);
				}
			}
			else // new operation with same SEQ
			{
				return ProcessNewRequest(ostate, header, objects);
			}
		}
		else  // completely new sequence #
		{
			return ProcessNewRequest(ostate, header, objects);
		}		
	}
	else
	{		
		return ProcessNewRequest(ostate, header, objects);
	}	
}

OutstationSolicitedStateBase* OutstationContext::ProcessNewRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.sol.seqN = header.control.SEQ;

	if (header.function == FunctionCode::READ)
	{
		return ostate.sol.pState->OnNewReadRequest(ostate, header, objects);
	}
	else
	{
		return ostate.sol.pState->OnNewNonReadRequest(ostate, header, objects);
	}
}

OutstationSolicitedStateBase* OutstationContext::RespondToNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	auto format = [&](APDUResponse& response) {
		auto writer = response.GetWriter();
		response.SetFunction(FunctionCode::RESPONSE);
		response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
		auto iin = BuildNonReadResponse(ostate, header, objects, writer);
		response.SetIIN(iin | GetResponseIIN(ostate));
	};

	auto response = ostate.txBuffers.FormatSolicited(format);		
	OutstationContext::BeginResponseTx(ostate, response.ToReadOnly());
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OutstationContext::RespondToReadRequest(OState& ostate, uint8_t seq, const openpal::ReadBufferView& objects)
{
	auto format = [&](APDUResponse& response) {
		auto writer = response.GetWriter();
		response.SetFunction(FunctionCode::RESPONSE);
		auto result = OutstationContext::HandleRead(ostate, objects, writer);
		result.second.SEQ = seq;
		ostate.sol.expectedConSeqN = seq;
		response.SetControl(result.second);
		response.SetIIN(result.first | OutstationContext::GetResponseIIN(ostate));
	};

	auto response = ostate.txBuffers.FormatSolicited(format);
	OutstationContext::BeginResponseTx(ostate, response.ToReadOnly());
	if (response.GetControl().CON)
	{
		OutstationContext::StartSolicitedConfirmTimer(ostate);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return  &OutstationSolicitedStateIdle::Inst();
	}	
}

void OutstationContext::ProcessNoResponseFunction(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
		case(FunctionCode::DIRECT_OPERATE_NR) :
			OutstationContext::HandleDirectOperate(ostate, objects, nullptr); // no object writer, this is a no ack code
			break;
		default:
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring NR function code: %s", FunctionCodeToString(header.function));
			break;
	}	
}

void OutstationContext::BeginResponseTx(OState& ostate, const ReadBufferView& response)
{		
	logging::ParseAndLogResponseTx(&ostate.logger, response);
	ostate.isTransmitting = true;	
	ostate.pLower->BeginTransmit(response);
}

void OutstationContext::BeginUnsolTx(OState& ostate, const ReadBufferView& response)
{	
	logging::ParseAndLogResponseTx(&ostate.logger, response);
	ostate.isTransmitting = true;
	ostate.unsol.expectedConSeqN = ostate.unsol.seqN;
	ostate.unsol.seqN = AppControlField::NextSeq(ostate.unsol.seqN);
	ostate.pLower->BeginTransmit(response);
}

IINField OutstationContext::BuildNonReadResponse(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	switch (header.function)
	{				
		case(FunctionCode::WRITE) :
			return HandleWrite(ostate, objects);
		case(FunctionCode::SELECT) :
			return HandleSelect(ostate, objects, writer);
		case(FunctionCode::OPERATE) :
			return HandleOperate(ostate, objects, writer);
		case(FunctionCode::DIRECT_OPERATE) :
			return HandleDirectOperate(ostate, objects, &writer);
		case(FunctionCode::COLD_RESTART) :
			return HandleRestart(ostate, objects, false, &writer);
		case(FunctionCode::WARM_RESTART) :
			return HandleRestart(ostate, objects, true, &writer);
		case(FunctionCode::ASSIGN_CLASS) :
			return HandleAssignClass(ostate, objects);
		case(FunctionCode::DELAY_MEASURE) :
			return HandleDelayMeasure(ostate, objects, writer);
		case(FunctionCode::DISABLE_UNSOLICITED) :
			return ostate.params.allowUnsolicited ? HandleDisableUnsolicited(ostate, objects, writer) : IINField(IINBit::FUNC_NOT_SUPPORTED);
		case(FunctionCode::ENABLE_UNSOLICITED) :
			return ostate.params.allowUnsolicited ? HandleEnableUnsolicited(ostate, objects, writer) : IINField(IINBit::FUNC_NOT_SUPPORTED);
		default:
			return	IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

OutstationSolicitedStateBase* OutstationContext::ContinueMultiFragResponse(OState& ostate, uint8_t seq)
{	
	auto format = [&](APDUResponse& response) {
		auto writer = response.GetWriter();
		response.SetFunction(FunctionCode::RESPONSE);
		auto control = ostate.rspContext.LoadResponse(writer);
		control.SEQ = seq;
		ostate.sol.expectedConSeqN = seq;
		response.SetControl(control);
		response.SetIIN(ostate.staticIIN | GetDynamicIIN(ostate));
	};

	auto response = ostate.txBuffers.FormatSolicited(format);
	OutstationContext::BeginResponseTx(ostate, response.ToReadOnly());
	
	if (response.GetControl().CON)
	{
		OutstationContext::StartSolicitedConfirmTimer(ostate);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return &OutstationSolicitedStateIdle::Inst();
	}	
}

void OutstationContext::CheckForTaskStart(OState& ostate)
{		
	// if we're online, the solicited state is idle, and the unsolicited state 
	// is not transmitting we may be able to do a task
	if (ostate.isOnline && !ostate.isTransmitting && ostate.sol.IsIdle())
	{		
		if (ostate.deferred.IsSet())
		{			
			if (ostate.deferred.GetFunction() == FunctionCode::READ)
			{
				if (ostate.unsol.IsIdle())
				{					
					auto handler = [&](const APDUHeader& header, ReadBufferView objects, bool)
					{
						ostate.sol.pState = ostate.sol.pState->OnNewReadRequest(ostate, header, objects);
					};
					ostate.deferred.Process(handler);
				}
			}
			else
			{
				auto handler = [&](const APDUHeader& header, ReadBufferView objects, bool isRepeat)
				{
					if (isRepeat)
					{
						ostate.sol.pState = ostate.sol.pState->OnRepeatNonReadRequest(ostate, header, objects);
					}
					else
					{
						ostate.sol.pState = ostate.sol.pState->OnNewNonReadRequest(ostate, header, objects);
					}
				};

				ostate.deferred.Process(handler);
			}
		}
		else
		{		
			if (ostate.unsol.IsIdle())
			{
				OutstationContext::CheckForUnsolicited(ostate);
			}
		}
	}	
}

void OutstationContext::CheckForUnsolicited(OState& ostate)
{
	if (ostate.params.allowUnsolicited)
	{
		if (ostate.unsol.completedNull)
		{				
			// are there events to be reported?
			if (ostate.params.unsolClassMask.Intersects(ostate.eventBuffer.UnwrittenClassField()))
			{			
				auto format = [&](APDUResponse& response) 
				{
					auto writer = response.GetWriter();
					// even though we're not loading static data, we need to lock 
					// the database since it updates the event buffer
					Transaction tx(ostate.database);
					ostate.eventBuffer.Unselect();
					ostate.eventBuffer.SelectAllByClass(ostate.params.unsolClassMask);
					ostate.eventBuffer.Load(writer);
					OutstationContext::ConfigureUnsolHeader(ostate, response);
				};
			
				auto response = ostate.txBuffers.FormatUnsolicited(format);								
				OutstationContext::StartUnsolicitedConfirmTimer(ostate);
				ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
				OutstationContext::BeginUnsolTx(ostate, response.ToReadOnly());
			}			
		}
		else
		{
			// send a NULL unsolcited message			
			auto format = [&](APDUResponse& response) 
			{
				OutstationContext::ConfigureUnsolHeader(ostate, response);
			};
			
			auto response = ostate.txBuffers.FormatUnsolicited(format);
			OutstationContext::StartUnsolicitedConfirmTimer(ostate);
			ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
			OutstationContext::BeginUnsolTx(ostate, response.ToReadOnly());
		}
	}	
}

bool OutstationContext::StartSolicitedConfirmTimer(OState& ostate)
{
	auto timeout = [&]() 
	{ 
		ostate.sol.pState = ostate.sol.pState->OnConfirmTimeout(ostate);
		OutstationContext::CheckForTaskStart(ostate);
	};
	return ostate.confirmTimer.Start(ostate.params.unsolConfirmTimeout, timeout);
}

bool OutstationContext::StartUnsolicitedConfirmTimer(OState& ostate)
{
	auto timeout = [&]() 
	{ 
		ostate.unsol.pState = ostate.unsol.pState->OnConfirmTimeout(ostate);
		OutstationContext::CheckForTaskStart(ostate);
	};
	return ostate.confirmTimer.Start(ostate.params.unsolConfirmTimeout, timeout);
}

Pair<IINField, AppControlField> OutstationContext::HandleRead(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ostate.rspContext.Reset();

	// Do a transaction (lock) on the database  for multi-threaded environments
	Transaction tx(ostate.database);
	ostate.eventBuffer.Unselect(); // always unselect any perviously selected points when we start a new read request
	ostate.database.Unselect();
	ReadHandler handler(ostate.logger, ostate.database.GetSelector(), ostate.eventBuffer);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger, APDUParser::Settings::NoContents()); // don't expect range/count context on a READ
	if (result == APDUParser::Result::OK)
	{				
		auto control = ostate.rspContext.LoadResponse(writer);
		return Pair<IINField, AppControlField>(handler.Errors(), control);
	}
	else
	{
		ostate.rspContext.Reset();
		return Pair<IINField, AppControlField>(IINFromParseResult(result), AppControlField(true, true, false, false));
	}
}

IINField OutstationContext::HandleWrite(OState& ostate, const openpal::ReadBufferView& objects)
{
	WriteHandler handler(ostate.logger, *ostate.pApplication, &ostate.staticIIN);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
}

IINField OutstationContext::HandleDirectOperate(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter* pWriter)
{
	// since we're echoing, make sure there's enough size before beginning
	if (pWriter && (objects.Size() > pWriter->Remaining()))
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Igonring command request due to oversized payload size of %u", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(ostate.pCommandHandler, false);
		CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, pWriter);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
		return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleSelect(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Igonring command request due to oversized payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(ostate.pCommandHandler, true);
		CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, &writer);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
		if (result == APDUParser::Result::OK)
		{
			if (handler.AllCommandsSuccessful())
			{						
				ostate.control.Select(ostate.sol.seqN, ostate.pExecutor->GetTime(), objects);				
			}
			
			return handler.Errors();
		}
		else
		{
			return IINFromParseResult(result);
		}
	}
}

IINField OutstationContext::HandleOperate(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Igonring command request due to oversized payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		auto now = ostate.pExecutor->GetTime();
		auto result = ostate.control.ValidateSelection(ostate.sol.seqN, now, ostate.params.selectTimeout, objects);

		if (result == CommandStatus::SUCCESS)
		{
			CommandActionAdapter adapter(ostate.pCommandHandler, false);
			CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, &writer);
			auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
			return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
		}
		else
		{
			return HandleCommandWithConstant(ostate, objects, writer, result);
		}
	}
}

IINField OutstationContext::HandleDelayMeasure(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	if (objects.IsEmpty())
	{		
		Group52Var2 value = { 0 }; 	// respond with 0 time delay
		writer.WriteSingleValue<UInt8, Group52Var2>(QualifierCode::UINT8_CNT, value);
		return IINField::Empty();
	}
	else
	{
		// there shouldn't be any trailing headers in delay measure request, no need to even parse
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField OutstationContext::HandleRestart(OState& ostate, const openpal::ReadBufferView& objects, bool isWarmRestart, HeaderWriter* pWriter)
{
	if (objects.IsEmpty())
	{
		auto mode = isWarmRestart ? ostate.pApplication->WarmRestartSupport() : ostate.pApplication->ColdRestartSupport();

		switch (mode)
		{
			case(RestartMode::UNSUPPORTED) :
				return IINField(IINBit::FUNC_NOT_SUPPORTED);
			case(RestartMode::SUPPORTED_DELAY_COARSE) :
			{
				auto delay = isWarmRestart ? ostate.pApplication->WarmRestart() : ostate.pApplication->ColdRestart();
				if (pWriter)
				{
					Group52Var1 coarse = { delay };
					pWriter->WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, coarse);
				}
				return IINField::Empty();
			}
			default:
			{
				auto delay = isWarmRestart ? ostate.pApplication->WarmRestart() : ostate.pApplication->ColdRestart();
				if (pWriter)
				{
					Group52Var2 fine = { delay };
					pWriter->WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, fine);
				}
				return IINField::Empty();
			}
		}		
	}
	else
	{
		// there shouldn't be any trailing headers in restart requests, no need to even parse
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField OutstationContext::HandleAssignClass(OState& ostate, const openpal::ReadBufferView& objects)
{
	if (ostate.pApplication->SupportsAssignClass())
	{		
		AssignClassHandler handler(ostate.logger, *ostate.pExecutor, *ostate.pApplication, ostate.database.GetClassAssigner());

		// Lock the db as this can adjust configuration values in the database
		Transaction tx(ostate.database);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger, APDUParser::Settings::NoContents());
		return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
	}
	else
	{
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField OutstationContext::HandleDisableUnsolicited(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ClassBasedRequestHandler handler(ostate.logger);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	if (result == APDUParser::Result::OK)
	{
		ostate.params.unsolClassMask.Clear(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleEnableUnsolicited(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{	
	ClassBasedRequestHandler handler(ostate.logger);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	if (result == APDUParser::Result::OK)
	{
		ostate.params.unsolClassMask.Set(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleCommandWithConstant(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &constant, &writer);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	return IINFromParseResult(result);
}


}

