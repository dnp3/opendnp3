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

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>
#include <testlib/MockLogHandler.h>

#include <secauth/AggressiveModeParser.h>
#include <functional>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace secauth;
using namespace testlib;

void TestAggMode(const std::string& data, function<void(AggModeResult result)> validate)
{
	HexSequence objects(data);
	auto result = AggressiveModeParser::IsAggressiveMode(objects.ToRSlice(), nullptr);
	validate(result);
}

#define SUITE(name) "AggressiveModeParsingTestSuite - " name

TEST_CASE(SUITE("AcceptsValidInput"))
{
	TestAggMode("78 03 07 01 04 00 00 00 09 00 DE AD BE EF", [](AggModeResult result)
	{
		REQUIRE(result.result == ParseResult::OK);
		REQUIRE(result.isAggMode);
		REQUIRE(result.request.challengeSeqNum == 4);
		REQUIRE(result.request.userNum == 9);
		REQUIRE(ToHex(result.remainder) == "DE AD BE EF");
	});
}

TEST_CASE(SUITE("RejectsBadHeader"))
{
	TestAggMode("78 03", [](AggModeResult result)
	{
		REQUIRE(result.result == ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
		REQUIRE(!result.isAggMode);
	});
}

TEST_CASE(SUITE("RejectsNormalObject"))
{
	TestAggMode("02 01 01", [](AggModeResult result)
	{
		REQUIRE(result.result == ParseResult::OK);
		REQUIRE(!result.isAggMode);
	});
}

TEST_CASE(SUITE("RejectsInsufficientData"))
{
	TestAggMode("78 03 07 01 FF FF FF FF FF", [](AggModeResult result)
	{
		REQUIRE(result.result == ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
		REQUIRE(!result.isAggMode);
	});
}



