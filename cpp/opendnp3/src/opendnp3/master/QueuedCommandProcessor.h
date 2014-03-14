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

#include <openpal/IExecutor.h>

#include <queue>

namespace opendnp3
{

class QueuedCommandProcessor : public ICommandProcessor
{
public:

	QueuedCommandProcessor(openpal::IExecutor* pExecutor_, ITask* pEnableTask_);

	// Implement the ICommandProcessor interface

	void SelectAndOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);
	void DirectOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback);

	// Function used to marshall calls another ICommandProcessor

	bool Dispatch(ICommandProcessor* apProcessor);

private:

	openpal::IExecutor* pExecutor;
	ITask* pEnableTask;

	std::queue<std::function<void (ICommandProcessor*)>> requestQueue;

	template <class T>
	void SelectAndOperateT(const T& command, uint16_t index, std::function<void (CommandResponse)> callback)
	{									
		pExecutor->Post(
			[this, command, index, callback]() { 
				requestQueue.push(
					[command, index, callback](ICommandProcessor * pProcessor)
					{
						pProcessor->SelectAndOperate(command, index, callback);
					}
				);
				pEnableTask->Enable(); 
			}
		);
	}

	template <class T>
	void DirectOperateT(const T& command, uint16_t index, std::function<void (CommandResponse)> callback)
	{
		
		pExecutor->Post(
			[this, command, index, callback]() {
				requestQueue.push(
					[command, index, callback](ICommandProcessor * pProcessor)
					{
						pProcessor->DirectOperate(command, index, callback);
					}
				);
				pEnableTask->Enable(); 
			}
		);
	}
};

}

#endif

