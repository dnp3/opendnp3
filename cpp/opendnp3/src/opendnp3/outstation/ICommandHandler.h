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
#ifndef __I_COMMAND_HANDLER_H_
#define __I_COMMAND_HANDLER_H_

#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3
{

/**
* Interface used to dispatch SELECT / OPERATE / DIRECT OPERATE (Binary/Analog output) from the outstation to application code.
*/
class ICommandHandler
{
public:
	virtual ~ICommandHandler() {}

	/**
	* Ask if the application supports a ControlRelayOutputBlock - group 12 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Supports(const ControlRelayOutputBlock& arCommand, uint16_t aIndex) = 0;

	/**
	* Operate a ControlRelayOutputBlock - group 12 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Perform(const ControlRelayOutputBlock& arCommand, uint16_t aIndex) = 0;


	/**
	* Ask if the application supports a 16 bit analog output - group 41 variation 2
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Supports(const AnalogOutputInt16& arCommand, uint16_t aIndex) = 0;

	/**
	* Ask if the application supports a 16 bit analog output - group 41 variation 2
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Perform(const AnalogOutputInt16& arCommand, uint16_t aIndex) = 0;


	/**
	* Ask if the application supports a 32 bit analog output - group 41 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Supports(const AnalogOutputInt32& arCommand, uint16_t aIndex) = 0;

	/**
	* Operate a 32 bit analog output - group 41 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Perform(const AnalogOutputInt32& arCommand, uint16_t aIndex) = 0;

	/**
	* Ask if the application supports a single precision, floating point analog output - group 41 variation 3
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Supports(const AnalogOutputFloat32& arCommand, uint16_t aIndex) = 0;

	/**
	* Operate a single precision, floating point analog output - group 41 variation 3
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Perform(const AnalogOutputFloat32& arCommand, uint16_t aIndex) = 0;

	/**
	* Ask if the application supports a double precision, floating point analog output - group 41 variation 4
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Supports(const AnalogOutputDouble64& arCommand, uint16_t aIndex) = 0;

	/**
	* Operate a double precision, floating point analog output - group 41 variation 4
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Perform(const AnalogOutputDouble64& arCommand, uint16_t aIndex) = 0;

};

}

#endif

