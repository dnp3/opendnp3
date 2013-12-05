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
#include "OutstationSBOHandler.h"

using namespace openpal;

namespace opendnp3
{

OutstationSBOHandler::OutstationSBOHandler(TimeDuration aSelectTimeout, ICommandHandler* apCmdHandler, IExecutor* apExecutor) :
	mSelectTimeout(aSelectTimeout),
	mpCmdHandler(apCmdHandler),
	mpExecutor(apExecutor),
	mCurrentSequenceNum(0)
{

}

void OutstationSBOHandler::ClearAll()
{
	mCROBSelectMap.clear();
	mAnalog16SelectMap.clear();
	mAnalog32SelectMap.clear();
	mAnalogFloatSelectMap.clear();
	mAnalogDoubleSelectMap.clear();
}

CommandStatus OutstationSBOHandler::Select(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mCROBSelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mCROBSelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalog16SelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalog16SelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalog32SelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalog32SelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalogFloatSelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalogFloatSelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalogDoubleSelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalogDoubleSelectMap);
}

}
