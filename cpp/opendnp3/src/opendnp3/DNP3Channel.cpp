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

#include <openpal/IPhysicalLayerAsync.h>


#include "opendnp3/master/MasterStackImpl.h"
#include "opendnp3/outstation/OutstationStackImpl.h"


using namespace openpal;

namespace opendnp3
{

DNP3Channel::DNP3Channel(
    openpal::Logger logger,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    IOpenDelayStrategy* pStrategy,
    IPhysicalLayerAsync* pPhys_,	
	openpal::ITypedShutdownHandler<DNP3Channel*>* pShutdownHandler_,
	openpal::IEventHandler<ChannelState>* pStateHandler) :
		Loggable(logger),
		pPhys(pPhys_),		
		state(State::READY),
		pShutdownHandler(pShutdownHandler_),
		router(logger.GetSubLogger("Router"), pPhys.get(), minOpenRetry, maxOpenRetry, pStateHandler, this, pStrategy),
		group(pPhys->GetExecutor())
{

}

// comes from the outside, so we need to synchronize
void DNP3Channel::BeginShutdown()
{	
	pPhys->GetExecutor()->Post([this](){ this->InitiateShutdown(); });
}

// can only run on the executor itself
void DNP3Channel::InitiateShutdown()
{
	if (state == State::READY)
	{
		state = State::SHUTTING_DOWN;

		this->group.Shutdown();  // no more task callbacks
		
		for (auto pStack : stacks)
		{
			pStack->BeginShutdown();
		}

		router.Shutdown();
	}	
}

// called by the router when it's shutdown is complete
void DNP3Channel::OnShutdown()
{
	this->CheckForFinalShutdown();
}

void DNP3Channel::CheckForFinalShutdown()
{
	// The router is offline. The stacks are shutdown
	if ((state == State::SHUTTING_DOWN) && (router.GetState() == ChannelState::SHUTDOWN) && stacks.empty())
	{
		state = State::SHUTDOWN;

		pPhys->GetExecutor()->Start(TimeDuration::Zero(),
			[this]()
			{
				this->pShutdownHandler->OnShutdown(this);
			}
		);
	}	
}

openpal::IExecutor* DNP3Channel::GetExecutor()
{
	return pPhys->GetExecutor();
}

IMaster* DNP3Channel::AddMaster(const std::string& loggerId, LogLevel aLevel, ISOEHandler* apPublisher, IUTCTimeSource* apTimeSource, const MasterStackConfig& config)
{
	LinkRoute route(config.link.RemoteAddr, config.link.LocalAddr);
	ExecutorPause p(pPhys->GetExecutor());
	if(router.IsRouteInUse(route))
	{
		LOG_BLOCK(LogLevel::Error, "Route already in use: " << route.ToString());
		return nullptr;
	}
	else
	{		
		StackActionHandler handler(&router, pPhys->GetExecutor(), this);
		auto pMaster = new MasterStackImpl(logger, pPhys->GetExecutor(), apPublisher, apTimeSource, &group, config, handler);
		pMaster->SetLinkRouter(&router);
		stacks.insert(pMaster);
		router.AddContext(pMaster->GetLinkContext(), route);
		return pMaster;
	}
}

IOutstation* DNP3Channel::AddOutstation(const std::string& arLoggerId, LogLevel aLevel, ICommandHandler* apCmdHandler, ITimeWriteHandler* apTimeWriteHandler, const SlaveStackConfig& arCfg)
{
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	ExecutorPause p(pPhys->GetExecutor());
	if(router.IsRouteInUse(route))
	{
		LOG_BLOCK(LogLevel::Error, "Route already in use: " << route.ToString());
		return nullptr;
	}
	else
	{
		StackActionHandler handler(&router, pPhys->GetExecutor(), this);
		auto pOutstation = new OutstationStackImpl(logger, pPhys->GetExecutor(), apTimeWriteHandler, apCmdHandler, arCfg, handler);
		pOutstation->SetLinkRouter(&router);
		stacks.insert(pOutstation);
		router.AddContext(pOutstation->GetLinkContext(), route);
		return pOutstation;
	}
}

// these always happen on the strand
void DNP3Channel::OnShutdown(DNP3Stack* apStack)
{
	stacks.erase(apStack);	
	delete apStack;
	this->CheckForFinalShutdown();
}

}
