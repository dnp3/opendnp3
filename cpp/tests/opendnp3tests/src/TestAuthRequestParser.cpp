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


#include "BufferHelpers.h"
#include "HexConversions.h"

#include <openpal/util/ToHex.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/authv5/AuthRequestParser.h>

#include <asiodnp3/ConsoleLogger.h>

#include <functional>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace asiodnp3;

#define SUITE(name) "AuthRequestParserTestSuite - " name

class MockAuthRequestHandler : public IAuthRequestHandler
{
	virtual void OnAuthChallenge(const Group120Var1& challenge) override
	{
		
	}

	virtual void OnAuthReply(const Group120Var2& reply) override
	{
		
	}

	virtual void OnRequestKeyStatus(const Group120Var4& status) override
	{
		
	}

	virtual void OnChangeSessionKeys(const Group120Var6& change) override
	{
		
	}

};


TEST_CASE(SUITE("RejectsInsufficientDataForHeader"))
{
	HexSequence buffer("78 01");
	MockAuthRequestHandler handler;
	auto result = AuthRequestParser::Parse(buffer.ToReadOnly(), handler, nullptr);
	REQUIRE(result == ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
}

TEST_CASE(SUITE("RejectsUnknownQualifier"))
{
	HexSequence buffer("78 01 FF FF");
	MockAuthRequestHandler handler;
	auto result = AuthRequestParser::Parse(buffer.ToReadOnly(), handler, nullptr);
	REQUIRE(result == ParseResult::UNKNOWN_QUALIFIER);
}

TEST_CASE(SUITE("RejectsInsufficientFreeFormatData"))
{
	HexSequence buffer("78 01 5B 08 00 FF FF FF FF FF FF FF");
	MockAuthRequestHandler handler;
	auto result = AuthRequestParser::Parse(buffer.ToReadOnly(), handler, nullptr);
	REQUIRE(result == ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
}

TEST_CASE(SUITE("RejectsTooMuchFreeFormatData"))
{
	HexSequence buffer("78 01 5B 08 00 FF FF FF FF FF FF FF FF FF");
	MockAuthRequestHandler handler;
	auto result = AuthRequestParser::Parse(buffer.ToReadOnly(), handler, nullptr);
	REQUIRE(result == ParseResult::TOO_MUCH_DATA_FOR_OBJECTS);
}



