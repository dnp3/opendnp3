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

#define SUITE(name) "OutstationUnsolicitedTestSuite - " name

TEST_CASE(SUITE("sends null unsol on startup"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    OutstationTestObject t(cfg);
    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    REQUIRE(t.NumPendingTimers() == 1);
}

TEST_CASE(SUITE("Non-read during null unsol without workaround"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    cfg.params.noDefferedReadDuringUnsolicitedNullResponse = false;
    OutstationTestObject t(cfg);
    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    REQUIRE(t.lower->NumWrites() == 0);

    // send any non-read message
    t.SendToOutstation(hex::ClearRestartIIN(0));
    REQUIRE(t.lower->PopWriteAsHex() == hex::EmptyResponse(0));
    t.OnTxReady();

    // Continue sending unsolicited NULL responses
    t.AdvanceTime(TimeDuration::Seconds(5));
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(1, IINField()));
    REQUIRE(t.NumPendingTimers() == 1);
}

TEST_CASE(SUITE("Non-read during null unsol with workaround"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    cfg.params.noDefferedReadDuringUnsolicitedNullResponse = true;
    OutstationTestObject t(cfg);
    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    REQUIRE(t.lower->NumWrites() == 0);

    // send any non-read message
    t.SendToOutstation(hex::ClearRestartIIN(0));
    REQUIRE(t.lower->PopWriteAsHex() == hex::EmptyResponse(0));
    t.OnTxReady();

    // Continue sending unsolicited NULL responses
    t.AdvanceTime(TimeDuration::Seconds(5));
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(1, IINField()));
    REQUIRE(t.NumPendingTimers() == 1);
}

TEST_CASE(SUITE("Read during null unsol without workaround"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    cfg.params.noDefferedReadDuringUnsolicitedNullResponse = false;
    OutstationTestObject t(cfg);
    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    REQUIRE(t.lower->NumWrites() == 0);

    // send a read message
    t.SendToOutstation(hex::ClassPoll(0, PointClass::Class1));

    // Should not respond
    REQUIRE(t.lower->PopWriteAsHex().empty());
    REQUIRE(t.NumPendingTimers() == 1);

    // When timeout is reached, should respond to READ request
    t.AdvanceTime(TimeDuration::Seconds(5));
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
    t.OnTxReady();

    // Then continue sending unsolicited NULL responses
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(1, IINField(IINBit::DEVICE_RESTART)));
    REQUIRE(t.NumPendingTimers() == 1);
}

TEST_CASE(SUITE("Read during null unsol with workaround"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    cfg.params.noDefferedReadDuringUnsolicitedNullResponse = true;
    OutstationTestObject t(cfg);
    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    REQUIRE(t.lower->NumWrites() == 0);

    // send a read message
    t.SendToOutstation(hex::ClassPoll(0, PointClass::Class1));

    // Should respond immediatly
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
    t.OnTxReady();

    // Then continue sending unsolicited NULL responses
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(1, IINField(IINBit::DEVICE_RESTART)));
    REQUIRE(t.NumPendingTimers() == 1);
}

TEST_CASE(SUITE("UnsolConfirmTimeout"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    OutstationTestObject t(cfg);

    t.LowerLayerUp();

    // check for the startup null unsol packet, but fail the transaction
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();

    REQUIRE(t.NumPendingTimers() == 1); // confirm timer
    REQUIRE(t.AdvanceTime(TimeDuration::Seconds(5)));

    // immediately retries with new sequence #
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(1, IINField(IINBit::DEVICE_RESTART)));
}

TEST_CASE(SUITE("UnsolNumRetries"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    cfg.params.numUnsolRetries = NumRetries::Fixed(3);
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig::AllTypes(5);
    auto database = configure::by_count_of::binary_input(1);
    OutstationTestObject t(cfg, std::move(database));

    t.LowerLayerUp();

    // Confirm the unsolicited NULL response
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    // Generate an event
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true), 0); });

    // Check original unsolicited response, followed by the 3 retries
    for (unsigned int i = 0; i <= 3; ++i)
    {
        REQUIRE(t.NumPendingTimers() == 1); // confirm timer
        REQUIRE(t.AdvanceTime(TimeDuration::Seconds(5)));
        REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");
        t.OnTxReady();
    }

    // Check that it stops sending stuff
    REQUIRE(t.NumPendingTimers() == 0); // no confirm timer
    REQUIRE(t.lower->PopWriteAsHex().empty());

    // Generate another event
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(false), 0); });

    // Check seq number increment and 3 retries
    for (unsigned int i = 0; i <= 3; ++i)
    {
        REQUIRE(t.NumPendingTimers() == 1); // confirm timer
        REQUIRE(t.AdvanceTime(TimeDuration::Seconds(5)));
        REQUIRE(t.lower->PopWriteAsHex() == "F2 82 80 00 02 01 28 02 00 00 00 81 00 00 01");
        t.OnTxReady();
    }

    // Check that it stops sending stuff
    REQUIRE(t.NumPendingTimers() == 0); // no confirm timer
    REQUIRE(t.lower->PopWriteAsHex().empty());
}

