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

#include <asio.hpp>
#include <catch.hpp>
#include <functional>

#include <asiopal/PhysicalLayerSerial.h>

#include <opendnp3/app/AppConstants.h>
#include <dnp3mocks/ProtocolUtil.h>

#include "mocks/TransportLoopbackTestObject.h"
#include "mocks/LoopbackPhysicalLayer.h"

#include <testlib/BufferHelpers.h>


using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace testlib;

#define SUITE(name) "TransportLoopbackTestSuite - " name

// Do a bidirectional send operation and proceed until both sides have correctly
// received all the data
void TestLoopback(TransportLoopbackTestObject* pTest, uint32_t numBytes)
{
	pTest->Start();

	REQUIRE(pTest->ProceedUntil([&]()
	{
		return pTest->LayersUp();
	}));

	ByteStr b(numBytes, 0);

	pTest->mUpperA.SendDown(b.ToRSlice());
	pTest->mUpperB.SendDown(b.ToRSlice());

	REQUIRE(pTest->ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &(pTest->mUpperA), b.Size())));
	REQUIRE(pTest->ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &(pTest->mUpperB), b.Size())));
	REQUIRE(pTest->mUpperA.BufferEquals(b, b.Size()));
	REQUIRE(pTest->mUpperB.BufferEquals(b, b.Size()));
}

TEST_CASE(SUITE("TestTransportWithMockLoopback"))
{
	auto level = levels::NORMAL;

	LinkConfig cfgA(true, true);
	LinkConfig cfgB(false, true);

	testlib::MockLogHandler log;
	LogRoot root(&log, "test", level);
	asio::io_service service;
	LoopbackPhysicalLayer phys(root.logger, service);
	TransportLoopbackTestObject t(root.logger, service, phys.executor, &phys, cfgA, cfgB);

	TestLoopback(&t, opendnp3::DEFAULT_MAX_APDU_SIZE);
}

// Run this test on ARM to give us some regression protection for serial
#ifdef SERIAL_PORT
TEST_CASE(SUITE("TestTransportWithSerialLoopback"))
{
	LinkConfig cfgA(true, true);
	LinkConfig cfgB(false, true);

	cfgA.NumRetry = cfgB.NumRetry = 3;

	SerialSettings s;
	s.mDevice = TOSTRING(SERIAL_PORT);
	s.mBaud = 57600;
	s.mDataBits = 8;
	s.mStopBits = 1;
	s.mParity = PAR_NONE;
	s.mFlowType = FLOW_NONE;

	EventLog log;
	asio::io_service service;
	PhysicalLayerSerial phys(log.GetLogger(flags::WARN, "serial"), &service, s);
	TransportLoopbackTestObject t(&service, &phys, cfgA, cfgB);

	TestLoopback(&t, DEFAULT_FRAG_SIZE);
}
#endif


