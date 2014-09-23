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
#ifndef OPENDNP3_ICOMMANDPROCESSOR_H
#define OPENDNP3_ICOMMANDPROCESSOR_H

#include "opendnp3/app/AnalogOutput.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/master/ICommandCallback.h"

namespace opendnp3
{

/**
* Interface used to dispatch SELECT / OPERATE / DIRECT OPERATE from application code to a master
*/
class ICommandProcessor
{
public:

	

	/**
	* Select and operate a ControlRelayOutputBlock
	* @param command command to operate
	* @param index index of the command
	* @param pCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Direct operate a ControlRelayOutputBlock
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Select and operate a 16 bit analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Direct operate a 16 bit analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Select and operate a 32 bit analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Direct operate a 32 bit analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Select and operate a single precision analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Direct operate a single precision analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Select and operate a double precision analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback) = 0;

	/**
	* Direct operate a double precision analog output
	* @param command command to operate
	* @param index index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback) = 0;
};

}

#endif
