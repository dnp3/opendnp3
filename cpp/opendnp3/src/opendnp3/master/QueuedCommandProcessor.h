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
#ifndef __QUEUED_COMMAND_PROCESSOR_H_
#define __QUEUED_COMMAND_PROCESSOR_H_

#include "ICommandProcessor.h"

#include "opendnp3/master/AsyncTaskInterfaces.h"
#include "opendnp3/StaticSizeConfiguration.h"
#include "CommandAction.h"

#include <openpal/IExecutor.h>
#include <openpal/StaticQueue.h>

namespace opendnp3
{

class QueuedCommandProcessor : public ICommandProcessor
{
	
public:

	QueuedCommandProcessor(openpal::IExecutor* pExecutor_, ITask* pEnableTask_);

	// Implement the ICommandProcessor interface

	void SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback);
	void DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback);

	void SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback);
	void DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback);

	void SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback);
	void DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback);

	void SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback);
	void DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback);

	void SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback);
	void DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback);

	// Function used to marshall calls another ICommandProcessor

	bool Dispatch(ICommandProcessor* apProcessor);

private:

	openpal::IExecutor* pExecutor;
	ITask* pEnableTask;

	openpal::StaticQueue<CommandErasure, uint8_t, sizes::MAX_COMMAND_QUEUE_SIZE> requestQueue;

	void Enque(const CommandErasure& erasure, ICommandCallback* pCallback);
	
	template <class T>
	void SelectAndOperateT(T command, uint16_t index, ICommandCallback* pCallback)
	{		
		auto lambda = [command, index, pCallback](ICommandProcessor* pProcessor) { pProcessor->SelectAndOperate(command, index, pCallback); };
		auto erasure = CreateAction(lambda);
		auto enque = [erasure, this, pCallback](){ this->Enque(erasure, pCallback); };
		pExecutor->PostLambda(enque);
	}

	template <class T>
	void DirectOperateT(T command, uint16_t index, ICommandCallback* pCallback)
	{		
		auto lambda = [command, index, pCallback](ICommandProcessor* pProcessor) { pProcessor->DirectOperate(command, index, pCallback); };
		auto erasure = CreateAction(lambda);
		auto enque = [erasure, this, pCallback](){ this->Enque(erasure, pCallback); };
		pExecutor->PostLambda(enque);
	}
};

}

#endif

