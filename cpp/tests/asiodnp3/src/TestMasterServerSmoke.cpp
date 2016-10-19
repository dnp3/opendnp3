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
#include "catch.hpp"

#include "asiodnp3/DNP3Manager.h"
#include "asiodnp3/DefaultMasterApplication.h"
#include "asiodnp3/DefaultListenCallbacks.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/outstation/SimpleCommandHandler.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/master/ISOEHandler.h"

#include "asiopal/UTCTimeSource.h"
#include "asiodnp3/ConsoleLogger.h"
#include "dnp3mocks/NullSOEHandler.h"

#include <thread>

using namespace opendnp3;
using namespace asiodnp3;
using namespace asiopal;
using namespace openpal;

#define SUITE(name) "MasterServerSmokeTestSuite - " name

const int ITERATIONS = 10;

struct TestChannels
{
	TestChannels(DNP3Manager& manager, std::error_code& ec) :
		client(manager.AddTCPClient("client", levels::ALL, ChannelRetry::Default(), "127.0.0.1", "", 20000, nullptr)),
		listener(manager.CreateListener("listener", levels::ALL, IPEndpoint::Localhost(20000), std::make_shared<DefaultListenCallbacks>(), ec))
	{

	}


	std::shared_ptr<IChannel> client;
	std::shared_ptr<IListener> listener;
};

struct TestComponents : public TestChannels
{
	TestComponents(DNP3Manager& manager, std::error_code& ec) :
		TestChannels(manager, ec),
		outstation(client->AddOutstation("outstation", SuccessCommandHandler::Create(), DefaultOutstationApplication::Create(), GetConfig()))
	{

	}

	void Enable()
	{
		outstation->Enable();
	}

	static OutstationStackConfig GetConfig()
	{
		OutstationStackConfig config(DatabaseSizes::Empty());
		config.outstation.params.allowUnsolicited = true;
		return config;
	}


	std::shared_ptr<IOutstation> outstation;
};



TEST_CASE(SUITE("ConstructionDestruction"))
{
	for(int i = 0; i < ITERATIONS; ++i)
	{
		DNP3Manager manager(std::thread::hardware_concurrency());
		std::error_code ec;
		TestComponents components(manager, ec);
		if (ec) throw std::logic_error(ec.message());
		components.Enable();
	}
}





