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
#ifndef __SIMPLE_COMMAND_HANDLER_H_
#define __SIMPLE_COMMAND_HANDLER_H_

#include "ICommandHandler.h"

namespace opendnp3
{

/**
* Mock ICommandHandler used for examples and demos
*/
class SimpleCommandHandler : public ICommandHandler
{
public:

	/**
	* @param aStatusFunc functor used to retrieve that next CommandStatus enumeration
	*/
	SimpleCommandHandler(CommandStatus status);

	CommandStatus Supports(const ControlRelayOutputBlock& arCommand, uint16_t aIndex) override final;
	CommandStatus Perform(const ControlRelayOutputBlock& arCommand, uint16_t aIndex) override final;


	CommandStatus Supports(const AnalogOutputInt16& arCommand, uint16_t aIndex) override final;
	CommandStatus Perform(const AnalogOutputInt16& arCommand, uint16_t aIndex) override final;


	CommandStatus Supports(const AnalogOutputInt32& arCommand, uint16_t aIndex) override final;
	CommandStatus Perform(const AnalogOutputInt32& arCommand, uint16_t aIndex) override final;


	CommandStatus Supports(const AnalogOutputFloat32& arCommand, uint16_t aIndex) override final;
	CommandStatus Perform(const AnalogOutputFloat32& arCommand, uint16_t aIndex) override final;


	CommandStatus Supports(const AnalogOutputDouble64& arCommand, uint16_t aIndex) override final;
	CommandStatus Perform(const AnalogOutputDouble64& arCommand, uint16_t aIndex) override final;


protected:
	CommandStatus status;
	uint32_t numInvocations;
	
};

class SuccessCommandHandler : public SimpleCommandHandler
{
public:
	static SuccessCommandHandler* Inst()
	{
		return &handler;
	}

private:
	static SuccessCommandHandler handler;

protected:
	SuccessCommandHandler();
};

}

#endif

