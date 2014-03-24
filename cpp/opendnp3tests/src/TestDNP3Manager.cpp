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

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/IChannel.h>

#include <opendnp3/outstation/IOutstation.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>

#include <opendnp3/master/IMaster.h>
#include <opendnp3/master/ISOEHandler.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include <asiopal/UTCTimeSource.h>
#include <asiopal/Mutex.h>
#include <asiopal/ShutdownHandler.h>

#include <openpal/LogRoot.h>

#include <thread>

using namespace opendnp3;
using namespace asiopal;
using namespace openpal;

#define SUITE(name) "DNP3ManagerTestSuite - " name

const size_t ITERATIONS = 100;

TEST_CASE(SUITE("ConstructionDestruction"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{

		EventLog log;
		LogRoot root(&log, levels::ALL);
		IOServiceThreadPool pool(root.GetLogger("pool"), std::thread::hardware_concurrency());

		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(root.GetLogger("client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(&root, "clientChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(root.GetLogger("server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(&root, "serverChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());
		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig(DatabaseTemplate()));

		pMaster->Enable();
		pOutstation->Enable();		
	}
}

TEST_CASE(SUITE("ManualStackShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		EventLog log;		
		LogRoot root(&log, levels::ALL);
		IOServiceThreadPool pool(root.GetLogger("pool"), std::thread::hardware_concurrency());

		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(root.GetLogger("client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(&root, "clientChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(root.GetLogger("server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(&root, "serverChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig(DatabaseTemplate()));
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
		LogRoot root(&log, levels::ALL);
		IOServiceThreadPool pool(root.GetLogger("pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(root.GetLogger("client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(&root, "clientChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(root.GetLogger("server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(&root, "serverChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig(DatabaseTemplate()));
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
		LogRoot root(&log, levels::ALL);				
		IOServiceThreadPool pool(root.GetLogger("pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(root.GetLogger("client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pChannel = mgr.CreateChannel(&root, "clientChannel", TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		pChannel->BeginShutdown();

	}
}




