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
#include "DNP3Channel.h"

#include <asiopal/PhysicalLayerBase.h>

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include "MasterStack.h"
#include "OutstationStack.h"

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

DNP3Channel::DNP3Channel(
    std::unique_ptr<LogRoot> root_,
    const ChannelRetry& retry,
    std::unique_ptr<asiopal::PhysicalLayerASIO> phys_) :

	phys(std::move(phys_)),
	root(std::move(root_)),
	pShutdownHandler(nullptr),
	channelState(ChannelState::CLOSED),
	router(root->logger, phys->executor, phys.get(), retry, this, &statistics),
	stacks(router, phys->executor)
{
	phys->SetChannelStatistics(&statistics);

	auto onShutdown = [this]()
	{
		this->CheckForFinalShutdown();
	};
	router.SetShutdownHandler(Action0::Bind(onShutdown));
}

void DNP3Channel::OnStateChange(ChannelState state)
{
	channelState = state;
	for (auto & cb : callbacks)
	{
		cb(state);
	}
	this->CheckForFinalShutdown();
}

void DNP3Channel::AddStateListener(const std::function<void(opendnp3::ChannelState)>& listener)
{
	auto lambda = [this, listener]()
	{
		this->callbacks.push_back(listener);
		listener(channelState);
	};
	phys->executor.strand.post(lambda);
}

// comes from the outside, so we need to synchronize
void DNP3Channel::Shutdown()
{
	stacks.ShutdownAll(); // shutdown all the stacks

	// shutdown router
	asiopal::Synchronized<bool> blocking;
	auto initiate = [this, &blocking]()
	{
		this->InitiateShutdown(blocking);
	};
	phys->executor.strand.post(initiate);
	blocking.WaitForValue();

	// With the router shutdown, wait for any remaining timers
	phys->executor.WaitForShutdown();

	shutdownHandler.Apply();
}

LinkChannelStatistics DNP3Channel::GetChannelStatistics()
{
	auto get = [this]()
	{
		return statistics;
	};
	return phys->executor.ReturnBlockFor<LinkChannelStatistics>(get);
}

void DNP3Channel::InitiateShutdown(asiopal::Synchronized<bool>& handler)
{
	this->pShutdownHandler = &handler;
	router.Shutdown();
	this->CheckForFinalShutdown();
}

void DNP3Channel::CheckForFinalShutdown()
{
	if (pShutdownHandler && (router.GetState() == ChannelState::SHUTDOWN))
	{
		pShutdownHandler->SetValue(true);
	}
}

openpal::LogFilters DNP3Channel::GetLogFilters() const
{
	auto get = [this]()
	{
		return root->GetFilters();
	};
	return phys->executor.ReturnBlockFor<LogFilters>(get);
}

void DNP3Channel::SetLogFilters(const openpal::LogFilters& filters)
{
	auto set = [this, filters]()
	{
		root->SetFilters(filters);
	};
	phys->executor.BlockFor(set);
}

IMaster* DNP3Channel::AddMaster(char const* id, ISOEHandler& SOEHandler, IMasterApplication& application, const MasterStackConfig& config)
{
	auto add = [&]() -> IMaster*
	{
		auto factory = [&]() -> MasterStack*
		{
			auto root = std::unique_ptr<openpal::LogRoot>(new openpal::LogRoot(*this->root.get(), id));
			return new MasterStack(std::move(root), this->phys->executor, SOEHandler, application, config, stacks, router.GetTaskLock());
		};

		return this->AddStack<MasterStack>(config.link, factory);
	};

	return phys->executor.ReturnBlockFor<IMaster*>(add);
}

IOutstation* DNP3Channel::AddOutstation(char const* id, ICommandHandler& commandHandler, IOutstationApplication& application, const OutstationStackConfig& config)
{
	auto add = [this, id, &commandHandler, &application, config]() -> IOutstation*
	{
		auto factory = [&]()
		{
			auto root = std::unique_ptr<openpal::LogRoot>(new openpal::LogRoot(*this->root.get(), id));
			return new OutstationStack(std::move(root), this->phys->executor, commandHandler, application, config, stacks);
		};

		return this->AddStack<OutstationStack>(config.link, factory);
	};

	return phys->executor.ReturnBlockFor<IOutstation*>(add);
}

void DNP3Channel::SetShutdownHandler(const openpal::Action0& action)
{
	shutdownHandler = action;
}

template <class T>
T* DNP3Channel::AddStack(const opendnp3::LinkConfig& link, const std::function<T* ()>& factory)
{
	Route route(link.RemoteAddr, link.LocalAddr);
	if (router.IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(root->logger, flags::ERR, "Route already in use: %i -> %i", route.source, route.destination);
		return nullptr;
	}
	else
	{
		auto pStack = factory();
		stacks.Add(pStack);
		pStack->SetLinkRouter(router);
		router.AddContext(pStack->GetLinkContext(), route);
		return pStack;
	}
}

}
