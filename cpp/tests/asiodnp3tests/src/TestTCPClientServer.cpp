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
#include <catch.hpp>

#include "asiopal/TCPServer.h"
#include "asiopal/TCPClient.h"

#include "testlib/MockLogHandler.h"

#include "mocks/MockIO.h"
#include "mocks/MockTCPServerHandler.h"
#include "mocks/MockTCPClientHandler.h"


using namespace openpal;
using namespace asiopal;

#define SUITE(name) "TCPClientServerSuite - " name

TEST_CASE(SUITE("Client and server can connect"))
{
	auto iteration = []()
	{

		testlib::MockLogHandler log;

		auto io = MockIO::Create();
		auto shandler = std::make_shared<MockTCPServerHandler>();
		auto chandler = std::make_shared<MockTCPClientHandler>();

		auto client = TCPClient::Create(io->Executor(), "127.0.0.1", "0.0.0.0", 20000);

		std::error_code ec;
		auto server = TCPServer::Create(io->Executor(), shandler, log.root.Clone("server"), IPEndpoint::AllAdapters(20000), ec);
		REQUIRE_FALSE(ec); // now bound and listening

		REQUIRE(client->BeginConnect(chandler));

		auto condition = [&]() -> bool
		{
			return (shandler->num_accept == 1) && (chandler->num_connect == 1);
		};

		REQUIRE(io->RunUntil(condition) == 2);

		server->BeginShutdown();

		REQUIRE(io->RunUntilOutOfWork() == 1); //  the accept failure callback
	};

	// run multiple times to ensure the test is cleaning up after itself in terms of system resources
	for (int i = 0; i < 5; ++i) iteration();
}








