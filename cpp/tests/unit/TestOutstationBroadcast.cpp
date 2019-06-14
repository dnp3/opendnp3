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
#include "utils/OutstationTestObject.h"
#include "utils/APDUHexBuilders.h"

#include <dnp3mocks/DatabaseHelpers.h>

#include <ser4cpp/util/HexConversions.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "OutstationBroadcastTestSuite - " name

TEST_CASE(SUITE("Mandatory confirmation broadcast asks for confirmation"))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::ShallConfirm, hex::ClearRestartIIN(0));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should have ALL_STATIONS IIN set, and should ask for confirmation
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "E2 81 01 00");
    t.OnTxReady();

    // The next response should have ALL_STATIONS IIN set, and should ask for confirmation
    t.SendToOutstation(hex::ClassPoll(3, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "E3 81 01 00");
    t.OnTxReady();

    // Confirm the last response
    t.SendToOutstation(hex::SolicitedConfirm(3));
    t.OnTxReady();

    // Check that no confirmation is asked and that ALL_STATIONS IIN is reset
    t.SendToOutstation(hex::ClassPoll(3, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C3 81 00 00");
}

TEST_CASE(SUITE("No confirmation broadcast does not ask for confirmation, but set the ALL_STATIONS IIN bit once."))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::DontConfirm, hex::ClearRestartIIN(0));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should have ALL_STATIONS IIN set and restart IIN reset.
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C2 81 01 00");
    t.OnTxReady();

    // The next response should have ALL_STATIONS IIN reset
    t.SendToOutstation(hex::ClassPoll(3, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C3 81 00 00");
}

TEST_CASE(SUITE("Broadcast on unsupported function code should not set the ALL_STATIONS."))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::DontConfirm, hex::MeasureDelay(0));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should NOT have ALL_STATIONS IIN
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C2 81 80 00");
    t.OnTxReady();
}
