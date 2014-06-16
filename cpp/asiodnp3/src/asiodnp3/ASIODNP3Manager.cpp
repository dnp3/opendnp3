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
#include "ASIODNP3Manager.h"


#include "DNP3Manager.h"

#include <asiopal/Log.h>
#include <asiopal/IOServiceThreadPool.h>

#include <opendnp3/LogLevels.h>

#include <asiopal/PhysicalLayerSerial.h>
#include <asiopal/PhysicalLayerTCPClient.h>
#include <asiopal/PhysicalLayerTCPServer.h>

using namespace openpal;

namespace asiodnp3
{


ASIODNP3Manager::ASIODNP3Manager(uint32_t concurrency, std::function<void()> onThreadStart, std::function<void()> onThreadExit) :
	pLog(new asiopal::EventLog()),
	pThreadPool(new asiopal::IOServiceThreadPool(pLog.get(), opendnp3::flags::INFO, concurrency, onThreadStart, onThreadExit)),
	pManager(new DNP3Manager())
{

}

ASIODNP3Manager::~ASIODNP3Manager()
{

}

void ASIODNP3Manager::AddLogSubscriber(openpal::ILogBase* apLog)
{
	pLog->AddLogSubscriber(apLog);
}

void ASIODNP3Manager::Shutdown()
{
	pManager->Shutdown();
}

IChannel* ASIODNP3Manager::AddTCPClient(
	char const* id,
    uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    const std::string& host,
    uint16_t port,
    openpal::IEventHandler<opendnp3::ChannelState>* pStateHandler,
    opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pRoot = new LogRoot(pLog.get(), id, levels);
	auto pPhys = new asiopal::PhysicalLayerTCPClient(*pRoot, pThreadPool->GetIOService(), host, port);
	return pManager->CreateChannel(pRoot, minOpenRetry, maxOpenRetry, pPhys, pStateHandler, pStrategy);
}

IChannel* ASIODNP3Manager::AddTCPServer(
	char const* id,
    uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    const std::string& endpoint,
    uint16_t port,
    openpal::IEventHandler<opendnp3::ChannelState>* pStateHandler,
    opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pRoot = new LogRoot(pLog.get(), id, levels);
	auto pPhys = new asiopal::PhysicalLayerTCPServer(*pRoot, pThreadPool->GetIOService(), endpoint, port);
	return pManager->CreateChannel(pRoot, minOpenRetry, maxOpenRetry, pPhys, pStateHandler, pStrategy);
}

IChannel* ASIODNP3Manager::AddSerial(
	char const* id,
    uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    asiopal::SerialSettings aSettings,
    openpal::IEventHandler<opendnp3::ChannelState>* pStateHandler,
    opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pRoot = new LogRoot(pLog.get(), id, levels);
	auto pPhys = new asiopal::PhysicalLayerSerial(*pRoot, pThreadPool->GetIOService(), aSettings);
	return pManager->CreateChannel(pRoot, minOpenRetry, maxOpenRetry, pPhys, pStateHandler, pStrategy);
}

}

