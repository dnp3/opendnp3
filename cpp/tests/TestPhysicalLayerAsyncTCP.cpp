
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
#include <boost/asio.hpp>

#include <functional>

#include <opendnp3/LogToStdio.h>

#include <asiopal/ASIOExecutor.h>

#include <openpal/Exception.h>

#include "RandomizedBuffer.h"
#include "PhysLoopback.h"
#include "TestHelpers.h"
#include "BufferHelpers.h"
#include "AsyncTestObjectASIO.h"
#include "AsyncPhysTestObject.h"

#include <iostream>

using namespace opendnp3;
using namespace boost;
using namespace openpal;
using namespace asiopal;


BOOST_AUTO_TEST_SUITE(PhysicalLayerAsyncTCPSuite)

BOOST_AUTO_TEST_CASE(TestStateClosed)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	uint8_t buff[100];

	// Test that reads/writes of length 0 throw ArgumentException
	BOOST_REQUIRE_THROW(t.mTCPClient.AsyncWrite(buff, 0), ArgumentException);
	BOOST_REQUIRE_THROW(t.mTCPClient.AsyncRead(buff, 0), ArgumentException);

	//Test that in the closed state we get the proper invalid state exceptions
	BOOST_REQUIRE_THROW(t.mTCPClient.AsyncWrite(buff, 100), InvalidStateException);
	BOOST_REQUIRE_THROW(t.mTCPClient.AsyncRead(buff, 100), InvalidStateException);
	BOOST_REQUIRE_THROW(t.mTCPClient.AsyncClose(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(ClientConnectionRejected)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	BOOST_REQUIRE_EQUAL(t.mClientAdapter.GetNumOpenFailure(), 0);

	for(size_t i = 0; i < 2; ++i) {
		t.mTCPClient.AsyncOpen();
		BOOST_REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}

BOOST_AUTO_TEST_CASE(ClientConnectionCanceled)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	for(size_t i = 0; i < 2; ++i) {
		t.mTCPClient.AsyncOpen();
		t.mTCPClient.AsyncClose();

		BOOST_REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}

BOOST_AUTO_TEST_CASE(ServerAcceptCanceled)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	for(size_t i = 0; i < 2; ++i) {
		t.mTCPServer.AsyncOpen();
		t.mTCPServer.AsyncClose();

		BOOST_REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mServerAdapter, i + 1)));
	}
}

BOOST_AUTO_TEST_CASE(ConnectDisconnect)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	for(size_t i = 0; i < 10; ++i) {

		t.mTCPServer.AsyncOpen();
		t.mTCPClient.AsyncOpen();
		BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
		BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));

		//Check that since reads are outstanding, you only have to stop 1/2 of the connection
		if( (i % 2) == 0 ) t.mTCPServer.AsyncClose();
		else t.mTCPClient.AsyncClose();
		BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
		BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));
	}
}

BOOST_AUTO_TEST_CASE(TestSendShutdown)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	t.mTCPServer.AsyncOpen();
	t.mTCPClient.AsyncOpen();
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));

	ByteStr bs(1024, 77); //give some interesting seed value to make sure bytes are correctly written
	t.mClientUpper.SendDown(bs.Buffer(), bs.Size());

	t.mTCPClient.AsyncClose();
	BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
	BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));
}

BOOST_AUTO_TEST_CASE(TwoWaySend)
{
	const size_t SEND_SIZE = 1 << 20; // 1 MB

	AsyncPhysTestObject t(LEV_INFO, false);

	t.mTCPServer.AsyncOpen();
	t.mTCPClient.AsyncOpen();
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));

	//both layers are now up and reading, start them both writing
	ByteStr bs(SEND_SIZE, 77); //give some interesting seed value to make sure bytes are correctly written
	t.mClientUpper.SendDown(bs.Buffer(), bs.Size());
	t.mServerUpper.SendDown(bs.Buffer(), bs.Size());

	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &t.mServerUpper, SEND_SIZE)));
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &t.mClientUpper, SEND_SIZE)));

	BOOST_REQUIRE(t.mClientUpper.BufferEquals(bs.Buffer(), bs.Size()));
	BOOST_REQUIRE(t.mServerUpper.BufferEquals(bs.Buffer(), bs.Size()));

	t.mTCPServer.AsyncClose(); //stop one side
	BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
	BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));
}

