/*
 * Copyright 2013-2020 Automatak, LLC
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
#include "utils/BufferHelpers.h"

#include "dnp3mocks/MockLogHandler.h"
#include "dnp3mocks/MockSOEHandler.h"

#include <catch.hpp>
#include <master/MeasurementHandler.h>

#include <functional>

using namespace opendnp3;

/**
 * Test that the measurement handler correctly interprets measurement values in response data
 */
#define SUITE(name) "MeasurementHandlerTestSuite - " name

// Parse some input and verify that the ISOEHandler is invoked with expected results
ParseResult TestObjectHeaders(const std::string& objects,
                              ParseResult expectedResult,
                              const std::function<void(MockSOEHandler&)>& verify);

TEST_CASE(SUITE("accepts empty response"))
{
    auto verify = [](MockSOEHandler& soe) { REQUIRE(soe.TotalReceived() == 0); };

    TestObjectHeaders("", ParseResult::OK, verify);
}

TEST_CASE(SUITE("parses g50v1 correctly"))
{
    auto verify = [](MockSOEHandler& soe) {
        REQUIRE(soe.TotalReceived() == 2);
        REQUIRE(soe.timeSOE.size() == 2);
        REQUIRE(soe.timeSOE[0].value == 0xABABABABABAB);
        REQUIRE(soe.timeSOE[1].value == 0xBCBCBCBCBCBC);
    };

    // g50v1 count of 2
    auto objects = "32 01 07 02 AB AB AB AB AB AB BC BC BC BC BC BC";

    TestObjectHeaders(objects, ParseResult::OK, verify);
}

TEST_CASE(SUITE("parses g2v1 w/ invalid timestamp quality"))
{
    auto verify = [](MockSOEHandler& soe) {
        const auto record = soe.binarySOE.find(0XAA);
        REQUIRE(record != soe.binarySOE.end());        
        REQUIRE(record->second.meas.time.quality == TimestampQuality::INVALID);
    };

    auto objects = "02 01 17 01 AA 81";

    TestObjectHeaders(objects, ParseResult::OK, verify);
}


TEST_CASE(SUITE("parses g2v2 w/ synchronized timestamp quality"))
{
    auto verify = [](MockSOEHandler& soe) {
        const auto record = soe.binarySOE.find(0XAA);
        REQUIRE(record != soe.binarySOE.end());
        REQUIRE(record->second.meas.time.value == 8);
        REQUIRE(record->second.meas.time.quality == TimestampQuality::SYNCHRONIZED);
    };

    auto objects = "02 02 17 01 AA 81 08 00 00 00 00 00";

    TestObjectHeaders(objects, ParseResult::OK, verify);
}

TEST_CASE(SUITE("parses g2v3 w/ synchronized CTO"))
{
    auto verify = [](MockSOEHandler& soe) { 
        const auto record = soe.binarySOE.find(8);
        REQUIRE(record != soe.binarySOE.end());
        REQUIRE(record->second.meas.time.value == 8); // 7 + 1
        REQUIRE(record->second.meas.time.quality == TimestampQuality::SYNCHRONIZED);
    };
    
    auto objects = "33 01 07 01 07 00 00 00 00 00 02 03 17 01 08 81 01 00";

    TestObjectHeaders(objects, ParseResult::OK, verify);
}

TEST_CASE(SUITE("parses g2v3 w/ unsynchronized CTO"))
{
    auto verify = [](MockSOEHandler& soe) {
        const auto record = soe.binarySOE.find(8);
        REQUIRE(record != soe.binarySOE.end());
        REQUIRE(record->second.meas.time.value == 8); // 7 + 1
        REQUIRE(record->second.meas.time.quality == TimestampQuality::UNSYNCHRONIZED);
    };

    auto objects = "33 02 07 01 07 00 00 00 00 00 02 03 17 01 08 81 01 00";

    TestObjectHeaders(objects, ParseResult::OK, verify);
}

ParseResult TestObjectHeaders(const std::string& objects,
                              ParseResult expectedResult,
                              const std::function<void(MockSOEHandler&)>& verify)
{
    MockSOEHandler soe;
    MockLogHandler log;

    HexSequence hex(objects);

    auto result
        = MeasurementHandler::ProcessMeasurements(ResponseInfo(true, true, true), hex.ToRSeq(), log.logger, &soe);
    REQUIRE(result == expectedResult);
    verify(soe);
    return result;
}