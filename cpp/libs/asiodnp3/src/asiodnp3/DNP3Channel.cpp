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

#include <opendnp3/LogLevels.h>
#include <openpal/logging/LogMacros.h>

#include <future>

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

DNP3Channel::DNP3Channel(
	LogRoot* pLogRoot_,
	asiopal::ASIOExecutor& executor,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    IOpenDelayStrategy* pStrategy,
    openpal::IPhysicalLayer* pPhys_,
    ITypedShutdownHandler<DNP3Channel*>* pShutdownHandler_) :
		
		pPhys(pPhys_),
		pLogRoot(pLogRoot_),
		pExecutor(&executor),
		logger(pLogRoot->GetLogger()),
		state(State::READY),
		pShutdownHandler(pShutdownHandler_),
		channelState(ChannelState::CLOSED),
		router(*pLogRoot, executor, pPhys.get(), minOpenRetry, maxOpenRetry, this, pStrategy, &statistics)	
{
	pPhys->SetChannelStatistics(&statistics);

	auto onShutdown = [this]() { this->CheckForFinalShutdown(); };
	router.SetShutdownHandler(Runnable::Bind(onShutdown));
}

void DNP3Channel::OnStateChange(ChannelState state)
{
	channelState = state;
	for (auto& cb : callbacks) cb(state);
}

void DNP3Channel::AddStateChangeCallback(const StateChangeCallback& callback)
{
	auto lambda = [this, callback]() {
		this->callbacks.push_back(callback);
		callback(channelState);
	};
}

// comes from the outside, so we need to synchronize
void DNP3Channel::BeginShutdown()
{
	auto lambda = [this]() { this->InitiateShutdown(); };
	pExecutor->PostLambda(lambda);
}

LinkChannelStatistics DNP3Channel::GetChannelStatistics()
{
	std::promise<LinkChannelStatistics> p;
	auto lambda = [&]() { p.set_value(statistics); };
	pExecutor->PostLambda(lambda);
	return p.get_future().get();	
}

// can only run on the executor itself
void DNP3Channel::InitiateShutdown()
{
	if (state == State::READY)
	{
		state = State::SHUTTING_DOWN;		

		for (auto pStack : stacks)
		{
			pStack->BeginShutdown();
		}		
		
		router.Shutdown();
	}
}

void DNP3Channel::CheckForFinalShutdown()
{
	// The router is offline. The stacks are shutdown
	if ((state == State::SHUTTING_DOWN) && (router.GetState() == ChannelState::SHUTDOWN) && stacks.empty())
	{
		state = State::SHUTDOWN;

		auto lambda = [this]()
		{
			this->pShutdownHandler->OnShutdown(this);
		};

		pExecutor->Start(TimeDuration::Zero(), Runnable::Bind(lambda));
	}
}

openpal::IExecutor* DNP3Channel::GetExecutor()
{
	return pExecutor;
}

openpal::LogFilters DNP3Channel::GetLogFilters() const
{
	return pLogRoot->GetFilters();
}

void DNP3Channel::SetLogFilters(const openpal::LogFilters& filters)
{	
	auto lambda = [this, filters]() { this->pLogRoot->SetFilters(filters); };
	pExecutor->PostLambda(lambda);
}

IMaster* DNP3Channel::AddMaster(char const* id, ISOEHandler* apPublisher, IUTCTimeSource* apTimeSource, const MasterStackConfig& config)
{
	LinkRoute route(config.link.RemoteAddr, config.link.LocalAddr);
	asiopal::ExecutorPause p(*pExecutor);
	if(router.IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %i -> %i", route.remote, route.local);
		return nullptr;
	}
	else
	{
		StackActionHandler handler(&router, *pExecutor, this);
		auto pMaster = new MasterStackImpl(*pLogRoot, *pExecutor, apPublisher, apTimeSource, config, handler);
		pMaster->SetLinkRouter(&router);
		stacks.insert(pMaster);
		router.AddContext(pMaster->GetLinkContext(), route);
		return pMaster;				
	}
}

IOutstation* DNP3Channel::AddOutstation(char const* id, ICommandHandler* apCmdHandler, ITimeWriteHandler* apTimeWriteHandler, const OutstationStackConfig& arCfg)
{
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	asiopal::ExecutorPause p(*pExecutor);
	if(router.IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %i -> %i", route.remote, route.local);
		return nullptr;
	}
	else
	{		
		StackActionHandler handler(&router, *pExecutor, this);
		auto pOutstation = new OutstationStackImpl(*pLogRoot, *pExecutor, *apTimeWriteHandler, *apCmdHandler, arCfg, handler);
		pOutstation->SetLinkRouter(&router);
		stacks.insert(pOutstation);
		router.AddContext(pOutstation->GetLinkContext(), route);
		return pOutstation;		
	}
}

// these always happen on the strand
void DNP3Channel::OnShutdown(IStack* pStack)
{
	stacks.erase(pStack);
	delete pStack;
	this->CheckForFinalShutdown();
}

}
