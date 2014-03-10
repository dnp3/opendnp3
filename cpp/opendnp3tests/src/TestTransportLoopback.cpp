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
#include "TransportLoopbackTestObject.h"

#include <asio.hpp>
#include <catch.hpp>
#include <functional>

#include <asiopal/Log.h>
#include <asiopal/PhysicalLayerAsyncSerial.h>

#include "Exception.h"
#include "ProtocolUtil.h"
#include "BufferHelpers.h"
#include "LoopbackPhysicalLayerAsync.h"

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace boost;

#define SUITE(name) "TransportLoopbackTestSuite - " name

// Do a bidirectional send operation and proceed until both sides have correctly
// received all the data
void TestLoopback(TransportLoopbackTestObject* apTest, uint32_t aNumBytes)
{
	apTest->Start();

	REQUIRE(apTest->ProceedUntil(std::bind(&TransportLoopbackTestObject::LayersUp, apTest)));

	ByteStr b(aNumBytes, 0);

	apTest->mUpperA.SendDown(b.ToReadOnly());
	apTest->mUpperB.SendDown(b.ToReadOnly());

	REQUIRE(apTest->ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &(apTest->mUpperA), b.Size())));
	REQUIRE(apTest->ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &(apTest->mUpperB), b.Size())));
	REQUIRE(apTest->mUpperA.BufferEquals(b, b.Size()));
	REQUIRE(apTest->mUpperB.BufferEquals(b, b.Size()));
}

TEST_CASE(SUITE("TestTransportWithMockLoopback"))
{
	LinkConfig cfgA(true, true);
	LinkConfig cfgB(false, true);

	EventLog log;
	asio::io_service service;
	LoopbackPhysicalLayerAsync phys(Logger(&log, LogLevel::Warning, "loopback"), &service);
	TransportLoopbackTestObject t(&service, &phys, cfgA, cfgB);

	TestLoopback(&t, sizes::DEFAULT_APDU_BUFFER_SIZE);
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
	PhysicalLayerAsyncSerial phys(log.GetLogger(LogLevel::Warning, "serial"), &service, s);
	TransportLoopbackTestObject t(&service, &phys, cfgA, cfgB);

	TestLoopback(&t, DEFAULT_FRAG_SIZE);
}
#endif


