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

#include "CommandHandlerImpl.h"

#include <avr/io.h>

#include "Macros.h"

using namespace opendnp3;

CommandStatus CommandHandlerImpl::Select(const ControlRelayOutputBlock& command, uint16_t index)
{
	if(index == 0 && (command.functionCode == ControlCode::LATCH_OFF || command.functionCode == ControlCode::LATCH_ON))
	{
		return CommandStatus::SUCCESS;
	}
	else
	{
		return CommandStatus::NOT_SUPPORTED;
	}
}

CommandStatus CommandHandlerImpl::Operate(const ControlRelayOutputBlock& command, uint16_t index)
{
	if(index == 0)
	{
		if(command.functionCode == ControlCode::LATCH_ON)
		{
			SET(PORTB, BIT(7));
			return CommandStatus::SUCCESS;
		}
		else if(command.functionCode == ControlCode::LATCH_OFF)
		{
			CLEAR(PORTB, BIT(7));
			return CommandStatus::SUCCESS;
		}
		else
		{
			return CommandStatus::NOT_SUPPORTED;
		}
	}
	else
	{
		return CommandStatus::NOT_SUPPORTED;
	}
}


CommandStatus CommandHandlerImpl::Select(const AnalogOutputInt16& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }
CommandStatus CommandHandlerImpl::Operate(const AnalogOutputInt16& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }


CommandStatus CommandHandlerImpl::Select(const AnalogOutputInt32& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }
CommandStatus CommandHandlerImpl::Operate(const AnalogOutputInt32& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }


CommandStatus CommandHandlerImpl::Select(const AnalogOutputFloat32& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }
CommandStatus CommandHandlerImpl::Operate(const AnalogOutputFloat32& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }


CommandStatus CommandHandlerImpl::Select(const AnalogOutputDouble64& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }
CommandStatus CommandHandlerImpl::Operate(const AnalogOutputDouble64& command, uint16_t index) { return CommandStatus::NOT_SUPPORTED; }

