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
#include "AsyncSerialTestObject.h"


#include "BufferHelpers.h"
#include "Exception.h"

#include <asio.hpp>
#include <catch.hpp>

using namespace opendnp3;
using namespace openpal;


//run the tests on arm to give us some protection
#define SUITE(name) "PhysicalLayerSerialSuite - " name
#ifdef SERIAL_PORT

TEST_CASE(SUITE("TestSendReceiveLoopback"))
{
	SerialSettings s;
	s.mDevice = TOSTRING(SERIAL_PORT);
	s.mBaud = 9600;
	s.mDataBits = 8;
	s.mStopBits = 1;
	s.mParity = PAR_NONE;
	s.mFlowType = FLOW_NONE;

	AsyncSerialTestObject t(s);

	t.mPort.AsyncOpen();
	REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUpper)));

	ByteStr b(4096, 0);
	t.mUpper.SendDown(b, b.Size());

	REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::SizeEquals, &t.mUpper, b.Size())));
}

#endif



