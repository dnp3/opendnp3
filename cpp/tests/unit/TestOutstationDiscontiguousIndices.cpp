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
#include "utils/APDUHexBuilders.h"
#include "utils/OutstationTestObject.h"

#include <dnp3mocks/DatabaseHelpers.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "OutstationDiscontiguousIndexTestSuite - " name

std::string QueryDiscontiguousBinary(const std::string& request)
{
    OutstationTestObject t(OutstationConfig(),
                           configure::from({{2, BinaryConfig{}}, {4, BinaryConfig{}}, {5, BinaryConfig{}}

                           }));

    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Binary(true, Flags(0x01)), 2, EventMode::Suppress);
        db.Update(Binary(false, Flags(0x01)), 4, EventMode::Suppress);
    });

    t.SendToOutstation(request);
    return t.lower->PopWriteAsHex();
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
