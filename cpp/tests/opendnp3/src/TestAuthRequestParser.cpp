/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mocks/MockAPDUHeaderHandler.h"

#include <openpal/util/ToHex.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/parsing/APDUParser.h>

#include <asiodnp3/ConsoleLogger.h>

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>

#include <catch.hpp>

#include <functional>
#include <vector>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace asiodnp3;
using namespace testlib;

#define SUITE(name) "AuthRequestParserTestSuite - " name

TEST_CASE(SUITE("RejectsInsufficientDataForHeader"))
{
    HexSequence buffer("78 01");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
}

TEST_CASE(SUITE("RejectsUnknownQualifier"))
{
    HexSequence buffer("78 01 FF FF");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::UNKNOWN_QUALIFIER);
}

TEST_CASE(SUITE("RejectsInsufficientFreeFormatData"))
{
    HexSequence buffer("78 01 5B 01 08 00 FF FF FF FF FF FF FF");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
}

TEST_CASE(SUITE("RejectsTrailingData"))
{
    HexSequence buffer("78 01 5B 01 08 00 FF FF FF FF FF FF FF FF FF");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
}

TEST_CASE(SUITE("AcceptsMatchingFreeFormatData"))
{
    HexSequence buffer("78 01 5B 01 08 00 11 22 33 44 FF FF FF FF");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::OK);
    REQUIRE(handler.records.size() == 1);
    REQUIRE(handler.authChallenges.size() == 1);
    REQUIRE(handler.authChallenges[0].challengeSeqNum == 0x44332211);
}

TEST_CASE(SUITE("ParsersKeyStatusRequest"))
{
    HexSequence buffer("78 04 07 01 00 00");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::OK);
}

TEST_CASE(SUITE("AcceptsKeyStatusRequest"))
{
    HexSequence buffer("78 04 07 01 09 00");
    MockApduHeaderHandler handler;
    auto result = APDUParser::Parse(buffer.ToRSlice(), handler, nullptr);
    REQUIRE(result == ParseResult::OK);
    REQUIRE(handler.authStatusRequests.size() == 1);
    REQUIRE(handler.authStatusRequests[0].userNum == 9);
}
