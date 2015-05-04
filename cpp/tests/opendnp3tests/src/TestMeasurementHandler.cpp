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
#include <catch.hpp>

#include <opendnp3/master/MeasurementHandler.h>

#include <testlib/BufferHelpers.h>

#include "LogTester.h"
#include "MockSOEHandler.h"

#include <functional>

using namespace openpal;
using namespace opendnp3;
using namespace testlib;

/**
* Test that the measurement handler correctly interprets measurement values in response data
*/
#define SUITE(name) "MeasurementHandlerTestSuite - " name

// Parse some input and verify that the ISOEHandler is invoked with expected results
bool TestObjectHeaders(const std::string& objects, const std::function<void(MockSOEHandler&)>& verify);

TEST_CASE(SUITE("accepts empty response"))
{
	auto verify = [](MockSOEHandler& soe) 
	{
		REQUIRE(soe.TotalReceived() == 0);
	};

	REQUIRE(TestObjectHeaders("", verify));
}

TEST_CASE(SUITE("parses g121v1 correctly"))
{
	auto verify = [](MockSOEHandler& soe) 
	{
		REQUIRE(soe.TotalReceived() == 1);
	};

	// g120v1 - 1 byte start/stop - 2->2 - flags: 0x01, assoc = 0x0007, count = 0x00000008
	auto header = "79 01 00 02 02 01 07 00 08 00 00 00";

	REQUIRE(TestObjectHeaders(header, verify));
}

bool TestObjectHeaders(const std::string& objects, const std::function<void(MockSOEHandler&)>& verify)
{
	MockSOEHandler soe;
	LogTester log;
	auto logger = log.GetLogger();

	HexSequence hex(objects);

	auto result = MeasurementHandler::ProcessMeasurements(hex.ToReadOnly(), logger, &soe);
	verify(soe);
	return result;
}