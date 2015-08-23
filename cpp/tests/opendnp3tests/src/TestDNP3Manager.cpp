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
#include <asiodnp3/DefaultMasterApplication.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/IOutstationApplication.h>
#include <opendnp3/master/ISOEHandler.h>
#include <asiodnp3/MeasUpdate.h>

#include <asiopal/UTCTimeSource.h>

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
		DNP3Manager manager(std::thread::hardware_concurrency());
		
		auto pClient = manager.AddTCPClient("client", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "127.0.0.1", "", 20000);		
		auto pServer = manager.AddTCPServer("server", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "0.0.0.0", 20000);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), OutstationStackConfig(DatabaseTemplate()));
		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Instance(), asiodnp3::DefaultMasterApplication::Instance(), MasterStackConfig());
		
		pOutstation->Enable();
		pMaster->Enable();		
	}
}

TEST_CASE(SUITE("ManualStackShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		DNP3Manager manager(std::thread::hardware_concurrency());

		auto pClient = manager.AddTCPClient("client", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "127.0.0.1", "", 20000);
		auto pServer = manager.AddTCPServer("server", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "0.0.0.0", 20000);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), OutstationStackConfig(DatabaseTemplate()));
		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Instance(), asiodnp3::DefaultMasterApplication::Instance(), MasterStackConfig());

		pOutstation->Enable();
		pMaster->Enable();

		pOutstation->Shutdown();
		pMaster->Shutdown();
	}

}

TEST_CASE(SUITE("ManualChannelShutdownWithStacks"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		DNP3Manager manager(std::thread::hardware_concurrency());

		auto pClient = manager.AddTCPClient("client", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "127.0.0.1", "127.0.0.1", 20000);
		auto pServer = manager.AddTCPServer("server", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "0.0.0.0", 20000);

		auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), OutstationStackConfig(DatabaseTemplate()));
		auto pMaster = pClient->AddMaster("master", NullSOEHandler::Instance(), asiodnp3::DefaultMasterApplication::Instance(), MasterStackConfig());

		pMaster->Enable();
		pOutstation->Enable();

		pClient->Shutdown();
		pServer->Shutdown();
	}
}

TEST_CASE(SUITE("ManualChannelShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		DNP3Manager manager(std::thread::hardware_concurrency());

		auto pClient = manager.AddTCPClient("client", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "127.0.0.1", "127.0.0.1", 20000);
		auto pServer = manager.AddTCPServer("server", levels::NORMAL, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "0.0.0.0", 20000);		

		pClient->Shutdown();
		pServer->Shutdown();
	}
}

TEST_CASE(SUITE("StackLinkStatus"))
{
	DNP3Manager manager(1);

	auto pClient = manager.AddTCPClient("client", levels::NORMAL, TimeDuration::Seconds(1), TimeDuration::Seconds(1), "127.0.0.1", "", 20000);
	auto pServer = manager.AddTCPServer("server", levels::NORMAL, TimeDuration::Seconds(1), TimeDuration::Seconds(1), "0.0.0.0", 20000);

	auto mconf = MasterStackConfig();
	mconf.master.disableUnsolOnStartup = false;
	mconf.master.unsolClassMask = opendnp3::ClassField::AllClasses();
	mconf.link.KeepAlive = openpal::TimeDuration::Milliseconds(10);
	mconf.link.Timeout = openpal::TimeDuration::Milliseconds(10);
	mconf.link.NumRetry = 0;

	auto db = DatabaseTemplate();
	db.numAnalog = 1;

	auto oconf = OutstationStackConfig(db);
	oconf.outstation.params.allowUnsolicited = true;
	oconf.outstation.params.unsolClassMask = opendnp3::ClassField::AllClasses();

	auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), oconf);
	oconf.link.LocalAddr = 3;
	auto pOutstation2 = pServer->AddOutstation("outstation2", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), oconf);
	auto pMaster = pClient->AddMaster("master", NullSOEHandler::Instance(), asiodnp3::DefaultMasterApplication::Instance(), mconf);

	opendnp3::LinkStatus cstatus;
	auto lambda = [&](opendnp3::LinkStatus lstatus) {
		cstatus = lstatus;
	};

	pMaster->AddLinkStatusListener(lambda);
	REQUIRE(cstatus == opendnp3::LinkStatus::TIMEOUT);
	
	pOutstation->Enable();
	pOutstation2->Enable();
	pMaster->Enable();
	{//trasaction scope
		asiodnp3::MeasUpdate tx(pOutstation);
		tx.Update(opendnp3::Analog(123.4), 0);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::UNRESET);
	
	pOutstation->Disable();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::TIMEOUT);

	pOutstation->Enable();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::UNRESET);
	pMaster->ScanClasses(ClassField::AllClasses());
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::UNRESET);

	pOutstation->Shutdown();
	pMaster->Shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::TIMEOUT);
}

TEST_CASE(SUITE("StackLinkStatusConfirms"))
{
	DNP3Manager manager(1);

	auto pClient = manager.AddTCPClient("client", levels::NORMAL, TimeDuration::Seconds(1), TimeDuration::Seconds(1), "127.0.0.1", "", 20000);
	auto pServer = manager.AddTCPServer("server", levels::NORMAL, TimeDuration::Seconds(1), TimeDuration::Seconds(1), "0.0.0.0", 20000);

	auto mconf = MasterStackConfig();
	mconf.master.disableUnsolOnStartup = false;
	mconf.master.unsolClassMask = opendnp3::ClassField::AllClasses();
	mconf.link.UseConfirms = true;
	mconf.link.KeepAlive = openpal::TimeDuration::Milliseconds(10);
	mconf.link.Timeout = openpal::TimeDuration::Milliseconds(10);
	mconf.link.NumRetry = 0;

	auto db = DatabaseTemplate();
	db.numAnalog = 1;

	auto oconf = OutstationStackConfig(db);
	oconf.outstation.params.allowUnsolicited = true;
	oconf.outstation.params.unsolClassMask = opendnp3::ClassField::AllClasses();
	oconf.link.UseConfirms = true;

	auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), oconf);
	oconf.link.LocalAddr = 3;
	auto pOutstation2 = pServer->AddOutstation("outstation2", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), oconf);
	auto pMaster = pClient->AddMaster("master", NullSOEHandler::Instance(), asiodnp3::DefaultMasterApplication::Instance(), mconf);

	opendnp3::LinkStatus cstatus;
	auto lambda = [&](opendnp3::LinkStatus lstatus) {
		cstatus = lstatus;
	};

	pMaster->AddLinkStatusListener(lambda);

	pOutstation->Enable();
	pOutstation2->Enable();
	pMaster->Enable();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::RESET);

	pOutstation->Disable();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::TIMEOUT);

	pOutstation->Enable();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::UNRESET);
	pMaster->ScanClasses(ClassField::AllClasses());
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::RESET);

	pOutstation->Shutdown();
	pMaster->Shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	REQUIRE(cstatus == opendnp3::LinkStatus::TIMEOUT);
}