BOOST_AUTO_TEST_CASE(ServerAsyncCloseWhileOpeningKillsAcceptor)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	BOOST_REQUIRE_EQUAL(0, t.mClientAdapter.GetNumOpenFailure());

	for(size_t i = 0; i < 5; ++i) {
		t.mTCPServer.AsyncOpen();
		t.mTCPServer.AsyncClose();

		BOOST_REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mServerAdapter, i + 1)));

		// since we closed the server socket we shouldn't be able to connect now
		t.mTCPClient.AsyncOpen();

		BOOST_REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}

BOOST_AUTO_TEST_CASE(ServerAsyncCloseAfterOpeningKillsAcceptor)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	BOOST_REQUIRE_EQUAL(t.mClientAdapter.GetNumOpenFailure(), 0);

	for(size_t i = 0; i < 5; ++i) {
		t.mTCPServer.AsyncOpen();
		t.mTCPClient.AsyncOpen();

		BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
		BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));

		t.mTCPServer.AsyncClose();

		BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mServerUpper)));
		BOOST_REQUIRE(t.ProceedUntilFalse(std::bind(&MockUpperLayer::IsLowerLayerUp, &t.mClientUpper)));

		// since we closed the server socket we shouldn't be able to connect now
		t.mTCPClient.AsyncOpen();

		BOOST_REQUIRE(t.ProceedUntil(std::bind(&LowerLayerToPhysAdapter::OpenFailureEquals, &t.mClientAdapter, i + 1)));
	}
}


#define MACRO_LOOPBACK_SIZE 100
#ifndef ARM
#define MACRO_LOOPBACK_ITERATIONS 100
#else
#define MACRO_LOOPBACK_ITERATIONS 10
#endif

BOOST_AUTO_TEST_CASE(Loopback)
{
	const size_t SIZE = MACRO_LOOPBACK_SIZE;
	const size_t ITERATIONS = MACRO_LOOPBACK_ITERATIONS;

	EventLog log;
	Logger logger(&log, LEV_INFO, "test");
	AsyncTestObjectASIO test;
	PhysicalLayerAsyncTCPServer server(logger.GetSubLogger("server"), test.GetService(), "127.0.0.1", 30000);

	PhysLoopback loopback(logger.GetSubLogger("loopback"), &server);
	loopback.Start();

	PhysicalLayerAsyncTCPClient client(logger.GetSubLogger("client"), test.GetService(), "127.0.0.1", 30000);
	LowerLayerToPhysAdapter adapter(logger.GetSubLogger("adapter"), &client);
	MockUpperLayer upper(logger.GetSubLogger("mock"));
	adapter.SetUpperLayer(&upper);

	client.AsyncOpen();
	BOOST_REQUIRE(test.ProceedUntil(std::bind(&MockUpperLayer::IsLowerLayerUp, &upper)));

	RandomizedBuffer rb(SIZE);

	for(size_t i = 0; i < ITERATIONS; ++i) {
		rb.Randomize();
		upper.SendDown(rb, rb.Size());
		BOOST_REQUIRE(test.ProceedUntil(std::bind(&MockUpperLayer::BufferEquals, &upper, rb.Buffer(), rb.Size())));
		BOOST_REQUIRE(test.ProceedUntil(std::bind(&MockUpperLayer::CountersEqual, &upper, 1, 0)));
		upper.ClearBuffer();
		upper.Reset();
	}
}




BOOST_AUTO_TEST_SUITE_END()

