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

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/IOutstationApplication.h>
#include <opendnp3/outstation/OutstationContext.h>

#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/ILinkListener.h>
#include <opendnp3/transport/TransportStack.h>

#include <asiopal/ASIOExecutor.h>

#include "asiodnp3/IStackLifecycle.h"
#include "asiodnp3/IOutstation.h"
#include "asiodnp3/ILinkBind.h"

namespace asiodnp3
{

/// templated base class so that things can be shared between vanilla and SA interfaces
template <class Interface>
class OutstationStackBase : public Interface, public ILinkBind
{
public:

	OutstationStackBase(
	    std::unique_ptr<openpal::LogRoot> root,
	    openpal::IExecutor& executor,
	    opendnp3::ILinkListener& listener,
	    const opendnp3::OutstationStackConfig& config,
	    IStackLifecycle& lifecycle
	) :
		root(std::move(root)),
		pLifecycle(&lifecycle),
		stack(this->root->logger, executor, listener, config.outstation.params.maxRxFragSize, &statistics, config.link),
		pContext(nullptr)
	{}

	// ------- implement IOutstation -------

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final
	{
		auto set = [this, filters]()
		{
			this->root->SetFilters(filters);
		};

		pLifecycle->GetExecutor().BlockFor(set);
	}

	virtual opendnp3::DatabaseConfigView GetConfigView() override final
	{
		return this->pContext->GetConfigView();
	}

	virtual void SetRestartIIN() override final
	{
		// this doesn't need to be synchronous, just post it
		auto lambda = [this]()
		{
			this->pContext->SetRestartIIN();
		};
		pLifecycle->GetExecutor().strand.post(lambda);
	}

	virtual bool Enable() override final
	{
		return pLifecycle->EnableRoute(&stack.link);
	}

	virtual bool Disable() override final
	{
		return pLifecycle->DisableRoute(&stack.link);
	}

	virtual void Shutdown() override final
	{
		pLifecycle->Shutdown(&stack.link, this);
	}

	virtual opendnp3::StackStatistics GetStackStatistics() override final
	{
		auto get = [this]()
		{
			return statistics;
		};
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::StackStatistics>(get);
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

private:

	virtual opendnp3::IDatabase& GetDatabase() override final
	{
		return this->pContext->GetDatabase();
	}

	virtual openpal::IExecutor& GetExecutor() override final
	{
		return pLifecycle->GetExecutor();
	}

	virtual void CheckForUpdates() override final
	{
		this->pContext->CheckForTaskStart();
	}

protected:

	void SetContext(opendnp3::OContext& context)
	{
		this->stack.transport.SetAppLayer(context);
		this->pContext = &context;
	}

	std::unique_ptr<openpal::LogRoot> root;
	opendnp3::StackStatistics statistics;
	IStackLifecycle* pLifecycle;
	opendnp3::TransportStack stack;

private:

	opendnp3::OContext* pContext;
};

}

#endif

