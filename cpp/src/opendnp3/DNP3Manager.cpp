
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include <opendnp3/DNP3Manager.h>

#include "PhysicalLayerAsyncTCPClient.h"
#include "PhysicalLayerAsyncTCPServer.h"
#ifndef OPENDNP3_NO_SERIAL
#include "PhysicalLayerAsyncSerial.h"
#endif
#include "IOServiceThreadPool.h"
#include "Log.h"
#include "DNP3Channel.h"

using namespace openpal;

namespace opendnp3
{

DNP3Manager::DNP3Manager(uint32_t aConcurrency, std::function<void()> aOnThreadStart, std::function<void()> aOnThreadExit) :
	mpLog(new EventLog()),
	mpThreadPool(new IOServiceThreadPool(Logger(mpLog.get(), LEV_INFO, "ThreadPool"),  aConcurrency, aOnThreadStart, aOnThreadExit))
{

}

DNP3Manager::~DNP3Manager()
{
	this->Shutdown();
}

void DNP3Manager::AddLogSubscriber(ILogBase* apLog)
{
	mpLog->AddLogSubscriber(apLog);
}

void DNP3Manager::Shutdown()
{
	std::set<DNP3Channel*> copy(mChannels);
for(auto pChannel: copy) pChannel->Shutdown();
}

IChannel* DNP3Manager::AddTCPClient(const std::string& arName, FilterLevel aLevel, openpal::TimeDuration aOpenRetry, const std::string& arAddr, uint16_t aPort)
{
	auto logger = Logger(mpLog.get(), aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncTCPClient(logger, mpThreadPool->GetIOService(), arAddr, aPort);
	return CreateChannel(logger, aOpenRetry, pPhys);
}

IChannel* DNP3Manager::AddTCPServer(const std::string& arName, FilterLevel aLevel,openpal::TimeDuration aOpenRetry, const std::string& arEndpoint, uint16_t aPort)
{
	auto logger = Logger(mpLog.get(), aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncTCPServer(logger, mpThreadPool->GetIOService(), arEndpoint, aPort);
	return CreateChannel(logger, aOpenRetry, pPhys);
}

#ifndef OPENDNP3_NO_SERIAL
IChannel* DNP3Manager::AddSerial(const std::string& arName, FilterLevel aLevel, openpal::TimeDuration aOpenRetry, SerialSettings aSettings)
{
	auto logger = Logger(mpLog.get(), aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncSerial(logger, mpThreadPool->GetIOService(), aSettings);
	return CreateChannel(logger, aOpenRetry, pPhys);
}
#endif

IChannel* DNP3Manager::CreateChannel(openpal::Logger& arLogger, openpal::TimeDuration aOpenRetry, IPhysicalLayerAsync* apPhys)
{
	auto pChannel = new DNP3Channel(arLogger, aOpenRetry, apPhys, [this](DNP3Channel * apChannel) {
		this->OnChannelShutdownCallback(apChannel);
	});
	mChannels.insert(pChannel);
	return pChannel;
}

void DNP3Manager::OnChannelShutdownCallback(DNP3Channel* apChannel)
{
	mChannels.erase(apChannel);
	delete apChannel;
}

}


