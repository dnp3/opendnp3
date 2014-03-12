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
#include "QueuedCommandProcessor.h"

using namespace openpal;

namespace opendnp3
{

QueuedCommandProcessor::QueuedCommandProcessor(openpal::IExecutor* pExecutor_, ITask* pEnableTask_) :
	pExecutor(pExecutor_),
	pEnableTask(pEnableTask_)
{

}

void QueuedCommandProcessor::SelectAndOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{	
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{	
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{	
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{	
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{	
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void (CommandResponse)> aCallback)
{	
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

bool QueuedCommandProcessor::Dispatch(ICommandProcessor* apProcessor)
{	
	//Transaction tx(pExecutor); TODO!!
	if (requestQueue.empty())
	{
		return false;
	}
	else
	{
		requestQueue.front()(apProcessor);
		requestQueue.pop();
		return true;
	}
}

}

