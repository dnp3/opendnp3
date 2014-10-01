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
#include "opendnp3/master/TaskPriority.h"


#include <openpal/logging/Logger.h>
#include <openpal/Configure.h>
#include <assert.h>

#include <deque>
#include <memory>


namespace opendnp3
{

// Base class with machinery for performing command operations
class CommandTask : public IMasterTask
{
	
public:	
	
	template <class T>
	static IMasterTask* FDirectOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer, openpal::Logger logger);

	template <class T>
	static IMasterTask* FSelectAndOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer, openpal::Logger logger);

	virtual char const* Name() const override final { return "Command Task"; }

	virtual int Priority() const override final { return priority::COMMAND; }
	
	virtual bool BlocksLowerPriority() const override final { return false; }

	virtual openpal::MonotonicTimestamp ExpirationTime() const override final { return openpal::MonotonicTimestamp(0); }
	
	virtual bool IsRecurring() const override final { return false; }
	
	virtual void BuildRequest(APDURequest& request, uint8_t seq) override final;

	virtual TaskState OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now) override final;

	virtual void OnResponseTimeout(const openpal::MonotonicTimestamp& now) override final;

	virtual void OnLowerLayerClose(const openpal::MonotonicTimestamp& now) override final;

private:

	CommandTask(ICommandSequence* pSequence_, ICommandCallback& callback, openpal::Logger logger);

	TaskState OnSingleResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now);

	void LoadSelectAndOperate();
	void LoadDirectOperate();
	
	void Callback(const CommandResponse& cr);

	std::deque<FunctionCode> functionCodes;

	openpal::Logger logger;
	ICommandCallback* pCallback;	
	std::unique_ptr<ICommandSequence> pSequence;	
};

template <class T>
IMasterTask* CommandTask::FDirectOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer, openpal::Logger logger)
{
	auto pSequence = new CommandSequence<T>(logger, serializer, command, index);
	auto pCommand = new CommandTask(pSequence, callback, logger);
	pCommand->LoadDirectOperate();
	return pCommand;
}

template <class T>
IMasterTask* CommandTask::FSelectAndOperate(const T& command, uint16_t index, ICommandCallback& callback, const DNP3Serializer<T>& serializer, openpal::Logger logger)
{
	auto pSequence = new CommandSequence<T>(logger, serializer, command, index);
	auto pCommand = new CommandTask(pSequence, callback, logger);
	pCommand->LoadSelectAndOperate();
	return pCommand;
}

} //ens ns

#endif
