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
#ifndef OPENDNP3_COMMANDTASK_H
#define OPENDNP3_COMMANDTASK_H


#include "opendnp3/gen/FunctionCode.h"

#include "opendnp3/master/SingleResponseTask.h"
#include "opendnp3/master/CommandResponse.h"
#include "opendnp3/master/ICommandCallback.h"
#include "opendnp3/master/ICommandProcessor.h"
#include "opendnp3/master/CommandSequence.h"

#include <openpal/logging/Logger.h>
#include <openpal/Configure.h>
#include <assert.h>

#include <deque>

namespace opendnp3
{

// Base class with machinery for performing command operations
class CommandTask : public IMasterTask, public ICommandProcessor
{
	
public:	

	CommandTask(openpal::Logger* pLogger_);


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

	virtual char const* Name() const override final { return "Command Task"; }
	
	virtual void BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq) override final;

	virtual TaskStatus OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler) override final;

	virtual void OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler) override final;

	virtual void OnLowerLayerClose() override final;

private:

	TaskStatus OnSingleResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler);

	void LoadSelectAndOperate();
	void LoadDirectOperate();

	template <class T>
	void Configure(CommandSequence<T>& sequence, const T& value, uint16_t index, ICommandCallback& callback)
	{				
		this->pCallback = &callback;
		sequence.Configure(value, index);
		pActiveSequence = &sequence;
	}

	void Callback(const CommandResponse& cr);

	std::deque<FunctionCode> functionCodes;	

	openpal::Logger* pLogger;
	ICommandCallback* pCallback;	
	ICommandSequence* pActiveSequence;

	CommandSequence<ControlRelayOutputBlock> crobSeq;
	CommandSequence<AnalogOutputInt32> analogInt32Seq;
	CommandSequence<AnalogOutputInt16> analogInt16Seq;
	CommandSequence<AnalogOutputFloat32> analogFloat32Seq;
	CommandSequence<AnalogOutputDouble64> analogDouble64Seq;	
};


} //ens ns

#endif
