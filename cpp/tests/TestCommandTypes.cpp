
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

#include "TestHelpers.h"

#include <opendnp3/CommandStatus.h>
#include <opendnp3/ControlRelayOutputBlock.h>

using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(CommandTypesSuite)

BOOST_AUTO_TEST_CASE(ByteToCommand)
{
	BOOST_REQUIRE_EQUAL(CS_SUCCESS, IntToCommandStatus(0));
	BOOST_REQUIRE_EQUAL(CS_TIMEOUT, IntToCommandStatus(1));
	BOOST_REQUIRE_EQUAL(CS_NO_SELECT, IntToCommandStatus(2));
	BOOST_REQUIRE_EQUAL(CS_FORMAT_ERROR, IntToCommandStatus(3));
	BOOST_REQUIRE_EQUAL(CS_NOT_SUPPORTED, IntToCommandStatus(4));
	BOOST_REQUIRE_EQUAL(CS_ALREADY_ACTIVE, IntToCommandStatus(5));
	BOOST_REQUIRE_EQUAL(CS_HARDWARE_ERROR, IntToCommandStatus(6));
	BOOST_REQUIRE_EQUAL(CS_LOCAL, IntToCommandStatus(7));
	BOOST_REQUIRE_EQUAL(CS_TOO_MANY_OPS, IntToCommandStatus(8));
	BOOST_REQUIRE_EQUAL(CS_NOT_AUTHORIZED, IntToCommandStatus(9));
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
	BOOST_REQUIRE_EQUAL(CC_NULL, IntToControlCode(0));
	BOOST_REQUIRE_EQUAL(CC_PULSE, IntToControlCode(0x01));
	BOOST_REQUIRE_EQUAL(CC_LATCH_ON, IntToControlCode(0x03));
	BOOST_REQUIRE_EQUAL(CC_LATCH_OFF, IntToControlCode(0x04));
	BOOST_REQUIRE_EQUAL(CC_PULSE_CLOSE, IntToControlCode(0x41));
	BOOST_REQUIRE_EQUAL(CC_PULSE_TRIP, IntToControlCode(0x81));
	BOOST_REQUIRE_EQUAL(CC_UNDEFINED, IntToControlCode(0xFF));
}

BOOST_AUTO_TEST_CASE(ControlToString)
{
	BOOST_REQUIRE_EQUAL("CC_NULL", ControlCodeToString(CC_NULL));
	BOOST_REQUIRE_EQUAL("CC_PULSE", ControlCodeToString(CC_PULSE));
	BOOST_REQUIRE_EQUAL("CC_LATCH_ON", ControlCodeToString(CC_LATCH_ON));
	BOOST_REQUIRE_EQUAL("CC_LATCH_OFF", ControlCodeToString(CC_LATCH_OFF));
	BOOST_REQUIRE_EQUAL("CC_PULSE_CLOSE", ControlCodeToString(CC_PULSE_CLOSE));
	BOOST_REQUIRE_EQUAL("CC_PULSE_TRIP", ControlCodeToString(CC_PULSE_TRIP));
	BOOST_REQUIRE_EQUAL("Unknown", ControlCodeToString(CC_UNDEFINED));
}

BOOST_AUTO_TEST_SUITE_END()