TEST_CASE(SUITE("UnsolInfiniteRetries"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    cfg.params.numUnsolRetries = NumRetries::Infinite();
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig::AllTypes(5);
    auto database = configure::by_count_of::binary_input(1);
    OutstationTestObject t(cfg, std::move(database));

    t.LowerLayerUp();

    // Confirm the unsolicited NULL response
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    // Generate an event
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true), 0); });

    // Check that it keeps repeating the unsolicited response
    for (unsigned int i = 0; i < 1000; ++i)
    {
        REQUIRE(t.NumPendingTimers() == 1); // confirm timer
        REQUIRE(t.AdvanceTime(TimeDuration::Seconds(5)));
        REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");
        t.OnTxReady();
    }

    // Generate another event
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(false), 0); });

    // Check that it keeps repeating the same non-regenerated unsolicited response
    for (unsigned int i = 0; i < 1000; ++i)
    {
        REQUIRE(t.NumPendingTimers() == 1); // confirm timer
        REQUIRE(t.AdvanceTime(TimeDuration::Seconds(5)));
        REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");
        t.OnTxReady();
    }
}

TEST_CASE(SUITE("UnsolData"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField::AllEventClasses(); // allows us to skip the "enable unsol" step
    cfg.eventBufferConfig = EventBufferConfig::AllTypes(5);

    auto database = configure::by_count_of::binary_input(3);
    database.binary_input[0].clazz = PointClass::Class1;
    database.binary_input[1].clazz = PointClass::Class2;
    database.binary_input[2].clazz = PointClass::Class3;

    OutstationTestObject t(cfg, std::move(database));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    // Check that the confirm is reported to the IOutstationApplication
    REQUIRE(t.application->confirms.size() == 1);
    REQUIRE(t.application->confirms[0].is_unsolicited == true);
    REQUIRE(t.application->confirms[0].num_class1 == 0);
    REQUIRE(t.application->confirms[0].num_class2 == 0);
    REQUIRE(t.application->confirms[0].num_class3 == 0);

    // do a transaction before the layer comes online to prove that the null transaction
    // is occuring before unsol data is sent
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01)), 2); });

    // should immediately try to send another unsol packet,
    // Grp2Var1, qual 0x17, count 1, index 2, quality+val == 0x01
    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 02 00 01");
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(1));
    REQUIRE(t.lower->PopWriteAsHex().empty());

    // Check that the confirm is reported to the IOutstationApplication
    REQUIRE(t.application->confirms.size() == 2);
    REQUIRE(t.application->confirms[1].is_unsolicited == true);
    REQUIRE(t.application->confirms[1].num_class1 == 0);
    REQUIRE(t.application->confirms[1].num_class2 == 0);
    REQUIRE(t.application->confirms[1].num_class3 == 0);
}

TEST_CASE(SUITE("UnsolEventBufferOverflow"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField(PointClass::Class1);
    cfg.eventBufferConfig = EventBufferConfig(2);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(1));

    t.LowerLayerUp();
    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Binary(true, Flags(0x01)), 0);
        db.Update(Binary(false, Flags(0x01)), 0);
        db.Update(Binary(true, Flags(0x01)), 0);
    });

    // should immediately try to send 2 unsol events
    // Grp2Var1, qual 0x17, count 2, index 0
    // The last two values should be published, 0x01 and 0x81 (false and true)
    // the first value is lost off the front of the buffer
    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 08 02 01 28 02 00 00 00 01 00 00 81");
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(1));

    // Check that the confirm is reported to the IOutstationApplication
    REQUIRE(t.application->confirms.size() == 2);
    REQUIRE(t.application->confirms[1].is_unsolicited == true);
    REQUIRE(t.application->confirms[1].num_class1 == 0);
    REQUIRE(t.application->confirms[1].num_class2 == 0);
    REQUIRE(t.application->confirms[1].num_class3 == 0);

    REQUIRE(t.lower->PopWriteAsHex().empty());
}

TEST_CASE(SUITE("UnsolMultiFragments"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.maxTxFragSize = 20;                             // this will cause the unsol response to get fragmented
    cfg.params.unsolClassMask = ClassField::AllEventClasses(); // this allows the EnableUnsol sequence to be skipped
    cfg.eventBufferConfig = EventBufferConfig(0, 0, 5);
    OutstationTestObject t(cfg, configure::by_count_of::analog_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));
    REQUIRE(t.lower->PopWriteAsHex().empty());

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Analog(7, Flags(0x01)), 1);
        db.Update(Analog(13, Flags(0x01)), 3);
    });

    // Only enough room to in the APDU to carry a single value
    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 82 00 20 01 28 01 00 01 00 01 07 00 00 00");
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(1));

    // Check that the confirm is reported to the IOutstationApplication
    REQUIRE(t.application->confirms.size() == 2);
    REQUIRE(t.application->confirms[1].is_unsolicited == true);
    REQUIRE(t.application->confirms[1].num_class1 == 1);
    REQUIRE(t.application->confirms[1].num_class2 == 0);
    REQUIRE(t.application->confirms[1].num_class3 == 0);

    // should immediately try to send another unsol packet
    REQUIRE(t.lower->PopWriteAsHex() == "F2 82 80 00 20 01 28 01 00 03 00 01 0D 00 00 00");
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(2));

    REQUIRE(t.lower->PopWriteAsHex().empty());

    // Check that the confirm is reported to the IOutstationApplication
    REQUIRE(t.application->confirms.size() == 3);
    REQUIRE(t.application->confirms[2].is_unsolicited == true);
    REQUIRE(t.application->confirms[2].num_class1 == 0);
    REQUIRE(t.application->confirms[2].num_class2 == 0);
    REQUIRE(t.application->confirms[2].num_class3 == 0);
}

