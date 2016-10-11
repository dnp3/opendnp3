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

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include "MasterStack.h"
#include "OutstationStack.h"

using namespace openpal;
using namespace asiopal;
using namespace opendnp3;

namespace asiodnp3
{

DNP3Channel::DNP3Channel(
    const openpal::Logger& logger,
    const std::shared_ptr<asiopal::StrandExecutor>& executor,
    std::unique_ptr<IOHandler> iohandler,
    const std::weak_ptr<asiopal::IShutdownHandler>& shutdown) :

	logger(logger),
	executor(executor),
	iohandler(std::move(iohandler)),
	resources(ResourceManager::Create()),
	shutdown(shutdown)
{

}

// comes from the outside, so we need to synchronize
void DNP3Channel::Shutdown()
{
	if (resources) // have we been shutdown yet?
	{
		if (auto sd = this->shutdown.lock())
		{
			sd->OnShutdown(this->shared_from_this());
		}

		// shutdown the IO handler
		this->iohandler->Shutdown();

		// shutdown any remaining channels
		this->resources->Shutdown();
		this->resources.reset();
	}
}

LinkChannelStatistics DNP3Channel::GetChannelStatistics()
{
	auto get = [this]()
	{
		return this->iohandler->Statistics();
	};
	return this->executor->ReturnFrom<LinkChannelStatistics>(get);
}

openpal::LogFilters DNP3Channel::GetLogFilters() const
{
	auto get = [this]()
	{
		return this->logger.GetFilters();
	};
	return this->executor->ReturnFrom<LogFilters>(get);
}

void DNP3Channel::SetLogFilters(const openpal::LogFilters& filters)
{
	auto set = [self = this->shared_from_this(), filters]()
	{
		self->logger.SetFilters(filters);
	};
	this->executor->PostToStrand(set);
}

IMaster* DNP3Channel::AddMaster(const std::string& id, std::shared_ptr<opendnp3::ISOEHandler> SOEHandler, std::shared_ptr<opendnp3::IMasterApplication> application, const MasterStackConfig& config)
{
	throw std::logic_error("not implemented");
}

IOutstation* DNP3Channel::AddOutstation(const std::string& id, std::shared_ptr<ICommandHandler> commandHandler, std::shared_ptr<IOutstationApplication> application, const OutstationStackConfig& config)
{
	throw std::logic_error("not implemented");
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
