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
#ifndef OPENDNP3_MASTERCOMMANDPROCESSOR_H
#define OPENDNP3_MASTERCOMMANDPROCESSOR_H

#include "opendnp3/master/MasterState.h"
#include "opendnp3/master/ICommandProcessor.h"

namespace opendnp3
{

class MasterCommandProcessor : public ICommandProcessor
{
	public:	

	MasterCommandProcessor(MasterState& state);
			
	// ------- command events ----------

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

	MasterState* pState;	

	// -------- helpers --------	

	template <class T>
	void SelectAndOperateT(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer);

	template <class T>
	void DirectOperateT(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer);

	template <class T>
	void ProcessSelectAndOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer);

	template <class T>
	void ProcessDirectOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer);	
};

template <class T>
void MasterCommandProcessor::SelectAndOperateT(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer)
{
	auto pCallback = &callback;
	auto action = [command, index, pCallback, serializer, this]() { this->ProcessSelectAndOperate(command, index, *pCallback, serializer); };
	pState->pExecutor->PostLambda(action);
}

template <class T>
void MasterCommandProcessor::DirectOperateT(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer)
{
	auto pCallback = &callback;
	auto action = [command, index, pCallback, serializer, this]() { this->ProcessDirectOperate(command, index, *pCallback, serializer); };
	pState->pExecutor->PostLambda(action);
}

template <class T>
void MasterCommandProcessor::ProcessSelectAndOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer)
{
	if (pState->isOnline)
	{
		pState->scheduler.Schedule(openpal::ManagedPtr<IMasterTask>::Deleted(CommandTask::FSelectAndOperate(command, index, *(pState->pApplication), callback, serializer, pState->logger)));
		pState->PostCheckForTask();
	}
	else
	{
		callback.OnComplete(CommandResponse(TaskCompletion::FAILURE_NO_COMMS));
	}	
}

template <class T>
void MasterCommandProcessor::ProcessDirectOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer)
{
	if (pState->isOnline)
	{		
		pState->scheduler.Schedule(openpal::ManagedPtr<IMasterTask>::Deleted(CommandTask::FDirectOperate(command, index, *(pState->pApplication), callback, serializer, pState->logger)));
		pState->PostCheckForTask();		
	}
	else
	{
		callback.OnComplete(CommandResponse(TaskCompletion::FAILURE_NO_COMMS));
	}
}

}

#endif
