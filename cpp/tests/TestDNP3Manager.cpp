
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

#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/IChannel.h>
#include <opendnp3/IMaster.h>
#include <opendnp3/IOutstation.h>
#include <opendnp3/SimpleCommandHandler.h>
#include <opendnp3/IVtoEndpoint.h>
#include <opendnp3/LogToStdio.h>
#include <opendnp3/SimpleDataObserver.h>

#include <thread>

using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(DNP3ManagerTestSuite)

const size_t ITERATIONS = 100;

BOOST_AUTO_TEST_CASE(ConstructionDestruction)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		DNP3Manager mgr(std::thread::hardware_concurrency());

		auto pClient = mgr.AddTCPClient("client", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pServer = mgr.AddTCPServer("server", LEV_INFO, 5000, "127.0.0.1", 20000);
		pClient->AddMaster("master", LEV_INFO, NullDataObserver::Inst(), MasterStackConfig());
		pServer->AddOutstation("outstation", LEV_INFO, SuccessCommandHandler::Inst(), SlaveStackConfig());

	}
}

BOOST_AUTO_TEST_CASE(ManualStackShutdown)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		DNP3Manager mgr(std::thread::hardware_concurrency());

		auto pClient = mgr.AddTCPClient("client", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pServer = mgr.AddTCPServer("server", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pOutstation = pServer->AddOutstation("outstation", LEV_INFO, SuccessCommandHandler::Inst(), SlaveStackConfig());
		auto pMaster = pClient->AddMaster("master", LEV_INFO, NullDataObserver::Inst(), MasterStackConfig());

		pOutstation->Shutdown();
		pMaster->Shutdown();
	}

}

BOOST_AUTO_TEST_CASE(ManualChannelShutdownWithStack)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		DNP3Manager mgr(std::thread::hardware_concurrency());

		auto pChannel = mgr.AddTCPClient("client", LEV_INFO, 5000, "127.0.0.1", 20000);
		pChannel->AddMaster("master", LEV_INFO, NullDataObserver::Inst(), MasterStackConfig());
		pChannel->Shutdown();
	}
}

BOOST_AUTO_TEST_CASE(ManualChannelShutdown)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		DNP3Manager mgr(std::thread::hardware_concurrency());
		mgr.AddTCPClient("client", LEV_INFO, 5000, "127.0.0.1", 20000)->Shutdown();

	}
}

BOOST_AUTO_TEST_CASE(ConstructionDestructionWithVtoRouters)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		DNP3Manager mgr(std::thread::hardware_concurrency());

		auto pClient = mgr.AddTCPClient("client", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pServer = mgr.AddTCPServer("server", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pMaster = pClient->AddMaster("master", LEV_INFO, NullDataObserver::Inst(), MasterStackConfig());
		auto pOutstation = pServer->AddOutstation("outstation", LEV_INFO, SuccessCommandHandler::Inst(), SlaveStackConfig());

		pMaster->StartVtoRouterTCPClient("vtoclient", LEV_INFO, "127.0.0.1", 20001, VtoRouterSettings(0, true, false));
		pOutstation->StartVtoRouterTCPServer("vtoclient", LEV_INFO, "127.0.0.1", 20001, VtoRouterSettings(0, true, false));

	}
}

BOOST_AUTO_TEST_CASE(ConstructionDestructionWithVtoRoutersManualVtoShutdown)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		DNP3Manager mgr(std::thread::hardware_concurrency());

		auto pClient = mgr.AddTCPClient("client", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pServer = mgr.AddTCPServer("server", LEV_INFO, 5000, "127.0.0.1", 20000);
		auto pMaster = pClient->AddMaster("master", LEV_INFO, NullDataObserver::Inst(), MasterStackConfig());
		auto pOutstation = pServer->AddOutstation("outstation", LEV_INFO, SuccessCommandHandler::Inst(), SlaveStackConfig());

		auto pClientVto = pMaster->StartVtoRouterTCPClient("vtoclient", LEV_INFO, "127.0.0.1", 20001, VtoRouterSettings(0, true, false));
		auto pServerVto = pOutstation->StartVtoRouterTCPServer("vtoclient", LEV_INFO, "127.0.0.1", 20001, VtoRouterSettings(0, true, false));

		pClientVto->Shutdown();
		pServerVto->Shutdown();

	}
}



BOOST_AUTO_TEST_SUITE_END()


