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
#include <asiopal/StrandGetters.h>

#include "MasterStackImpl.h"
#include "OutstationStackImpl.h"

#include <opendnp3/LogLevels.h>
#include <openpal/logging/LogMacros.h>

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
	auto lambda = [this, callback]()
	{
		this->callbacks.push_back(callback);
		callback(channelState);
	};
	pExecutor->strand.post(lambda);
}

// comes from the outside, so we need to synchronize
void DNP3Channel::BeginShutdown()
{
	auto lambda = [this]() { this->InitiateShutdown(); };
	pExecutor->PostLambda(lambda);
}

LinkChannelStatistics DNP3Channel::GetChannelStatistics()
{	
	auto getter = [this]() { return statistics; };
	return asiopal::SynchronouslyGet<LinkChannelStatistics>(pExecutor->strand, getter);	
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

IMaster* DNP3Channel::AddMaster(char const* id, ISOEHandler* pPublisher, IUTCTimeSource* pTimeSource, const MasterStackConfig& config)
{
	auto add = [this, id, pPublisher, pTimeSource, config]() 
	{ 
		return this->_AddMaster(id, pPublisher, pTimeSource, config); 
	};

	return  asiopal::SynchronouslyGet<IMaster*>(pExecutor->strand, add);
}

IOutstation* DNP3Channel::AddOutstation(char const* id, ICommandHandler* pCmdHandler, ITimeWriteHandler* pTimeWriteHandler, const OutstationStackConfig& config)
{
	auto add = [this, id, pCmdHandler, pTimeWriteHandler, config]() 
	{ 
		return this->_AddOutstation(id, pCmdHandler, pTimeWriteHandler, config);
	};
	return asiopal::SynchronouslyGet<IOutstation*>(pExecutor->strand, add);
}

IMaster* DNP3Channel::_AddMaster(char const* id,
	opendnp3::ISOEHandler* pPublisher,
	openpal::IUTCTimeSource* pTimeSource,
	const opendnp3::MasterStackConfig& config)
{
	LinkRoute route(config.link.RemoteAddr, config.link.LocalAddr);
	if (router.IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %i -> %i", route.remote, route.local);
		return nullptr;
	}
	else
	{
		StackActionHandler handler(&router, *pExecutor, this);
		auto pMaster = new MasterStackImpl(*pLogRoot, *pExecutor, pPublisher, pTimeSource, config, handler);
		pMaster->SetLinkRouter(&router);
		stacks.insert(pMaster);
		router.AddContext(pMaster->GetLinkContext(), route);
		return pMaster;
	}
}

IOutstation* DNP3Channel::_AddOutstation(char const* id,
	opendnp3::ICommandHandler* pCmdHandler,
	opendnp3::ITimeWriteHandler* pTimeWriteHandler,
	const opendnp3::OutstationStackConfig& config)
{
	LinkRoute route(config.link.RemoteAddr, config.link.LocalAddr);
	if (router.IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %i -> %i", route.remote, route.local);
		return nullptr;
	}
	else
	{
		StackActionHandler handler(&router, *pExecutor, this);
		auto pOutstation = new OutstationStackImpl(*pLogRoot, *pExecutor, *pTimeWriteHandler, *pCmdHandler, config, handler);
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