void WriteDuringUnsol(bool beforeTx)
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true, Flags(0x01)), 0); });

    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");

    if (beforeTx)
    {
        t.SendToOutstation(hex::ClearRestartIIN(0));
        t.OnTxReady();
    }
    else
    {
        t.OnTxReady();
        t.SendToOutstation(hex::ClearRestartIIN(0));
    }

    // check that we get a response to this immediately without the confirm
    REQUIRE(t.lower->PopWriteAsHex() == hex::EmptyResponse(0));

    // now send the confirm to the outstation
    t.SendToOutstation(hex::UnsolConfirm(1));
    REQUIRE(t.lower->PopWriteAsHex().empty());
}

// Test that non-read fragments are immediately responded to while
// waiting for a response to unsolicited data
TEST_CASE(SUITE("WriteDuringUnsolBeforeTx"))
{
    WriteDuringUnsol(true);
}

TEST_CASE(SUITE("WriteDuringUnsolAfterTx"))
{
    WriteDuringUnsol(false);
}

TEST_CASE(SUITE("ReadDuringUnsolConfirm"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true, Flags(0x01)), 0); });

    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");

    t.OnTxReady();
    t.SendToOutstation("C0 01 3C 02 06");

    t.SendToOutstation(hex::UnsolConfirm(1));

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
}

TEST_CASE(SUITE("ReadDuringUnsolWithoutConfirm"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig(5);
    cfg.params.unsolConfirmTimeout = TimeDuration::Seconds(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true, Flags(0x01)), 0); });

    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");
    t.OnTxReady();

    // Send read request during the unsolicited
    t.SendToOutstation("C0 01 3C 02 06");

    // Outstation should not respond
    REQUIRE(t.lower->PopWriteAsHex().empty());

    // Timeout unsolicited response
    t.AdvanceTime(TimeDuration::Seconds(5));

    // Outstation should immediatly answer to the READ request with the event
    REQUIRE(t.lower->PopWriteAsHex() == "E0 81 80 00 02 01 28 01 00 00 00 81");
}

TEST_CASE(SUITE("ReadWriteDuringUnsol"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == "F0 82 80 00");
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true, Flags(0x01)), 0); });

    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 81");
    t.OnTxReady();

    // send a read request that will be overwritten
    t.SendToOutstation("C0 01 3C 01 06");

    // now send a write IIN request, and test that the outstation answers immediately
    t.SendToOutstation("C0 02 50 01 00 07 07 00");

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 00 00");
}

TEST_CASE(SUITE("RepeatRequestDuringUnsol"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.params.unsolClassMask = ClassField::AllEventClasses();
    cfg.eventBufferConfig = EventBufferConfig(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == "F0 82 80 00");
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    t.SendToOutstation(hex::ClearRestartIIN(0));
    REQUIRE(t.lower->PopWriteAsHex() == hex::EmptyResponse(0));
    t.OnTxReady();

    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true, Flags(0x01)), 1); });
    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 00 00 02 01 28 01 00 01 00 81");
    t.OnTxReady();

    // while waiting for a confirm, repeat the previous request, byte-for-byte
    t.SendToOutstation(hex::ClearRestartIIN(0));
    REQUIRE(t.lower->PopWriteAsHex() == hex::EmptyResponse(0));
    t.OnTxReady();
}

TEST_CASE(SUITE("UnsolEnable"))
{
    OutstationConfig cfg;
    cfg.params.allowUnsolicited = true;
    cfg.eventBufferConfig = EventBufferConfig(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();

    REQUIRE(t.lower->PopWriteAsHex() == hex::NullUnsolicited(0));
    t.OnTxReady();
    t.SendToOutstation(hex::UnsolConfirm(0));

    // do a transaction to show that unsol data is not being reported yet
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01)), 0); });

    REQUIRE(t.lower->PopWriteAsHex().empty());

    // Enable unsol class 1
    t.SendToOutstation("C0 14 3C 02 06");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 82 00");
    t.OnTxReady();

    // should automatically send the previous data as unsol
    REQUIRE(t.lower->PopWriteAsHex() == "F1 82 80 00 02 01 28 01 00 00 00 01");
}

TEST_CASE(SUITE("UnsolEnableDisableFailure"))
{
    OutstationConfig cfg;
    cfg.eventBufferConfig = EventBufferConfig(5);
    OutstationTestObject t(cfg, configure::by_count_of::binary_input(5));

    t.LowerLayerUp();
    t.SendToOutstation("C0 14 3C 02 06");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 01"); // FUNC_NOT_SUPPORTED
}
