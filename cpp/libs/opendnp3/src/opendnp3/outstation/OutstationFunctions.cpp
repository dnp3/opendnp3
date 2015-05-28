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

#include "OutstationFunctions.h"

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/APDULogging.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/app/parsing/APDUHeaderParser.h"

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

void OFunctions::ProcessRequestNoAck(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
		case(FunctionCode::DIRECT_OPERATE_NR) :
			OFunctions::HandleDirectOperate(ostate, objects, nullptr); // no object writer, this is a no ack code
			break;
		default:
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring NR function code: %s", FunctionCodeToString(header.function));
			break;
	}
}

IINField OFunctions::HandleNonReadResponse(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects, HeaderWriter& writer)
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

Pair<IINField, AppControlField> OFunctions::HandleRead(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ostate.rspContext.Reset();	
	ostate.eventBuffer.Unselect(); // always un-select any previously selected points when we start a new read request
	ostate.database.Unselect();

	ReadHandler handler(ostate.logger, ostate.database.GetSelector(), ostate.eventBuffer);
	auto result = APDUParser::Parse(objects, handler, &ostate.logger, ParserSettings::NoContents()); // don't expect range/count context on a READ
	if (result == ParseResult::OK)
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

IINField OFunctions::HandleWrite(OState& ostate, const openpal::ReadBufferView& objects)
{
	WriteHandler handler(ostate.logger, *ostate.pApplication, &ostate.staticIIN);
	auto result = APDUParser::Parse(objects, handler, &ostate.logger);
	return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
}

IINField OFunctions::HandleDirectOperate(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter* pWriter)
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
		auto result = APDUParser::Parse(objects, handler, &ostate.logger);
		return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
	}
}

IINField OFunctions::HandleSelect(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
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
		auto result = APDUParser::Parse(objects, handler, &ostate.logger);
		if (result == ParseResult::OK)
		{
			if (handler.AllCommandsSuccessful())
			{						
				ostate.control.Select(ostate.sol.seq.num, ostate.pExecutor->GetTime(), objects);
			}
			
			return handler.Errors();
		}
		else
		{
			return IINFromParseResult(result);
		}
	}
}

IINField OFunctions::HandleOperate(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
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
		auto result = ostate.control.ValidateSelection(ostate.sol.seq.num, now, ostate.params.selectTimeout, objects);

		if (result == CommandStatus::SUCCESS)
		{
			CommandActionAdapter adapter(ostate.pCommandHandler, false);
			CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, &writer);
			auto result = APDUParser::Parse(objects, handler, &ostate.logger);
			return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
		}
		else
		{
			return HandleCommandWithConstant(ostate, objects, writer, result);
		}
	}
}

IINField OFunctions::HandleDelayMeasure(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
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

IINField OFunctions::HandleRestart(OState& ostate, const openpal::ReadBufferView& objects, bool isWarmRestart, HeaderWriter* pWriter)
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

IINField OFunctions::HandleAssignClass(OState& ostate, const openpal::ReadBufferView& objects)
{
	if (ostate.pApplication->SupportsAssignClass())
	{		
		AssignClassHandler handler(ostate.logger, *ostate.pExecutor, *ostate.pApplication, ostate.database.GetClassAssigner());		
		auto result = APDUParser::Parse(objects, handler, &ostate.logger, ParserSettings::NoContents());
		return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
	}
	else
	{
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField OFunctions::HandleDisableUnsolicited(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ClassBasedRequestHandler handler(ostate.logger);
	auto result = APDUParser::Parse(objects, handler, &ostate.logger);
	if (result == ParseResult::OK)
	{
		ostate.params.unsolClassMask.Clear(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OFunctions::HandleEnableUnsolicited(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{	
	ClassBasedRequestHandler handler(ostate.logger);
	auto result = APDUParser::Parse(objects, handler, &ostate.logger);
	if (result == ParseResult::OK)
	{
		ostate.params.unsolClassMask.Set(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OFunctions::HandleCommandWithConstant(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &constant, &writer);
	auto result = APDUParser::Parse(objects, handler, &ostate.logger);
	return IINFromParseResult(result);
}


}

