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
#include <catch.hpp>

#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/IChannel.h>
#include <asiodnp3/IMaster.h>
#include <asiodnp3/IOutstation.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>
#include <opendnp3/master/ISOEHandler.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/PhysicalLayerTCPClient.h>
#include <asiopal/PhysicalLayerTCPServer.h>

#include <asiopal/UTCTimeSource.h>
#include <asiopal/ShutdownHandler.h>

#include <openpal/LogRoot.h>

#include <thread>

using namespace opendnp3;
using namespace asiodnp3;
using namespace asiopal;
using namespace openpal;

#define SUITE(name) "DNP3ManagerTestSuite - " name

const size_t ITERATIONS = 100;

TEST_CASE(SUITE("ConstructionDestruction"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{

		EventLog log;
		openpal::LogRoot root(&log, "id", levels::NORMAL);
		IOServiceThreadPool pool(&log, levels::NORMAL, std::thread::hardware_concurrency());

		DNP3Manager mgr;

		auto pClientRoot = new LogRoot(&log, "client", levels::NORMAL);
		auto pClientPhys = new PhysicalLayerTCPClient(*pClientRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(pClientRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerRoot = new LogRoot(&log, "server", levels::NORMAL);
		auto pServerPhys = new PhysicalLayerTCPServer(*pServerRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(pServerRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());
		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), &NullTimeWriteHandler::Inst(), OutstationStackConfig(DatabaseTemplate()));

		pMaster->Enable();
		pOutstation->Enable();
	}
}

TEST_CASE(SUITE("ManualStackShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		EventLog log;
		
		IOServiceThreadPool pool(&log, levels::NORMAL, std::thread::hardware_concurrency());

		DNP3Manager mgr;

		auto pClientRoot = new LogRoot(&log, "client", levels::NORMAL);
		auto pClientPhys = new PhysicalLayerTCPClient(*pClientRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(pClientRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerRoot = new LogRoot(&log, "server", levels::NORMAL);
		auto pServerPhys = new PhysicalLayerTCPServer(*pServerRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(pServerRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), &NullTimeWriteHandler::Inst(), OutstationStackConfig(DatabaseTemplate()));
		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());

		pOutstation->Enable();
		pMaster->Enable();

		pOutstation->BeginShutdown();
		pMaster->BeginShutdown();
	}

}

TEST_CASE(SUITE("ManualChannelShutdownWithStacks"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		EventLog log;
		IOServiceThreadPool pool(&log, levels::NORMAL, std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientRoot = new LogRoot(&log, "client", levels::NORMAL);
		auto pClientPhys = new PhysicalLayerTCPClient(*pClientRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(pClientRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerRoot = new LogRoot(&log, "server", levels::NORMAL);
		auto pServerPhys = new PhysicalLayerTCPServer(*pServerRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(pServerRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), &NullTimeWriteHandler::Inst(), OutstationStackConfig(DatabaseTemplate()));
		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());

		pMaster->Enable();
		pOutstation->Enable();

		pClient->BeginShutdown();
		pServer->BeginShutdown();
	}
}

TEST_CASE(SUITE("ManualChannelShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		EventLog log;
		IOServiceThreadPool pool(&log, levels::NORMAL, std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientRoot = new LogRoot(&log, "client", levels::NORMAL);
		auto pClientPhys = new PhysicalLayerTCPClient(*pClientRoot, pool.GetIOService(), "127.0.0.1", 20000);
		auto pChannel = mgr.CreateChannel(pClientRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		pChannel->BeginShutdown();

	}
}




