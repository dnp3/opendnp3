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
#ifndef __COMMAND_TASK_H_
#define __COMMAND_TASK_H_

#include "MasterTaskBase.h"

#include "CommandResponse.h"
#include <openpal/Logger.h>
#include <openpal/Sequence.h>

#include "opendnp3/gen/FunctionCode.h"
#include "opendnp3/master/ICommandCallback.h"
#include "CommandSequence.h"

#include <openpal/Configure.h>
#include <assert.h>

namespace opendnp3
{

// Base class with machinery for performing command operations
class CommandTask : public MasterTaskBase
{
	// immutable sequences of function codes
	const static openpal::Sequence<FunctionCode> Operate;
	const static openpal::Sequence<FunctionCode> DirectOperate;
	const static openpal::Sequence<FunctionCode> SelectAndOperate;

public:

	CommandTask(openpal::Logger);

	void ConfigureSBO(const ControlRelayOutputBlock& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureSBO(const AnalogOutputInt16& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureSBO(const AnalogOutputInt32& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureSBO(const AnalogOutputFloat32& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureSBO(const AnalogOutputDouble64& command, uint32_t index,  ICommandCallback* pCallback);

	void ConfigureDO(const ControlRelayOutputBlock& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureDO(const AnalogOutputInt16& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureDO(const AnalogOutputInt32& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureDO(const AnalogOutputFloat32& command, uint32_t index,  ICommandCallback* pCallback);
	void ConfigureDO(const AnalogOutputDouble64& command, uint32_t index,  ICommandCallback* pCallback);

	void ConfigureRequest(APDURequest& request);

	char const* Name() const;

protected:

	// override from base class
	void OnFailure();

private:


	template <class T>
	void Configure(CommandSequence<T>& sequence, const T& value, uint32_t index, const openpal::Sequence<FunctionCode>* apSequence, ICommandCallback* pCallback)
	{
		assert(apSequence != nullptr);
		mpFunctionSequence = apSequence;
		this->mpCallback = pCallback;
		sequence.Configure(value, index);
		mpActiveSequence = &sequence;
	}

	void Callback(const CommandResponse& cr);

	ICommandCallback* mpCallback;
	const openpal::Sequence<FunctionCode>* mpFunctionSequence;

	ICommandSequence* mpActiveSequence;

	CommandSequence<ControlRelayOutputBlock> crobSeq;
	CommandSequence<AnalogOutputInt32> analogInt32Seq;
	CommandSequence<AnalogOutputInt16> analogInt16Seq;
	CommandSequence<AnalogOutputFloat32> analogFloat32Seq;
	CommandSequence<AnalogOutputDouble64> analogDouble64Seq;	

	bool _OnPartialResponse(const APDUResponseRecord&);
	TaskResult _OnFinalResponse(const APDUResponseRecord&);
};

} //ens ns

#endif
