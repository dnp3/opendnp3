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
#include "mocks/OutstationTestObject.h"

#include <dnp3mocks/APDUHexBuilders.h>

#include <testlib/HexConversions.h>

#include <catch.hpp>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "OutstationDiscontiguousIndexTestSuite - " name

TEST_CASE(SUITE("integrity scan returns expected result with default index assignments"))
{
    // this will tell the outstation to use discontiguous index mode, but we won't change the address assignments
    OutstationConfig config;
    config.params.indexMode = IndexMode::Discontiguous;
    OutstationTestObject t(config, DatabaseSizes::BinaryOnly(2));
    t.LowerLayerUp();

    t.SendToOutstation(hex::IntegrityPoll(0));

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 02 00 00 01 02 02");
}

TEST_CASE(SUITE("can safely update non-existent index in discontiguous index mode"))
{
    OutstationConfig config;
    config.params.indexMode = IndexMode::Discontiguous;
    OutstationTestObject t(config, DatabaseSizes::BinaryOnly(2));

    auto view = t.context.GetConfigView();
    for (int i = 0; i < 2; ++i)
    {
        view.binaries[i].config.clazz = PointClass::Class1;
        view.binaries[i].config.svariation = StaticBinaryVariation::Group1Var2;
        view.binaries[i].config.evariation = EventBinaryVariation::Group2Var3;
        view.binaries[i].config.vIndex = i + 2;
    }

    // now send an update to boundary points that doesn't exist
    REQUIRE_FALSE(t.context.GetUpdateHandler().Update(Binary(true), 0, EventMode::Suppress));
    REQUIRE_FALSE(t.context.GetUpdateHandler().Update(Binary(true), 1, EventMode::Suppress));
    // 2 & 3 exist
    REQUIRE_FALSE(t.context.GetUpdateHandler().Update(Binary(true), 4, EventMode::Suppress));
}

TEST_CASE(SUITE("range scan returns expected results with default index assignments"))
{
    // this will tell the outstation to use discontiguous index mode, but we won't change the address assignments
    OutstationConfig config;
    config.params.indexMode = IndexMode::Discontiguous;
    OutstationTestObject t(config, DatabaseSizes::BinaryOnly(2));
    t.LowerLayerUp();

    t.SendToOutstation("C0 01 01 02 00 00 01");

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 02 00 00 01 02 02");
}

std::string QueryDiscontiguousBinary(const std::string& request)
{
    OutstationConfig config;
    config.params.indexMode = IndexMode::Discontiguous;

    OutstationTestObject t(config, DatabaseSizes::BinaryOnly(3));

    // assign virtual indices to the database specified above
    auto view = t.context.GetConfigView();
    view.binaries[0].config.vIndex = 2;
    view.binaries[1].config.vIndex = 4;
    view.binaries[2].config.vIndex = 5;

    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Binary(true, 0x01), 2, EventMode::Suppress);
        db.Update(Binary(false, 0x01), 4, EventMode::Suppress);
    });

    t.SendToOutstation(request);
    return t.lower->PopWriteAsHex();
}

TEST_CASE(SUITE("octet strings behave as expected with discontiguous indices"))
{
    // this will tell the outstation to use discontiguous index mode, but we won't change the address assignments
    OutstationConfig config;
    config.eventBufferConfig.maxOctetStringEvents = 2;
    config.params.indexMode = IndexMode::Discontiguous;
    OutstationTestObject t(config, DatabaseSizes::OctetStringOnly(2));
    {
        auto view = t.context.GetConfigView();
        view.octetStrings[0].config.vIndex = 1;
        view.octetStrings[1].config.vIndex = 3;
    }
    t.LowerLayerUp();

    t.SendToOutstation(hex::IntegrityPoll(0));

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 6E 01 00 01 01 00 6E 01 00 03 03 00");
}

TEST_CASE(SUITE("ReadDiscontiguousClass0"))
{
    REQUIRE(QueryDiscontiguousBinary("C0 01 3C 01 06") == "C0 81 80 00 01 02 00 02 02 81 01 02 00 04 05 01 02");
}

TEST_CASE(SUITE("ReadDiscontiguousBadRangeBelow"))
{
    // read 01 var 2, [00 : 01]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 00 01") == "C0 81 80 04");
}

TEST_CASE(SUITE("ReadDiscontiguousBadRangeAbove"))
{
    // read 01 var 2, [06 : 09]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 06 09") == "C0 81 80 04");
}

TEST_CASE(SUITE("ReadDiscontiguousSingleRange"))
{
    // read 01 var 2, [02 : 02]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 02 02") == "C0 81 80 00 01 02 00 02 02 81");
}

TEST_CASE(SUITE("ReadDiscontiguousDoubleRange"))
{
    // read 01 var 2, [04 : 05]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 04 05") == "C0 81 80 00 01 02 00 04 05 01 02");
}

TEST_CASE(SUITE("ReadDiscontiguousPastUpperBound"))
{
    // read 01 var 2, [05 : 06]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 05 06") == "C0 81 80 04 01 02 00 05 05 02");
}

TEST_CASE(SUITE("ReadDiscontiguousAllDataWithMultipleRanges"))
{
    // read 01 var 2, [02 : 02]; read 01 var 2, [04 : 05]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 02 02 01 02 00 04 05")
            == "C0 81 80 00 01 02 00 02 02 81 01 02 00 04 05 01 02");
}

TEST_CASE(SUITE("ReadDiscontiguousAllDataWithMultipleRangesAndRangeError"))
{
    // read 01 var 2, [02 : 03]; read 01 var 2, [04 : 05]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 02 03 01 02 00 04 05")
            == "C0 81 80 04 01 02 00 02 02 81 01 02 00 04 05 01 02");
}

TEST_CASE(SUITE("ReadDiscontiguousAllDataWithRangeError"))
{
    // read 01 var 2, [02 : 05]
    REQUIRE(QueryDiscontiguousBinary("C0 01 01 02 00 02 05") == "C0 81 80 04 01 02 00 02 02 81 01 02 00 04 05 01 02");
}
