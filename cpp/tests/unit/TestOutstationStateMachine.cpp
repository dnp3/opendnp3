/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include <ser4cpp/util/HexConversions.h>

#include <dnp3mocks/DatabaseHelpers.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "OutstationStateMachineTestSuite - " name

TEST_CASE(SUITE("Responds to repeat READ request with same octets as last response"))
{
    OutstationConfig config;
    OutstationTestObject t(config, configure::by_count_of::analog_input(1));
    t.LowerLayerUp();

    t.SendToOutstation("C0 01 1E 00 06");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00");
    t.OnTxReady();

    // change the value in the outstation
    t.Transaction([](IUpdateHandler& db) { db.Update(Analog(1, Flags(0x01)), 0); });

    // repeat the read request
    t.SendToOutstation("C0 01 1E 00 06");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00");
}

TEST_CASE(SUITE("Responds to non-READ request while waiting for unsolicited confirm"))
{
    OutstationConfig config;
    config.params.allowUnsolicited = true;
    OutstationTestObject t(config, configure::by_count_of::analog_input(1));
    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == "F0 82 80 00");
    t.OnTxReady();

    t.SendToOutstation("C0 02");                        // empty write
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00"); // null response
    t.OnTxReady();

    REQUIRE(t.lower->PopWriteAsHex().empty()); // shouldn't send anything else
}
