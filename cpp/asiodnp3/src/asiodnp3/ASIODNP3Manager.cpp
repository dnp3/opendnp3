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

#include <asiopal/PhysicalLayerAsyncSerial.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

using namespace openpal;

namespace asiodnp3
{


ASIODNP3Manager::ASIODNP3Manager(uint32_t aConcurrency, std::function<void()> aOnThreadStart, std::function<void()> aOnThreadExit) :
	mpLog(new asiopal::EventLog()),
	mpThreadPool(new asiopal::IOServiceThreadPool(Logger(mpLog.get(), LogLevel::Info, "pool"), aConcurrency, aOnThreadStart, aOnThreadExit)),
	mpManager(new opendnp3::DNP3Manager())
{

}

ASIODNP3Manager::~ASIODNP3Manager()
{

}

void ASIODNP3Manager::AddLogSubscriber(openpal::ILogBase* apLog)
{
	mpLog->AddLogSubscriber(apLog);
}

openpal::ILogBase* ASIODNP3Manager::GetLog()
{
	return mpLog.get();
}

void ASIODNP3Manager::Shutdown()
{
	mpManager->Shutdown();
}

opendnp3::IChannel* ASIODNP3Manager::AddTCPClient(
	openpal::Logger aLogger, 
	openpal::TimeDuration minOpenRetry, 
	openpal::TimeDuration maxOpenRetry, 
	const std::string& host, 
	uint16_t port,
	opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncTCPClient(aLogger, mpThreadPool->GetIOService(), host, port);
	return mpManager->CreateChannel(aLogger, minOpenRetry, maxOpenRetry, pPhys, pStrategy);
}

opendnp3::IChannel* ASIODNP3Manager::AddTCPServer(
	openpal::Logger aLogger,
	openpal::TimeDuration minOpenRetry,
	openpal::TimeDuration maxOpenRetry,
	const std::string& endpoint, 
	uint16_t port,
	opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncTCPServer(aLogger, mpThreadPool->GetIOService(), endpoint, port);
	return mpManager->CreateChannel(aLogger, minOpenRetry, maxOpenRetry, pPhys, pStrategy);
}

opendnp3::IChannel* ASIODNP3Manager::AddSerial(
	openpal::Logger aLogger,
	openpal::TimeDuration minOpenRetry,
	openpal::TimeDuration maxOpenRetry,
	asiopal::SerialSettings aSettings,
	opendnp3::IOpenDelayStrategy* pStrategy)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncSerial(aLogger, mpThreadPool->GetIOService(), aSettings);
	return mpManager->CreateChannel(aLogger, minOpenRetry, maxOpenRetry, pPhys, pStrategy);
}

}

