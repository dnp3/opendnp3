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
#include <asio.hpp>

#include <functional>

#include <asiopal/ASIOExecutor.h>

#include <openpal/logging/LogRoot.h>

#include "mocks/PhysLoopback.h"
#include "mocks/TestObjectASIO.h"
#include "mocks/PhysTestObject.h"

#include <testlib/BufferHelpers.h>
#include <testlib/RandomizedBuffer.h>

#include <iostream>

using namespace opendnp3;
using namespace boost;
using namespace openpal;
using namespace asiopal;
using namespace testlib;


#define SUITE(name) "PhysicalLayerTCPSuite - " name

TEST_CASE(SUITE("TestStateClosed"))
{
	PhysTestObject t;

	uint8_t b[100];
	WSlice buff(b, 100);
	WSlice empty;

	t.mTCPClient.BeginWrite(empty.ToRSlice());
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.mTCPClient.BeginRead(empty);
	REQUIRE(t.log.PopOneEntry(flags::ERR));

	t.mTCPClient.BeginWrite(buff.ToRSlice());
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.mTCPClient.BeginRead(buff);
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.mTCPClient.BeginClose();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
}

TEST_CASE(SUITE("ClientConnectionRejected"))
{
	PhysTestObject t;

	REQUIRE(t.mClientAdapter.GetNumOpenFailure() ==  0);

	for(size_t i = 0; i < 2; ++i)
	{
		t.mTCPClient.BeginOpen();
		REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}

TEST_CASE(SUITE("ClientConnectionCanceled"))
{
	PhysTestObject t;

	for(size_t i = 0; i < 2; ++i)
	{
		t.mTCPClient.BeginOpen();
		t.mTCPClient.BeginClose();

		REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}

TEST_CASE(SUITE("ServerAcceptCanceled"))
{
	PhysTestObject t;

	for(size_t i = 0; i < 2; ++i)
	{
		t.mTCPServer.BeginOpen();
		t.mTCPServer.BeginClose();

		REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mServerAdapter, i + 1)));
	}
}

TEST_CASE(SUITE("ConnectDisconnect"))
{
	PhysTestObject t;

	for(size_t i = 0; i < 10; ++i)
	{

		t.mTCPServer.BeginOpen();
		t.mTCPClient.BeginOpen();
		REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
		REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));

		//Check that since reads are outstanding, you only have to stop 1/2 of the connection
		if ((i % 2) == 0) t.mTCPServer.BeginClose();
		else t.mTCPClient.BeginClose();
		REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
		REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));
	}
}

TEST_CASE(SUITE("TestSendShutdown"))
{
	PhysTestObject t;

	t.mTCPServer.BeginOpen();
	t.mTCPClient.BeginOpen();
	REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
	REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));

	ByteStr bs(1024, 77); //give some interesting seed value to make sure bytes are correctly written
	t.mClientUpper.SendDown(bs.ToRSlice());

	t.mTCPClient.BeginClose();
	REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
	REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));
}

TEST_CASE(SUITE("TwoWaySend"))
{
	const size_t SEND_SIZE = 1 << 20; // 1 MB

	PhysTestObject t;

	t.mTCPServer.BeginOpen();
	t.mTCPClient.BeginOpen();
	REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
	REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));

	//both layers are now up and reading, start them both writing
	ByteStr bs(SEND_SIZE, 77); //give some interesting seed value to make sure bytes are correctly written
	t.mClientUpper.SendDown(bs.ToRSlice());
	t.mServerUpper.SendDown(bs.ToRSlice());

	REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &t.mServerUpper, SEND_SIZE)));
	REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &t.mClientUpper, SEND_SIZE)));

	REQUIRE(t.mClientUpper.BufferEquals(bs.ToRSlice()));
	REQUIRE(t.mServerUpper.BufferEquals(bs.ToRSlice()));

	t.mTCPServer.BeginClose(); //stop one side
	REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
	REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));
}

TEST_CASE(SUITE("ServerCloseWhileOpeningKillsAcceptor"))
{
	PhysTestObject t;

	REQUIRE(0 ==  t.mClientAdapter.GetNumOpenFailure());

	for(size_t i = 0; i < 5; ++i)
	{
		t.mTCPServer.BeginOpen();
		t.mTCPServer.BeginClose();

		REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mServerAdapter, i + 1)));

		// since we closed the server socket we shouldn't be able to connect now
		t.mTCPClient.BeginOpen();

		REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}

TEST_CASE(SUITE("ServerCloseAfterOpeningKillsAcceptor"))
{
	PhysTestObject t;

	REQUIRE(t.mClientAdapter.GetNumOpenFailure() ==  0);

	for(size_t i = 0; i < 5; ++i)
	{
		t.mTCPServer.BeginOpen();
		t.mTCPClient.BeginOpen();

		REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
		REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));

		t.mTCPServer.BeginClose();

		REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mServerUpper)));
		REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsOnline, &t.mClientUpper)));

		// since we closed the server socket we shouldn't be able to connect now
		t.mTCPClient.BeginOpen();

		REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}


#define MACRO_LOOPBACK_SIZE 100
#ifndef ARM
#define MACRO_LOOPBACK_ITERATIONS 100
#else
#define MACRO_LOOPBACK_ITERATIONS 10
#endif

TEST_CASE(SUITE("Loopback"))
{
	const size_t SIZE = MACRO_LOOPBACK_SIZE;
	const size_t ITERATIONS = MACRO_LOOPBACK_ITERATIONS;

	testlib::MockLogHandler log;
	LogRoot root(&log, "test", levels::NORMAL);
	TestObjectASIO test;
	PhysicalLayerTCPServer server(root.logger, test.GetService(), "127.0.0.1", 30000);
	PhysLoopback loopback(root.logger, server.executor, &server);
	loopback.Start();

	PhysicalLayerTCPClient client(root.logger, test.GetService(), "127.0.0.1", "127.0.0.1", 30000);
	LowerLayerToPhysAdapter adapter(client);
	MockUpperLayer upper;
	adapter.SetUpperLayer(upper);
	upper.SetLowerLayer(adapter);

	client.BeginOpen();
	REQUIRE(test.ProceedUntil(std::bind(&MockUpperLayer::IsOnline, &upper)));

	RandomizedBuffer rb(SIZE);

	for(size_t i = 0; i < ITERATIONS; ++i)
	{
		rb.Randomize();
		upper.SendDown(rb.ToRSlice());
		REQUIRE(test.ProceedUntil([&]()
		{
			return upper.BufferEquals(rb.ToRSlice());
		}));
		REQUIRE(test.ProceedUntil([&]()
		{
			return upper.CountersEqual(1, 0);
		}));
		upper.ClearBuffer();
		upper.Reset();
	}
}






