/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <asiopal/UTCTimeSource.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/DefaultMasterApplication.h>
#include <asiodnp3/PrintingChannelListener.h>
#include <asiodnp3/PrintingCommandCallback.h>
#include <asiodnp3/PrintingSOEHandler.h>

#include <catch.hpp>

#include <thread>

using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

const uint32_t FILTERS = levels::NOTHING;
const uint32_t NUM_THREADS = 1;

void start_outstation(DNP3Manager& manager)
{
    const auto channel
        = manager.AddTCPServer("server", FILTERS, ServerAcceptMode::CloseNew, "127.0.0.1", 20000, nullptr);
    const auto outstation
        = channel->AddOutstation("outstation", SuccessCommandHandler::Create(), DefaultOutstationApplication::Create(),
                                 OutstationStackConfig(DatabaseSizes::AllTypes(0)));
    outstation->Enable();
}

void start_master(DNP3Manager& manager)
{
    const auto channel
        = manager.AddTCPClient("client", FILTERS, ChannelRetry(TimeDuration::Seconds(0), TimeDuration::Seconds(0)),
                               "127.0.0.1", "127.0.0.1", 20000, nullptr);
    const auto master = channel->AddMaster("master", PrintingSOEHandler::Create(),
                                           asiodnp3::DefaultMasterApplication::Create(), MasterStackConfig());
    const auto scan = master->AddClassScan(ClassField::AllClasses(), TimeDuration::Milliseconds(1));
    master->Enable();
}

TEST_CASE("TestDeadlock")
{
    for (int i = 0; i < 1000; ++i)
    {
        if (i % 100 == 0)
        {
            INFO("start iteration: " << i);
        }

        DNP3Manager manager2(NUM_THREADS);
        DNP3Manager manager1(NUM_THREADS);

        const bool is_even = (i % 2) == 0;

        start_outstation(is_even ? manager1 : manager2);
        start_master(is_even ? manager2 : manager1);

        // give the two sides sufficient time to connect
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        // shutdown manager2 followed by manager1
    }
}

// See issue #216
TEST_CASE("TestDeadlock2")
{
    const uint32_t FILTERS = levels::NOTHING;

	for(auto i = 0; i < 1000; ++i)
	{
		auto manager2 = std::make_unique<DNP3Manager>(3, ConsoleLogger::Create());
		auto channel2 = manager2->AddTCPServer("server", FILTERS, ServerAcceptMode::CloseNew, "127.0.0.1", 20000, PrintingChannelListener::Create());
		OutstationStackConfig config(DatabaseSizes::AllTypes(1));
		config.link.LocalAddr = 10;
		config.link.RemoteAddr = 1;
		auto outstation = channel2->AddOutstation("outstation", SuccessCommandHandler::Create(), DefaultOutstationApplication::Create(), config);
		outstation->Enable();

		auto manager = std::make_unique<DNP3Manager>(3, ConsoleLogger::Create());
		auto channel = manager->AddTCPClient("tcpclient", FILTERS, ChannelRetry(TimeDuration::Seconds(0), TimeDuration::Seconds(0)),
								"127.0.0.1", "127.0.0.1", 20000, PrintingChannelListener::Create());
		MasterStackConfig stackConfig;
		stackConfig.link.LocalAddr = 1;
		stackConfig.link.RemoteAddr = 10;
		auto master = channel->AddMaster("master",PrintingSOEHandler::Create(),asiodnp3::DefaultMasterApplication::Create(),stackConfig);
		auto integrityScan = master->AddClassScan(ClassField::AllClasses(), TimeDuration::Milliseconds(1));
		master->Enable();

		std::this_thread::sleep_for(std::chrono::milliseconds(2));

		integrityScan.reset();
		master.reset();
		channel.reset();

		outstation.reset();
		channel2.reset();

		manager2.reset();
		manager.reset();
	}
}
