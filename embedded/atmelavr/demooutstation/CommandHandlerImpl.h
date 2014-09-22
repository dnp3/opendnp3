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
#ifndef __AVR_COMMAND_HANDLER_H_
#define __AVR_COMMAND_HANDLER_H_

#include <openpal/util/Uncopyable.h>

#include "opendnp3/outstation/ICommandHandler.h"

/**
* Mock ICommandHandler used for examples and demos
*/
class CommandHandlerImpl : public opendnp3::ICommandHandler, openpal::Uncopyable
{
public:	

	opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& command, uint16_t index) override final;
	opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index) override final;


	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& command, uint16_t index) override final;
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& command, uint16_t index) override final;


	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& command, uint16_t index) override final;
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& command, uint16_t index) override final;


	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index) override final;
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& command, uint16_t index) override final;


	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& command, uint16_t index) override final;
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& command, uint16_t index) override final;	
	
};

#endif

