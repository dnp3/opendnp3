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
#include "DNP3Channel.h"

#include <asiopal/PhysicalLayerBase.h>

#include "MasterStackImpl.h"
#include "OutstationStackImpl.h"

#include <openpal/logging/LogMacros.h>

#include <opendnp3/LogLevels.h>

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

DNP3Channel::DNP3Channel(
	LogRoot* pLogRoot_,
	asiopal::ASIOExecutor& executor,	
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    IOpenDelayStrategy& strategy,
    openpal::IPhysicalLayer* pPhys_,
	openpal::ICryptoProvider* pCrypto_) :
		
		pPhys(pPhys_),
		pCrypto(pCrypto_),
		pLogRoot(pLogRoot_),
		pExecutor(&executor),		
		logger(pLogRoot->GetLogger()),		
		pShutdownHandler(nullptr),
		channelState(ChannelState::CLOSED),
		router(*pLogRoot, executor, pPhys.get(), minOpenRetry, maxOpenRetry, this, strategy, &statistics),
		stacks(router, executor)
{
	pPhys->SetChannelStatistics(&statistics);

	auto onShutdown = [this]() { this->CheckForFinalShutdown(); };
	router.SetShutdownHandler(Action0::Bind(onShutdown));
}

void DNP3Channel::OnStateChange(ChannelState state)
{
	channelState = state;
	for (auto& cb : callbacks)
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
	pExecutor->strand.post(lambda);
}

// comes from the outside, so we need to synchronize
void DNP3Channel::Shutdown()
{
	stacks.ShutdownAll(); // shutdown all the stacks

	// shutdown router
	asiopal::Synchronized<bool> blocking;
	auto initiate = [this, &blocking]() { this->InitiateShutdown(blocking); };
	pExecutor->strand.post(initiate);
	blocking.WaitForValue();

	// With the router shutdown, wait for any remaining timers
	pExecutor->WaitForShutdown();

	shutdownHandler.Apply();
}

LinkChannelStatistics DNP3Channel::GetChannelStatistics()
{	
	auto get = [this]() { return statistics; };
	return pExecutor->ReturnBlockFor<LinkChannelStatistics>(get);
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
	auto get = [this](){ return pLogRoot->GetFilters(); };
	return pExecutor->ReturnBlockFor<LogFilters>(get);
}

void DNP3Channel::SetLogFilters(const openpal::LogFilters& filters)
{	
	auto set = [this, filters]() { this->pLogRoot->SetFilters(filters); };
	pExecutor->BlockFor(set);	
}

IMaster* DNP3Channel::AddMaster(char const* id, ISOEHandler& SOEHandler, IMasterApplication& application, const MasterStackConfig& config)
{
	auto add = [&]() -> IMaster*
	{
		auto factory = [&]()
		{
			return new MasterStackImpl(id, *pLogRoot, *pExecutor, SOEHandler, application, config, stacks, taskLock);
		};

		return this->AddStack<MasterStackImpl>(config.link, factory);
	};

	return pExecutor->ReturnBlockFor<IMaster*>(add);	
}

IMaster* DNP3Channel::AddMaster(	char const* id,
									opendnp3::ISOEHandler& SOEHandler,
									opendnp3::IMasterApplication& application,
									const secauth::MasterAuthStackConfig& config,
									secauth::IMasterUserDatabase& userDB)
{	
	if (!pCrypto)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Manager was not initialized with a crypto provider");
		return nullptr;
	}


	auto add = [&]() -> IMaster*
	{
		auto factory = [&]()
		{
			return new MasterStackImpl(id, *pLogRoot, *pExecutor, SOEHandler, application, config, stacks, taskLock, userDB, *pCrypto);			
		};

		return this->AddStack<MasterStackImpl>(config.link, factory);
	};
	
	return pExecutor->ReturnBlockFor<IMaster*>(add);
}

IOutstation* DNP3Channel::AddOutstation(char const* id, ICommandHandler& commandHandler, IOutstationApplication& application, const OutstationStackConfig& config)
{
	auto add = [this, id, &commandHandler, &application, config]() -> IOutstation*
	{ 				
		auto factory = [&]()
		{
			return new OutstationStackImpl(id, *pLogRoot, *pExecutor, commandHandler, application, config, stacks);
		};

		return this->AddStack<OutstationStackImpl>(config.link, factory);
	};

	return pExecutor->ReturnBlockFor<IOutstation*>(add);
}

IOutstation* DNP3Channel::AddOutstation(char const* id,
	opendnp3::ICommandHandler& commandHandler,
	secauth::IOutstationApplicationSA& application,
	const secauth::OutstationAuthStackConfig& config,	
	openpal::IUTCTimeSource& timeSource)
{
	if (!pCrypto)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Manager was not initialized with a crypto provider");
		return nullptr;
	}

	auto add = [&]() -> IOutstation*
	{
		auto factory = [&]()
		{
			return new OutstationStackImpl(id, *pLogRoot, *pExecutor, commandHandler, application, config, stacks, timeSource, *pCrypto);
		};

		return this->AddStack<OutstationStackImpl>(config.link, factory);
	};

	return pExecutor->ReturnBlockFor<IOutstation*>(add);
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
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %i -> %i", route.source, route.destination);
		return nullptr;
	}
	else
	{
		auto pStack = factory();
		stacks.Add(pStack);
		pStack->SetLinkRouter(router);
		router.AddContext(&pStack->GetLinkContext(), route);
		return pStack;
	}
}

}
