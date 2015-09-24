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
#include "asiodnp3/DNP3Manager.h"

#include "ChannelSet.h"

#include <opendnp3/LogLevels.h>

#include <asiopal/LogFanoutHandler.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/PhysicalLayerSerial.h>
#include <asiopal/PhysicalLayerTCPClient.h>
#include <asiopal/PhysicalLayerTCPServer.h>

#ifdef OPENDNP3_USE_TLS

#include <asiotls/PhysicalLayerTLSClient.h>

#endif

using namespace openpal;

namespace asiodnp3
{


DNP3Manager::DNP3Manager(
    uint32_t concurrencyHint,
    openpal::ICryptoProvider* crypto,
    std::function<void()> onThreadStart,
    std::function<void()> onThreadExit) :
	pCrypto(crypto),
	pFanoutHandler(new asiopal::LogFanoutHandler()),
	pThreadPool(new asiopal::IOServiceThreadPool(pFanoutHandler.get(), opendnp3::flags::INFO, concurrencyHint, onThreadStart, onThreadExit)),
	pChannelSet(new ChannelSet())
{

}

DNP3Manager::~DNP3Manager()
{

}

void DNP3Manager::AddLogSubscriber(openpal::ILogHandler* handler)
{
	pFanoutHandler->Subscribe(handler);
}

void DNP3Manager::Shutdown()
{
	pChannelSet->Shutdown();
}

IChannel* DNP3Manager::AddTCPClient(
    char const* id,
    uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    const std::string& host,
    const std::string& local,
    uint16_t port,
    opendnp3::IOpenDelayStrategy& strategy)
{
	auto pRoot = new LogRoot(pFanoutHandler.get(), id, levels);
	auto pPhys = new asiopal::PhysicalLayerTCPClient(*pRoot, pThreadPool->GetIOService(), host, local, port);
	return pChannelSet->CreateChannel(pRoot, pPhys->executor, minOpenRetry, maxOpenRetry, pPhys, pCrypto, strategy);
}

IChannel* DNP3Manager::AddTCPServer(
    char const* id,
    uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    const std::string& endpoint,
    uint16_t port,
    opendnp3::IOpenDelayStrategy& strategy)
{
	auto pRoot = new LogRoot(pFanoutHandler.get(), id, levels);
	auto pPhys = new asiopal::PhysicalLayerTCPServer(*pRoot, pThreadPool->GetIOService(), endpoint, port);
	return pChannelSet->CreateChannel(pRoot, pPhys->executor, minOpenRetry, maxOpenRetry, pPhys, pCrypto, strategy);
}

#ifdef OPENDNP3_USE_TLS

IChannel* DNP3Manager::AddTLSClient(
	char const* id,
	uint32_t levels,
	openpal::TimeDuration minOpenRetry,
	openpal::TimeDuration maxOpenRetry,
	const std::string& host,
	const std::string& local,
	uint16_t port,
	const std::string& peerCertFilePath,
	const std::string& privateKeyFilePath,
	opendnp3::IOpenDelayStrategy& strategy)
{
	auto pRoot = new LogRoot(pFanoutHandler.get(), id, levels);
	auto pPhys = new asiotls::PhysicalLayerTLSClient(*pRoot, pThreadPool->GetIOService(), host, local, port, peerCertFilePath, privateKeyFilePath);
	return pChannelSet->CreateChannel(pRoot, pPhys->executor, minOpenRetry, maxOpenRetry, pPhys, pCrypto, strategy);
}

#endif

IChannel* DNP3Manager::AddSerial(
    char const* id,
    uint32_t levels,
    openpal::TimeDuration minOpenRetry,
    openpal::TimeDuration maxOpenRetry,
    asiopal::SerialSettings aSettings,
    opendnp3::IOpenDelayStrategy& strategy)
{
	auto pRoot = new LogRoot(pFanoutHandler.get(), id, levels);
	auto pPhys = new asiopal::PhysicalLayerSerial(*pRoot, pThreadPool->GetIOService(), aSettings);
	return pChannelSet->CreateChannel(pRoot, pPhys->executor, minOpenRetry, maxOpenRetry, pPhys, pCrypto, strategy);
}

}

