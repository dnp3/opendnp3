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
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <APL/CommandQueue.h>
#include <limits>

using namespace apl;


template<class T>
void OptimalTypeTest(T val, SetpointEncodingType correct)
{
	Setpoint sp(val);
	BOOST_REQUIRE_EQUAL(correct, sp.GetOptimalEncodingType());
}

template<class T>
void AutoTypeTest(T val, SetpointEncodingType correct)
{
	Setpoint sp; sp.SetValue(val);
	BOOST_REQUIRE_EQUAL(sp.GetValue(), val);
	BOOST_REQUIRE_EQUAL(correct, sp.GetEncodingType());
}
BOOST_AUTO_TEST_SUITE(CommandTypesSuite)

BOOST_AUTO_TEST_CASE(SetpointSet)
{
	AutoTypeTest(0.01, SPET_AUTO_DOUBLE);
}
BOOST_AUTO_TEST_CASE(SetpointSetInt)
{
	AutoTypeTest(5, SPET_AUTO_INT);
}
BOOST_AUTO_TEST_CASE(OptimalFloat)
{
	OptimalTypeTest(100.0, SPET_FLOAT);
}
BOOST_AUTO_TEST_CASE(OptimalDouble)
{
	OptimalTypeTest(std::numeric_limits<float>::max() * 100.0, SPET_DOUBLE);
}
BOOST_AUTO_TEST_CASE(OptimalInt16)
{
	OptimalTypeTest(55, SPET_INT16);
}
BOOST_AUTO_TEST_CASE(OptimalInt32)
{
	OptimalTypeTest(80000, SPET_INT32);
}


BOOST_AUTO_TEST_CASE(ByteToCommand)
{
	BOOST_REQUIRE_EQUAL(CS_SUCCESS, ByteToCommandStatus(0));
	BOOST_REQUIRE_EQUAL(CS_TIMEOUT, ByteToCommandStatus(1));
	BOOST_REQUIRE_EQUAL(CS_NO_SELECT, ByteToCommandStatus(2));
	BOOST_REQUIRE_EQUAL(CS_FORMAT_ERROR, ByteToCommandStatus(3));
	BOOST_REQUIRE_EQUAL(CS_NOT_SUPPORTED, ByteToCommandStatus(4));
	BOOST_REQUIRE_EQUAL(CS_ALREADY_ACTIVE, ByteToCommandStatus(5));
	BOOST_REQUIRE_EQUAL(CS_HARDWARE_ERROR, ByteToCommandStatus(6));
	BOOST_REQUIRE_EQUAL(CS_LOCAL, ByteToCommandStatus(7));
	BOOST_REQUIRE_EQUAL(CS_TOO_MANY_OPS, ByteToCommandStatus(8));
	BOOST_REQUIRE_EQUAL(CS_NOT_AUTHORIZED, ByteToCommandStatus(9));
}

BOOST_AUTO_TEST_CASE(CommandToString)
{
	BOOST_REQUIRE_EQUAL("CS_SUCCESS", ToString(CS_SUCCESS));
	BOOST_REQUIRE_EQUAL("CS_TIMEOUT", ToString(CS_TIMEOUT));
	BOOST_REQUIRE_EQUAL("CS_NO_SELECT", ToString(CS_NO_SELECT));
	BOOST_REQUIRE_EQUAL("CS_FORMAT_ERROR", ToString(CS_FORMAT_ERROR));
	BOOST_REQUIRE_EQUAL("CS_NOT_SUPPORTED", ToString(CS_NOT_SUPPORTED));
	BOOST_REQUIRE_EQUAL("CS_ALREADY_ACTIVE", ToString(CS_ALREADY_ACTIVE));
	BOOST_REQUIRE_EQUAL("CS_HARDWARE_ERROR", ToString(CS_HARDWARE_ERROR));
	BOOST_REQUIRE_EQUAL("CS_LOCAL", ToString(CS_LOCAL));
	BOOST_REQUIRE_EQUAL("CS_TOO_MANY_OPS", ToString(CS_TOO_MANY_OPS));
	BOOST_REQUIRE_EQUAL("CS_NOT_AUTHORIZED", ToString(CS_NOT_AUTHORIZED));
	BOOST_REQUIRE_EQUAL("Unknown", ToString(CS_UNDEFINED));
}

BOOST_AUTO_TEST_CASE(ByteToControl)
{
	BOOST_REQUIRE_EQUAL(CC_NULL, ByteToControlCode(0));
	BOOST_REQUIRE_EQUAL(CC_PULSE, ByteToControlCode(0x01));
	BOOST_REQUIRE_EQUAL(CC_LATCH_ON, ByteToControlCode(0x03));
	BOOST_REQUIRE_EQUAL(CC_LATCH_OFF, ByteToControlCode(0x04));
	BOOST_REQUIRE_EQUAL(CC_PULSE_CLOSE, ByteToControlCode(0x41));
	BOOST_REQUIRE_EQUAL(CC_PULSE_TRIP, ByteToControlCode(0x81));
	BOOST_REQUIRE_EQUAL(CC_UNDEFINED, ByteToControlCode(0xFF));
}

BOOST_AUTO_TEST_CASE(ControlToString)
{
	BOOST_REQUIRE_EQUAL("CC_NULL", ToString(CC_NULL));
	BOOST_REQUIRE_EQUAL("CC_PULSE", ToString(CC_PULSE));
	BOOST_REQUIRE_EQUAL("CC_LATCH_ON", ToString(CC_LATCH_ON));
	BOOST_REQUIRE_EQUAL("CC_LATCH_OFF", ToString(CC_LATCH_OFF));
	BOOST_REQUIRE_EQUAL("CC_PULSE_CLOSE", ToString(CC_PULSE_CLOSE));
	BOOST_REQUIRE_EQUAL("CC_PULSE_TRIP", ToString(CC_PULSE_TRIP));
	BOOST_REQUIRE_EQUAL("Unknown", ToString(CC_UNDEFINED));
}

BOOST_AUTO_TEST_CASE(CommandTypeToString)
{
	BOOST_REQUIRE_EQUAL("BinaryOutput", ToString(CT_BINARY_OUTPUT));
	BOOST_REQUIRE_EQUAL("Setpoint", ToString(CT_SETPOINT));
	BOOST_REQUIRE_EQUAL("Unknown", ToString(CT_NONE));
}

BOOST_AUTO_TEST_SUITE_END()


