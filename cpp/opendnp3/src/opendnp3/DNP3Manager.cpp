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
#include "DNP3Manager.h"

#include "DNP3Channel.h"

using namespace openpal;

namespace opendnp3
{

DNP3Manager::DNP3Manager(openpal::IMutex* pMutex_) : 
	pMutex(pMutex_),
	isShuttingDown(false),
	pShutdownHandler(nullptr)
{

}

void DNP3Manager::BeginShutdown(openpal::IShutdownHandler* pHandler)
{	
	if (InitiateShutdown(pHandler))
	{
		// no more channels, so call the handler ourselves
		pHandler->OnShutdown();
	}
}

IChannel* DNP3Manager::CreateChannel(
    openpal::Logger aLogger,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    openpal::IPhysicalLayerAsync* apPhys,
	IEventHandler<ChannelState>* pStateHandler,
    IOpenDelayStrategy* pOpenStrategy)
{
	CriticalSection cs(pMutex);
	auto pChannel = new DNP3Channel(aLogger, minOpenRetry, maxOpenRetry, pOpenStrategy, apPhys, pMutex, this, pStateHandler);
	channels.insert(pChannel);
	return pChannel;
}

void DNP3Manager::OnShutdown(DNP3Channel* pChannel)
{
	if (OnChannelShutdown(pChannel) && pShutdownHandler)
	{
		pShutdownHandler->OnShutdown();
	}
}

bool DNP3Manager::InitiateShutdown(openpal::IShutdownHandler* pHandler)
{
	CriticalSection cs(pMutex);
	assert(!isShuttingDown);
	isShuttingDown = true;
	pShutdownHandler = pHandler;
	for (auto pChannel : channels)
	{
		pChannel->Shutdown();
	}
	return channels.empty();
}

bool DNP3Manager::OnChannelShutdown(DNP3Channel* pChannel)
{
	CriticalSection cs(pMutex);
	channels.erase(pChannel);
	delete pChannel;
	return (isShuttingDown && channels.empty());
}

}


