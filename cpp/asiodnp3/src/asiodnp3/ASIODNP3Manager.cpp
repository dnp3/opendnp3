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
#include <asiodnp3/ASIODNP3Manager.h>

#include <asiopal/Log.h>
#include <asiopal/IOServiceThreadPool.h>

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/LogLevels.h>

#include <asiopal/PhysicalLayerAsyncSerial.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

using namespace openpal;

namespace asiodnp3
{


ASIODNP3Manager::ASIODNP3Manager(uint32_t concurrency, std::function<void()> onThreadStart, std::function<void()> onThreadExit) :
	pLog(new asiopal::EventLog()),	
	pThreadPool(new asiopal::IOServiceThreadPool(pLog.get(), opendnp3::flags::INFO, "pool", concurrency, onThreadStart, onThreadExit)),
	pManager(new opendnp3::DNP3Manager())
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

opendnp3::IChannel* ASIODNP3Manager::AddTCPClient(
	const std::string& id,
	uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    const std::string& host,
    uint16_t port,
	openpal::IEventHandler<opendnp3::ChannelState>* pStateHandler,
    opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncTCPClient(LogConfig(pLog.get(), levels, id), pThreadPool->GetIOService(), host, port);
	return pManager->CreateChannel(id, minOpenRetry, maxOpenRetry, pPhys, pStateHandler, pStrategy);
}

opendnp3::IChannel* ASIODNP3Manager::AddTCPServer(
	const std::string& id,
	uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    const std::string& endpoint,
    uint16_t port,
	openpal::IEventHandler<opendnp3::ChannelState>* pStateHandler,
    opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncTCPServer(LogConfig(pLog.get(), levels, id), pThreadPool->GetIOService(), endpoint, port);
	return pManager->CreateChannel(id, minOpenRetry, maxOpenRetry, pPhys, pStateHandler, pStrategy);
}

opendnp3::IChannel* ASIODNP3Manager::AddSerial(
	const std::string& id,
	uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    asiopal::SerialSettings aSettings,
	openpal::IEventHandler<opendnp3::ChannelState>* pStateHandler,
    opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncSerial(LogConfig(pLog.get(), levels, id), pThreadPool->GetIOService(), aSettings);
	return pManager->CreateChannel(id, minOpenRetry, maxOpenRetry, pPhys, pStateHandler, pStrategy);
}

}

