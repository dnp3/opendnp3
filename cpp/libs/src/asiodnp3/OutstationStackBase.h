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
#ifndef ASIODNP3_OUTSTATIONSTACKBASE_H
#define ASIODNP3_OUTSTATIONSTACKBASE_H

#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/IOutstationApplication.h>
#include <opendnp3/outstation/OutstationContext.h>

#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/ILinkListener.h>
#include <opendnp3/transport/TransportStack.h>

#include <asiopal/StrandExecutor.h>

#include "asiodnp3/IOutstation.h"
#include "asiodnp3/ILinkBind.h"
#include "asiodnp3/OutstationStackConfig.h"

namespace asiodnp3
{

/// templated base class so that things can be shared between vanilla and SA interfaces
template <class Interface>
class OutstationStackBase : public Interface, public ILinkBind
{
public:

	OutstationStackBase(
	    const openpal::Logger& logger,
		const std::shared_ptr<asiopal::StrandExecutor>& executor,
	    opendnp3::ILinkListener& listener,
	    const OutstationStackConfig& config	    
	) :
		logger(logger),
		executor(executor),		
		stack(logger, *executor.get(), listener, config.outstation.params.maxRxFragSize, &statistics, config.link),
		context(nullptr)
	{}

	// ------- implement IOutstation -------

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final
	{
		/*
		auto set = [this, filters]()
		{
			this->logger.SetFilters(filters);
		};		
		lifecycle->GetExecutor().BlockFor(set);
		*/
		// TODO
	}

	virtual void SetRestartIIN() override final
	{
		// this doesn't need to be synchronous, just post it
		auto lambda = [this]()
		{
			this->context->SetRestartIIN();
		};
		this->executor->PostToStrand(lambda);
	}

	virtual bool Enable() override final
	{
		// TODO 
		return false;
	}

	virtual bool Disable() override final
	{
		// TODO
		return false;
	}

	virtual void Shutdown() override final
	{
		// TODO
	}

	virtual opendnp3::StackStatistics GetStackStatistics() override final
	{
		auto get = [this]()
		{
			return statistics;
		};
		return this->executor->ReturnFrom<opendnp3::StackStatistics>(get);
	}

	// ------- implement ILinkBind ---------

	virtual void SetLinkRouter(opendnp3::ILinkTx& router) override final
	{
		stack.link.SetRouter(router);
	}

	virtual opendnp3::ILinkSession& GetLinkContext() override final
	{
		return stack.link;
	}

	virtual void Apply(ChangeSet& changes) override final
	{
		// C++11 lambdas don't support move semantics
		auto pchanges = std::make_shared<ChangeSet>(std::move(changes));

		auto task = [this, pchanges]()
		{

			pchanges->Apply(this->context->GetUpdateHanlder());

			this->context->CheckForTaskStart(); // force the outstation to check for updates
		};

		this->executor->PostToStrand(task);
	}

protected:

	void SetContext(opendnp3::OContext& context)
	{
		this->stack.transport.SetAppLayer(context);
		this->context = &context;
	}

	openpal::Logger logger;
	const std::shared_ptr<asiopal::StrandExecutor> executor;
	opendnp3::StackStatistics statistics;	
	opendnp3::TransportStack stack;

private:

	opendnp3::OContext* context;
};

}

#endif

