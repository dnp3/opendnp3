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
#ifndef OPENDNP3_COMMANDMARSHALLER_H
#define OPENDNP3_COMMANDMARSHALLER_H

#include "ICommandProcessor.h"

#include <openpal/executor/IExecutor.h>

namespace opendnp3
{

/**
* Marshalls commands from any thread to an executor
*/
class CommandMarshaller : public ICommandProcessor
{
	
public:

	CommandMarshaller(openpal::IExecutor& executor, ICommandProcessor& proxyTo);

	// Implement the ICommandProcessor interface

	virtual void SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback) override final;
	virtual void DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback) override final;

	virtual void SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback) override final;
	virtual void DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback) override final;

	virtual void SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback) override final;
	virtual void DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback) override final;

	virtual void SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback) override final;
	virtual void DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback) override final;

	virtual void SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback) override final;
	virtual void DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback) override final;

private:

	openpal::IExecutor* pExecutor;
	ICommandProcessor* pProxyTo;
	
	template <class T>
	void SelectAndOperateT(const T& command, uint16_t index, ICommandCallback& callback)
	{
		auto pCallback = &callback;
		auto action = [command, index, pCallback, this]() { pProxyTo->SelectAndOperate(command, index, *pCallback); };
		pExecutor->PostLambda(action);
	}

	template <class T>
	void DirectOperateT(const T& command, uint16_t index, ICommandCallback& callback)
	{	
		auto pCallback = &callback;
		auto action = [command, index, pCallback, this]() { pProxyTo->DirectOperate(command, index, *pCallback); };
		pExecutor->PostLambda(action);
	}
};

}

#endif

