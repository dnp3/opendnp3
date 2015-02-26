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
#include <secauthv5/AggressiveModeParser.h>

#include <functional>

#include "LogTester.h"

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace secauthv5;
using namespace testlib;

void TestAggMode(const std::string& data, function<void (std::pair<ParseResult, bool>, Group120Var3)> validate)
{
	HexSequence objects(data);
	Group120Var3 request;
	auto result = AggressiveModeParser::IsAggressiveMode(objects.ToReadOnly(), request, nullptr);
	validate(result, request);
}

#define SUITE(name) "AggressiveModeParsingTestSuite - " name

TEST_CASE(SUITE("AcceptsValidInput"))
{
	TestAggMode("78 03 07 01 04 00 00 00 09 00", [](std::pair<ParseResult, bool> result, Group120Var3 request)
	{	
		REQUIRE(result.first == ParseResult::OK);
		REQUIRE(result.second);		
		REQUIRE(request.challengeSeqNum == 4);
		REQUIRE(request.userNum == 9);
	});	
}

TEST_CASE(SUITE("RejectsBadHeader"))
{
	TestAggMode("78 03", [](std::pair<ParseResult, bool> result, Group120Var3 request)
	{
		REQUIRE(result.first == ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
		REQUIRE(!result.second);		
	});
}

TEST_CASE(SUITE("RejectsNormalObject"))
{
	TestAggMode("02 01 01", [](std::pair<ParseResult, bool> result, Group120Var3 request)
	{
		REQUIRE(result.first == ParseResult::OK);
		REQUIRE(!result.second);		
	});
}

TEST_CASE(SUITE("RejectsInsufficientData"))
{
	TestAggMode("78 03 07 01 FF FF FF FF FF", [](std::pair<ParseResult, bool> result, Group120Var3 request)
	{
		REQUIRE(result.first == ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
		REQUIRE(!result.second);		
	});
}



