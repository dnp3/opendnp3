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
	const openpal::Logger& logger,
    const ChannelRetry& retry,
    std::shared_ptr<IChannelListener> listener,
    std::unique_ptr<asiopal::PhysicalLayerASIO> physin) :

	phys(std::move(physin)),
	logger(logger),
	pShutdownHandler(nullptr),
	router(logger, phys->executor, phys.get(), retry, listener, &statistics),
	stacks(router, phys->executor)
{
	phys->SetChannelStatistics(&statistics);

	auto onShutdown = [this]()
	{
		this->CheckForFinalShutdown();
	};
	router.SetShutdownHandler(onShutdown);
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

	shutdownHandler();
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
		return this->logger.GetFilters();
	};
	return phys->executor.ReturnBlockFor<LogFilters>(get);
}

void DNP3Channel::SetLogFilters(const openpal::LogFilters& filters)
{
	auto set = [this, filters]()
	{
		this->logger.SetFilters(filters);
	};
	phys->executor.BlockFor(set);
}

IMaster* DNP3Channel::AddMaster(char const* id, std::shared_ptr<opendnp3::ISOEHandler> SOEHandler, std::shared_ptr<opendnp3::IMasterApplication> application, const MasterStackConfig& config)
{
	auto add = [&]() -> IMaster*
	{
		auto factory = [&]() -> MasterStack*
		{
			return new MasterStack(this->logger.Detach(id), this->phys->executor, SOEHandler, application, config, stacks, router.GetTaskLock());
		};

		return this->AddStack<MasterStack>(config.link, factory);
	};

	return phys->executor.ReturnBlockFor<IMaster*>(add);
}

IOutstation* DNP3Channel::AddOutstation(char const* id, std::shared_ptr<ICommandHandler> commandHandler, std::shared_ptr<IOutstationApplication> application, const OutstationStackConfig& config)
{
	auto add = [this, id, &commandHandler, &application, config]() -> IOutstation*
	{
		auto factory = [&]()
		{
			return new OutstationStack(this->logger.Detach(id), this->phys->executor, commandHandler, application, config, stacks);
		};

		return this->AddStack<OutstationStack>(config.link, factory);
	};

	return phys->executor.ReturnBlockFor<IOutstation*>(add);
}

void DNP3Channel::SetShutdownHandler(const openpal::action_t& action)
{
	shutdownHandler = action;
}

template <class T>
T* DNP3Channel::AddStack(const opendnp3::LinkConfig& link, const std::function<T* ()>& factory)
{
	Route route(link.RemoteAddr, link.LocalAddr);
	if (router.IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::ERR, "Route already in use: %i -> %i", route.source, route.destination);
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
