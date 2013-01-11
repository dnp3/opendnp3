//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//

#include "AsyncSerialTestObject.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>
#include <boost/bind.hpp>

#include <APL/Thread.h>
#include <APL/Log.h>
#include <APLTestTools/BufferHelpers.h>
#include <APL/Exception.h>

using namespace apl;
using namespace boost;



//run the tests on arm to give us some protection
BOOST_AUTO_TEST_SUITE(PhysicalLayerSerialSuite)
#ifdef SERIAL_PORT

BOOST_AUTO_TEST_CASE(TestSendReceiveLoopback)
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
	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUpper)));

	ByteStr b(4096, 0);
	t.mUpper.SendDown(b, b.Size());

	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::SizeEquals, &t.mUpper, b.Size())));
}

#endif

BOOST_AUTO_TEST_SUITE_END()

