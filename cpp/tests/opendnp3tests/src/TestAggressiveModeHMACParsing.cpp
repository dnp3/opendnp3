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

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>
#include <secauthv5/AggressiveModeParser.h>

#include <functional>

#include "LogTester.h"

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace secauthv5;
using namespace testlib;

void TestAggModeHMAC(const std::string& data, uint32_t hmacSize, function<void(AggModeHMACResult result)> validate)
{
	HexSequence objects(data);	
	auto result = AggressiveModeParser::ParseHMAC(objects.ToReadOnly(), hmacSize, nullptr);
	validate(result);
}

#define SUITE(name) "AggressiveModeHMACParsingTestSuite - " name

TEST_CASE(SUITE("AcceptsValidInput"))
{
	auto objects = "DE AD BE EF";
	auto trailer = "78 09 5B 01 04 00";
	auto hmac = "AB BA AB BA";
	auto input = AppendHex({ objects, trailer, hmac });

	TestAggModeHMAC(input, 4, [=](AggModeHMACResult result)
	{	
		REQUIRE(result.result == ParseResult::OK);
		REQUIRE(ToHex(result.hmac.hmacValue) == hmac);
		REQUIRE(ToHex(result.objects) == objects);
	});	
}

TEST_CASE(SUITE("RejectsBadAlignment"))
{
	auto input = "DE AD BE EF 78 09 5B 01 04 00 AB BA AB BA";	

	// this causes an unknown object
	TestAggModeHMAC(input, 3, [=](AggModeHMACResult result)
	{
		REQUIRE(result.result == ParseResult::UNKNOWN_OBJECT);
	});
}

TEST_CASE(SUITE("RejectsSizeMismatch"))
{
	auto input = "DE AD BE EF 78 09 5B 01 03 00 AB BA AB BA";

	// this causes an unknown object
	TestAggModeHMAC(input, 4, [=](AggModeHMACResult result)
	{
		REQUIRE(result.result == ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
	});
}

TEST_CASE(SUITE("RejectsInsufficient"))
{
	auto input = "DE AD BE EF 78 09 5B 01 03";

	// this causes an unknown object
	TestAggModeHMAC(input, 4, [=](AggModeHMACResult result)
	{
		REQUIRE(result.result == ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
	});
}



