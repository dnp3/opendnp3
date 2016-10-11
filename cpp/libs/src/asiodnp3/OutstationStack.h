/*
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
#ifndef ASIODNP3_OUTSTATIONSTACK_H
#define ASIODNP3_OUTSTATIONSTACK_H

#include "asiodnp3/IOutstation.h"

#include "asiopal/StrandExecutor.h"
#include "opendnp3/outstation/OutstationContext.h"
#include "opendnp3/transport/TransportStack.h"
#include "asiodnp3/OutstationStackConfig.h"
#include "asiodnp3/StackBase.h"
#include "asiodnp3/IOHandler.h"

namespace asiodnp3
{

/**
* A stack object for an outstation
*/
class OutstationStack final : public IOutstation, public std::enable_shared_from_this<OutstationStack>
{
public:

	OutstationStack(
	    const openpal::Logger& logger,
	    const std::shared_ptr<asiopal::StrandExecutor>& executor,
	    const std::shared_ptr<opendnp3::ICommandHandler>& commandHandler,
	    const std::shared_ptr<opendnp3::IOutstationApplication>& application,
		const std::shared_ptr<IOHandler>& iohandler,
		const std::weak_ptr<asiopal::IShutdownHandler>& shutdown,
	    const OutstationStackConfig& config);

	static std::shared_ptr<OutstationStack> Create(
	    const openpal::Logger& logger,
	    const std::shared_ptr<asiopal::StrandExecutor>& executor,
	    const std::shared_ptr<opendnp3::ICommandHandler>& commandHandler,
	    const std::shared_ptr<opendnp3::IOutstationApplication>& application,
		const std::shared_ptr<IOHandler>& iohandler,
		const std::weak_ptr<asiopal::IShutdownHandler>& shutdown,
	    const OutstationStackConfig& config
	)
	{
		return std::make_shared<OutstationStack>(logger, executor, commandHandler, application, iohandler, shutdown, config);		
	}

	opendnp3::ILinkSession& GetLink()
	{
		return stack->tstack.link;
	}

	// --------- Implement IStack ---------

	virtual bool Enable() override
	{
		return stack->Enable();
	}

	virtual bool Disable() override
	{
		return stack->Disable();
	}

	virtual void Shutdown() override
	{
		stack->Shutdown(this->shared_from_this());
	}
	
	virtual opendnp3::StackStatistics GetStackStatistics() override
	{
		return stack->GetStackStatistics();
	}

	// --------- Implement IOutstation ---------


	virtual void SetLogFilters(const openpal::LogFilters& filters) override;
	
	virtual void SetRestartIIN() override;
	
	virtual void Apply(ChangeSet& changes) override;

private:

	const std::shared_ptr<StackBase> stack;
	const std::shared_ptr<opendnp3::ICommandHandler> commandHandler;
	const std::shared_ptr<opendnp3::IOutstationApplication> application;	
	opendnp3::OContext ocontext;
};

}

#endif

