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

#include <ser4cpp/util/HexConversions.h>

#include <dnp3mocks/DatabaseHelpers.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "OutstationBroadcastTestSuite - " name

TEST_CASE(SUITE("ShallConfirm broadcast asks for confirmation and resets on confirm"))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::ShallConfirm, hex::ClearRestartIIN(0));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should have BROADCAST IIN set, and should ask for confirmation
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "E2 81 01 00");
    t.OnTxReady();

    // The next response should have BROADCAST IIN set, and should ask for confirmation
    t.SendToOutstation(hex::ClassPoll(3, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "E3 81 01 00");
    t.OnTxReady();

    // Confirm the last response
    t.SendToOutstation(hex::SolicitedConfirm(3));
    t.OnTxReady();

    // Check that no confirmation is asked and that BROADCAST IIN is reset
    t.SendToOutstation(hex::ClassPoll(4, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C4 81 00 00");
}

TEST_CASE(SUITE("Confirmation sequence number is properly updated even if the response is not sequence related"))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::ShallConfirm, hex::ClearRestartIIN(0));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should have BROADCAST IIN set, and should ask for confirmation
    t.SendToOutstation(hex::RecordCurrentTime(2));
    REQUIRE(t.lower->PopWriteAsHex() == "E2 81 01 00");
    t.OnTxReady();

    // Confirm the last response
    t.SendToOutstation(hex::SolicitedConfirm(2));
    t.OnTxReady();

    // Check that no confirmation is asked and that BROADCAST IIN is reset
    t.SendToOutstation(hex::ClassPoll(3, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C3 81 00 00");
}

TEST_CASE(SUITE("DontConfirm broadcast does not ask for confirmation, but set the BROADCAST IIN bit once"))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::DontConfirm, hex::ClearRestartIIN(0));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should have BROADCAST IIN set and restart IIN reset.
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C2 81 01 00");
    t.OnTxReady();

    // The next response should have BROADCAST IIN reset
    t.SendToOutstation(hex::ClassPoll(3, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C3 81 00 00");
}

TEST_CASE(SUITE("Receiving a broadcast should assume confirmation failed"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);
    OutstationTestObject t(config, configure::by_count_of::binary_input(1));
    t.LowerLayerUp();

    // Generate an event
    t.Transaction([](IUpdateHandler& handler) { handler.Update(Binary(true), 0); });

    // When reading, check that we receive a response with a confirmation
    t.SendToOutstation(hex::ClassPoll(0, PointClass::Class1));
    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "E0 81 80 00");
    t.OnTxReady();

    // Send a broadcast message
    t.BroadcastToOutstation(LinkBroadcastAddress::DontConfirm, hex::ClearRestartIIN(1));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // Try confirming
    t.SendToOutstation(hex::SolicitedConfirm(0));
    t.OnTxReady();

    // When reading, check BROADCAST IIN, check that the events are still pending
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "C2 81 03 00");
    t.OnTxReady();
}

TEST_CASE(SUITE("Broadcast with malformed request should still set the BROADCAST IIN"))
{
    OutstationConfig config;
    OutstationTestObject t(config, DatabaseConfig());
    t.LowerLayerUp();

    t.BroadcastToOutstation(LinkBroadcastAddress::DontConfirm, "FF FF FF");

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // The next response should have BROADCAST IIN
    t.SendToOutstation(hex::ClassPoll(2, PointClass::Class0));
    REQUIRE(t.lower->PopWriteAsHex() == "C2 81 81 00");
    t.OnTxReady();
}

TEST_CASE(SUITE("Unsolicited responses should advertise BROADCAST IIN"))
{
    OutstationConfig config;
    config.params.unsolClassMask = ClassField::AllClasses();
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);
    config.params.allowUnsolicited = true;
    config.params.numUnsolRetries = NumRetries::Fixed(0);
    OutstationTestObject t(config, configure::by_count_of::binary_input(1));
    t.LowerLayerUp();
    t.context.OnTxReady();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));

    // Confirm unsolicited NULL response
    t.SendToOutstation(hex::UnsolConfirm(0));

    // Send a broadcast message
    t.BroadcastToOutstation(LinkBroadcastAddress::DontConfirm, hex::ClearRestartIIN(1));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // Generate an event
    t.Transaction([](IUpdateHandler& handler) { handler.Update(Binary(true), 0); });

    // Should send unsolicited with BROADCAST IIN set
    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "F1 82 01 00");
    t.context.OnTxReady();

    // Timeout and generate another unsolicited
    t.AdvanceToNextTimer();
    t.Transaction([](IUpdateHandler& handler) { handler.Update(Binary(false), 0); });

    // Next unsolicited shouldn't have BROADCAST IIN set
    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "F2 82 00 00");
    t.context.OnTxReady();
}

TEST_CASE(SUITE("ShallConfirm: Unsolicited responses should clear BROADCAST when confirm is received"))
{
    OutstationConfig config;
    config.params.unsolClassMask = ClassField::AllClasses();
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);
    config.params.allowUnsolicited = true;
    config.params.numUnsolRetries = NumRetries::Fixed(0);
    OutstationTestObject t(config, configure::by_count_of::binary_input(1));
    t.LowerLayerUp();
    t.context.OnTxReady();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));

    // Confirm unsolicited NULL response
    t.SendToOutstation(hex::UnsolConfirm(0));

    // Send a broadcast message
    t.BroadcastToOutstation(LinkBroadcastAddress::ShallConfirm, hex::ClearRestartIIN(1));

    // Outstation should not respond to broadcast request
    REQUIRE(t.lower->HasNoData());

    // Generate an event
    t.Transaction([](IUpdateHandler& handler) { handler.Update(Binary(true), 0); });

    // Should send unsolicited with BROADCAST IIN set
    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "F1 82 01 00");
    t.context.OnTxReady();

    // Timeout and generate another unsolicited
    t.AdvanceToNextTimer();
    t.Transaction([](IUpdateHandler& handler) { handler.Update(Binary(false), 0); });

    // Next unsolicited should still have BROADCAST IIN set
    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "F2 82 01 00");
    t.context.OnTxReady();

    // Confirm the unsolicited response
    t.SendToOutstation(hex::UnsolConfirm(2));

    // Generate an event
    t.Transaction([](IUpdateHandler& handler) { handler.Update(Binary(true), 0); });

    REQUIRE(t.lower->PopWriteAsHex().substr(0, 11) == "F3 82 00 00");
}
