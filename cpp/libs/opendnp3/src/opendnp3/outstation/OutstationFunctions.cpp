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

void OFunctions::ProcessRequestNoAck(OContext& ocontext, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
		case(FunctionCode::DIRECT_OPERATE_NR) :
			OFunctions::HandleDirectOperate(ocontext, objects, nullptr); // no object writer, this is a no ack code
			break;
		default:
			FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Ignoring NR function code: %s", FunctionCodeToString(header.function));
			break;
	}
}

IINField OFunctions::HandleNonReadResponse(OContext& ocontext, const APDUHeader& header, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	switch (header.function)
	{				
		case(FunctionCode::WRITE) :
			return HandleWrite(ocontext, objects);
		case(FunctionCode::SELECT) :
			return HandleSelect(ocontext, objects, writer);
		case(FunctionCode::OPERATE) :
			return HandleOperate(ocontext, objects, writer);
		case(FunctionCode::DIRECT_OPERATE) :
			return HandleDirectOperate(ocontext, objects, &writer);
		case(FunctionCode::COLD_RESTART) :
			return HandleRestart(ocontext, objects, false, &writer);
		case(FunctionCode::WARM_RESTART) :
			return HandleRestart(ocontext, objects, true, &writer);
		case(FunctionCode::ASSIGN_CLASS) :
			return HandleAssignClass(ocontext, objects);
		case(FunctionCode::DELAY_MEASURE) :
			return HandleDelayMeasure(ocontext, objects, writer);
		case(FunctionCode::DISABLE_UNSOLICITED) :
			return ocontext.params.allowUnsolicited ? HandleDisableUnsolicited(ocontext, objects, writer) : IINField(IINBit::FUNC_NOT_SUPPORTED);
		case(FunctionCode::ENABLE_UNSOLICITED) :
			return ocontext.params.allowUnsolicited ? HandleEnableUnsolicited(ocontext, objects, writer) : IINField(IINBit::FUNC_NOT_SUPPORTED);
		default:
			return	IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

Pair<IINField, AppControlField> OFunctions::HandleRead(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ocontext.rspContext.Reset();	
	ocontext.eventBuffer.Unselect(); // always un-select any previously selected points when we start a new read request
	ocontext.database.Unselect();

	ReadHandler handler(ocontext.logger, ocontext.database.GetSelector(), ocontext.eventBuffer);
	auto result = APDUParser::Parse(objects, handler, &ocontext.logger, ParserSettings::NoContents()); // don't expect range/count context on a READ
	if (result == ParseResult::OK)
	{				
		auto control = ocontext.rspContext.LoadResponse(writer);
		return Pair<IINField, AppControlField>(handler.Errors(), control);
	}
	else
	{
		ocontext.rspContext.Reset();
		return Pair<IINField, AppControlField>(IINFromParseResult(result), AppControlField(true, true, false, false));
	}
}

IINField OFunctions::HandleWrite(OContext& ocontext, const openpal::ReadBufferView& objects)
{
	WriteHandler handler(ocontext.logger, *ocontext.pApplication, &ocontext.staticIIN);
	auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
	return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
}

IINField OFunctions::HandleDirectOperate(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter* pWriter)
{
	// since we're echoing, make sure there's enough size before beginning
	if (pWriter && (objects.Size() > pWriter->Remaining()))
	{
		FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Igonring command request due to oversized payload size of %u", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(ocontext.pCommandHandler, false);
		CommandResponseHandler handler(ocontext.logger, ocontext.params.maxControlsPerRequest, &adapter, pWriter);
		auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
		return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
	}
}

IINField OFunctions::HandleSelect(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Igonring command request due to oversized payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(ocontext.pCommandHandler, true);
		CommandResponseHandler handler(ocontext.logger, ocontext.params.maxControlsPerRequest, &adapter, &writer);
		auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
		if (result == ParseResult::OK)
		{
			if (handler.AllCommandsSuccessful())
			{						
				ocontext.control.Select(ocontext.sol.seq.num, ocontext.pExecutor->GetTime(), objects);
			}
			
			return handler.Errors();
		}
		else
		{
			return IINFromParseResult(result);
		}
	}
}

IINField OFunctions::HandleOperate(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Igonring command request due to oversized payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		auto now = ocontext.pExecutor->GetTime();
		auto result = ocontext.control.ValidateSelection(ocontext.sol.seq.num, now, ocontext.params.selectTimeout, objects);

		if (result == CommandStatus::SUCCESS)
		{
			CommandActionAdapter adapter(ocontext.pCommandHandler, false);
			CommandResponseHandler handler(ocontext.logger, ocontext.params.maxControlsPerRequest, &adapter, &writer);
			auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
			return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
		}
		else
		{
			return HandleCommandWithConstant(ocontext, objects, writer, result);
		}
	}
}

IINField OFunctions::HandleDelayMeasure(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer)
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

IINField OFunctions::HandleRestart(OContext& ocontext, const openpal::ReadBufferView& objects, bool isWarmRestart, HeaderWriter* pWriter)
{
	if (objects.IsEmpty())
	{
		auto mode = isWarmRestart ? ocontext.pApplication->WarmRestartSupport() : ocontext.pApplication->ColdRestartSupport();

		switch (mode)
		{
			case(RestartMode::UNSUPPORTED) :
				return IINField(IINBit::FUNC_NOT_SUPPORTED);
			case(RestartMode::SUPPORTED_DELAY_COARSE) :
			{
				auto delay = isWarmRestart ? ocontext.pApplication->WarmRestart() : ocontext.pApplication->ColdRestart();
				if (pWriter)
				{
					Group52Var1 coarse = { delay };
					pWriter->WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, coarse);
				}
				return IINField::Empty();
			}
			default:
			{
				auto delay = isWarmRestart ? ocontext.pApplication->WarmRestart() : ocontext.pApplication->ColdRestart();
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

IINField OFunctions::HandleAssignClass(OContext& ocontext, const openpal::ReadBufferView& objects)
{
	if (ocontext.pApplication->SupportsAssignClass())
	{		
		AssignClassHandler handler(ocontext.logger, *ocontext.pExecutor, *ocontext.pApplication, ocontext.database.GetClassAssigner());		
		auto result = APDUParser::Parse(objects, handler, &ocontext.logger, ParserSettings::NoContents());
		return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
	}
	else
	{
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField OFunctions::HandleDisableUnsolicited(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ClassBasedRequestHandler handler(ocontext.logger);
	auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
	if (result == ParseResult::OK)
	{
		ocontext.params.unsolClassMask.Clear(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OFunctions::HandleEnableUnsolicited(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{	
	ClassBasedRequestHandler handler(ocontext.logger);
	auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
	if (result == ParseResult::OK)
	{
		ocontext.params.unsolClassMask.Set(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OFunctions::HandleCommandWithConstant(OContext& ocontext, const openpal::ReadBufferView& objects, HeaderWriter& writer, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(ocontext.logger, ocontext.params.maxControlsPerRequest, &constant, &writer);
	auto result = APDUParser::Parse(objects, handler, &ocontext.logger);
	return IINFromParseResult(result);
}


}

