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

#include <functional>

using namespace opendnp3;

#define SUITE(name) "OutstationEventResponsesTestSuite - " name

TEST_CASE(SUITE("empty response when to class 1 when no events available"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 01 3C 02 06"); // Read class 1
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
}

TEST_CASE(SUITE("ReceiveNewRequestSolConfirmWait"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);
    OutstationTestObject t(config, configure::by_count_of::binary_input(1));
    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true, Flags(0x01)), 0); });

    t.SendToOutstation(hex::ClassPoll(0, PointClass::Class1));
    REQUIRE(t.lower->PopWriteAsHex() == "E0 81 80 00 02 01 28 01 00 00 00 81");
    t.OnTxReady();

    t.SendToOutstation(hex::ClassPoll(1, PointClass::Class1));
    REQUIRE(t.lower->PopWriteAsHex() == "E1 81 80 00 02 01 28 01 00 00 00 81");
}

TEST_CASE(SUITE("ReadClass1WithSOE"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);
    OutstationTestObject t(config, configure::by_count_of::all_types(100));

    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Analog(0x1234, Flags(0x01)), 0x17); // 0x 12 34 00 00 in little endian
        db.Update(Binary(true, Flags(0x01)), 0x10);
        db.Update(Analog(0x2222, Flags(0x01)), 0x17); // 0x 22 22 00 00 in little endian
    });

    t.SendToOutstation(hex::ClassPoll(0, PointClass::Class1));
    REQUIRE(t.lower->PopWriteAsHex()
            == "E0 81 80 00 20 01 28 01 00 17 00 01 34 12 00 00 02 01 28 01 00 10 00 81 20 01 28 01 00 17 00 01 22 22 "
               "00 00");
    t.OnTxReady();
    t.SendToOutstation(hex::SolicitedConfirm(0));

    t.SendToOutstation(hex::ClassPoll(1, PointClass::Class1)); // Repeat read class 1
    REQUIRE(t.lower->PopWriteAsHex() == "C1 81 80 00");        // Buffer should have been cleared
}

TEST_CASE(SUITE("ReadClass1WithSOEWithTimeoutInBetween"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);
    config.params.solConfirmTimeout = TimeDuration::Seconds(5);
    config.params.unsolConfirmTimeout = TimeDuration::Seconds(10);
    OutstationTestObject t(config, configure::by_count_of::all_types(100));

    t.LowerLayerUp();

    // Generate event
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(true), 0); });

    // Read and expect the event
    t.SendToOutstation(hex::ClassPoll(0, PointClass::Class1));
    REQUIRE(t.lower->PopWriteAsHex() == "E0 81 80 00 02 01 28 01 00 00 00 81");
    t.OnTxReady();

    // Wait for timeout
    t.AdvanceTime(TimeDuration::Seconds(5));

    // Send confirmation too late
    t.SendToOutstation(hex::SolicitedConfirm(0));

    // Generate another event
    t.Transaction([](IUpdateHandler& db) { db.Update(Binary(false), 0); });

    // Read again and expect two events
    t.SendToOutstation(hex::ClassPoll(1, PointClass::Class1));
    REQUIRE(t.lower->PopWriteAsHex() == "E1 81 80 00 02 01 28 02 00 00 00 81 00 00 01");
}

TEST_CASE(SUITE("EventBufferOverflowAndClear"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(2);
    OutstationTestObject t(config, configure::by_count_of::all_types(100));

    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Binary(true, Flags(0x01)), 0); // this event is lost in the overflow
        db.Update(Binary(true, Flags(0x01)), 1);
        db.Update(Binary(true, Flags(0x01)), 2);
    });

    t.SendToOutstation("C0 01");
    REQUIRE("C0 81 82 08" == t.lower->PopWriteAsHex());
    t.OnTxReady();

    t.SendToOutstation("C1 01 3C 02 07 01"); // class 1, count of 1
    REQUIRE("E1 81 82 08 02 01 28 01 00 01 00 81" == t.lower->PopWriteAsHex());
    t.OnTxReady();
    t.SendToOutstation(hex::SolicitedConfirm(1));

    // Check that the confirm is reported to the IOutstationApplication
    REQUIRE(t.application->confirms.size() == 1);
    REQUIRE(t.application->confirms[0].is_unsolicited == false);
    REQUIRE(t.application->confirms[0].num_class1 == 1);
    REQUIRE(t.application->confirms[0].num_class2 == 0);
    REQUIRE(t.application->confirms[0].num_class3 == 0);

    t.SendToOutstation("C0 01");
    REQUIRE("C0 81 82 00" == t.lower->PopWriteAsHex());
}

TEST_CASE(SUITE("MultipleClasses"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);

    auto database = configure::by_count_of::all_types(100);
    database.binary_input[0].clazz = PointClass::Class1;
    database.analog_input[0].clazz = PointClass::Class2;
    database.counter[0].clazz = PointClass::Class3;

    OutstationTestObject t(config, std::move(database));
    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Binary(true), 0);
        db.Update(Analog(3), 0);
        db.Update(Counter(7), 0);
    });

    t.SendToOutstation("C0 01");                        // empty READ
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 8E 00"); // all event bits set + restart
    t.OnTxReady();

    // ------ read 1 event at a time by class, until all events are gone ----

    t.SendToOutstation(hex::ClassPoll(1, PointClass::Class2));                              // Class 2
    REQUIRE(t.lower->PopWriteAsHex() == "E1 81 8A 00 20 01 28 01 00 00 00 01 03 00 00 00"); // restart + Class 1/3
    t.OnTxReady();
    t.SendToOutstation("C1 00");

    t.SendToOutstation("C2 01 3C 04 06");                                                   // Class 3
    REQUIRE(t.lower->PopWriteAsHex() == "E2 81 82 00 16 01 28 01 00 00 00 01 07 00 00 00"); // restart + Class 1/3
    t.OnTxReady();
    t.SendToOutstation("C2 00");

    t.SendToOutstation("C3 01 3C 02 06");                                       // Class 1
    REQUIRE(t.lower->PopWriteAsHex() == "E3 81 80 00 02 01 28 01 00 00 00 81"); // restart only
    t.OnTxReady();
    t.SendToOutstation("C3 00");

    t.SendToOutstation("C4 01");                        // empty READ
    REQUIRE(t.lower->PopWriteAsHex() == "C4 81 80 00"); // restart only
}

void TestEventRead(const std::string& request,
                   const std::string& response,
                   const std::function<void(IUpdateHandler& db)>& loadFun,
                   const std::function<void(DatabaseConfig& db)>& configure = [](DatabaseConfig& view) {})
{

    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(10);

    DatabaseConfig database = configure::by_count_of::all_types(5);
    configure(database);

    OutstationTestObject t(config, std::move(database));

    t.LowerLayerUp();

    t.Transaction([&](IUpdateHandler& db) { loadFun(db); });

    t.SendToOutstation(request);
    REQUIRE(t.lower->PopWriteAsHex() == response);
}

TEST_CASE(SUITE("Class1"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01)), 0); };

    TestEventRead(hex::ClassPoll(0, PointClass::Class1), "E0 81 80 00 02 01 28 01 00 00 00 01", update);
}

TEST_CASE(SUITE("Class1OneByteLimitedCount"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01)), 2);
        db.Update(Binary(true, Flags(0x01)), 1);
    };

    // reads a single event, IIB bit indicates more events present
    TestEventRead("C0 01 3C 02 07 01", "E0 81 82 00 02 01 28 01 00 02 00 01", update);
}

TEST_CASE(SUITE("Class1TwoByteLimitedCount"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01)), 2);
        db.Update(Binary(true, Flags(0x01)), 1);
        db.Update(Binary(true, Flags(0x01)), 3);
    };

    // reads a single event, IIB bit indicates more events present
    TestEventRead("C0 01 3C 02 08 02 00", "E0 81 82 00 02 01 28 02 00 02 00 01 01 00 81", update);
}

TEST_CASE(SUITE("MixedClassLimitedCount"))
{
    auto configure = [](DatabaseConfig& db) {
        db.binary_input[0].clazz = PointClass::Class1;
        db.binary_input[1].clazz = PointClass::Class2;
        db.binary_input[2].clazz = PointClass::Class3;
    };

    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01)), 0);

        db.Update(Binary(true, Flags(0x01)), 1);
        db.Update(Binary(false, Flags(0x01)), 1);

        db.Update(Binary(true, Flags(0x01)), 2);
        db.Update(Binary(false, Flags(0x01)), 2);
    };

    // read 1 class 2 event and 1 class 3 event, 1 of each event type left in buffer
    // get back two g2v1 events in the same header for indices 1 & 2
    TestEventRead("C0 01 3C 03 07 01 3C 04 07 01", "E0 81 8E 00 02 01 28 02 00 01 00 81 02 00 81", update, configure);
}

TEST_CASE(SUITE("reports g22v5 correctly"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Counter(23, Flags(0x01), DNPTime(1512595515000)), 0); };

    auto configure = [](DatabaseConfig& db) { db.counter[0].evariation = EventCounterVariation::Group22Var5; };

    TestEventRead("C0 01 3C 02 06", "E0 81 80 00 16 05 28 01 00 00 00 01 17 00 00 00 78 E6 B7 2D 60 01", update,
                  configure);
}

TEST_CASE(SUITE("reports g22v6 correctly"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Counter(23, Flags(0x01), DNPTime(1512595515000)), 0); };

    auto configure = [](DatabaseConfig& db) { db.counter[0].evariation = EventCounterVariation::Group22Var6; };

    TestEventRead("C0 01 3C 02 06", "E0 81 80 00 16 06 28 01 00 00 00 01 17 00 78 E6 B7 2D 60 01", update, configure);
}

TEST_CASE(SUITE("ReadGrp2Var0"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01)), 0); };

    TestEventRead("C0 01 02 00 06", "E0 81 80 00 02 01 28 01 00 00 00 01", update);
}

TEST_CASE(SUITE("ReadGrp22Var0"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Counter(0, Flags(0x01)), 0); };

    TestEventRead("C0 01 16 00 06", "E0 81 80 00 16 01 28 01 00 00 00 01 00 00 00 00", update);
}

TEST_CASE(SUITE("ReadGrp32Var0"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Analog(0.0, Flags(0x01)), 0); };

    TestEventRead("C0 01 20 00 06", "E0 81 80 00 20 01 28 01 00 00 00 01 00 00 00 00", update);
}

TEST_CASE(SUITE("ReadGrp32Var7"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Analog(0.0, Flags(0x01), DNPTime(0x010203040506)), 0); };

    const auto response = "E0 81 80 00 20 07 28 01 00 00 00 01 00 00 00 00 06 05 04 03 02 01";

    // specifically read this variation, but let the default be something else
    TestEventRead("C0 01 20 07 06", response, update,
                  [](DatabaseConfig& db) { db.analog_input[0].evariation = EventAnalogVariation::Group32Var1; });

    // configure this as the default variation and ask for variation 0
    TestEventRead("C0 01 20 00 06", response, update,
                  [](DatabaseConfig& db) { db.analog_input[0].evariation = EventAnalogVariation::Group32Var7; });
}

TEST_CASE(SUITE("ReadGrp32Var5"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Analog(0.0, Flags(0x01), DNPTime(0x010203040506)), 0); };

    const auto response = "E0 81 80 00 20 05 28 01 00 00 00 01 00 00 00 00";

    // specifically read this variation, but let the default be something else
    TestEventRead("C0 01 20 05 06", response, update,
                  [](DatabaseConfig& db) { db.analog_input[0].evariation = EventAnalogVariation::Group32Var1; });

    // configure this as the default variation and ask for variation 0
    TestEventRead("C0 01 20 00 06", response, update,
                  [](DatabaseConfig& db) { db.analog_input[0].evariation = EventAnalogVariation::Group32Var5; });
}

TEST_CASE(SUITE("ReadGrp2Var1"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01)), 3); };

    TestEventRead("C0 01 02 01 06", "E0 81 80 00 02 01 28 01 00 03 00 01", update); // 1 byte count == 1, ONLINE quality
}

TEST_CASE(SUITE("ReadGrp2Var1LimitedCount"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01)), 3);
        db.Update(Binary(true, Flags(0x01)), 2);
        db.Update(Binary(false, Flags(0x01)), 1);
    };

    // read 2 events only, Class 1 data IIN still set
    TestEventRead("C0 01 02 01 07 02", "E0 81 82 00 02 01 28 02 00 03 00 01 02 00 81",
                  update); // 1 byte count == 1, ONLINE quality
}

TEST_CASE(SUITE("ReadGrp2Var2"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01), DNPTime(0x4571)), 3); };

    TestEventRead("C0 01 02 02 06", "E0 81 80 00 02 02 28 01 00 03 00 01 71 45 00 00 00 00",
                  update); // 1 byte count == 1, ONLINE quality
}

TEST_CASE(SUITE("ReadGrp2Var3SingleValue"))
{
    auto update = [](IUpdateHandler& db) { db.Update(Binary(false, Flags(0x01), DNPTime(0x4571, TimestampQuality::UNSYNCHRONIZED)), 3); };

    TestEventRead("C0 01 02 03 06", "E0 81 80 00 33 02 07 01 71 45 00 00 00 00 02 03 28 01 00 03 00 01 00 00", update);
}

TEST_CASE(SUITE("ReadGrp2Var3TwoValues"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01), DNPTime(0x4571, TimestampQuality::SYNCHRONIZED)), 3);
        db.Update(Binary(true, Flags(0x01), DNPTime(0x4579, TimestampQuality::SYNCHRONIZED)), 4);
    };

    auto rsp = "E0 81 80 00 33 01 07 01 71 45 00 00 00 00 02 03 28 02 00 03 00 01 00 00 04 00 81 08 00";

    TestEventRead("C0 01 02 03 06", rsp, update);
}

TEST_CASE(SUITE("ReadGrp2Var3TwoValuesNegativeDifference"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01), DNPTime(0x4571, TimestampQuality::SYNCHRONIZED)), 3);
        db.Update(Binary(true, Flags(0x01), DNPTime(0x4570, TimestampQuality::SYNCHRONIZED)), 4);
    };

    std::string header = "E0 81 80 00";
    std::string cto1 = " 33 01 07 01 71 45 00 00 00 00 02 03 28 01 00 03 00 01 00 00";
    std::string cto2 = " 33 01 07 01 70 45 00 00 00 00 02 03 28 01 00 04 00 81 00 00";

    auto rsp = header + cto1 + cto2;

    TestEventRead("C0 01 02 03 06", rsp, update);
}

TEST_CASE(SUITE("ReadGrp2Var3TwoValuesDifferenceTooBigForCTO"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01), DNPTime(0x000000, TimestampQuality::SYNCHRONIZED)), 3);
        db.Update(Binary(true, Flags(0x01), DNPTime(0x010000, TimestampQuality::SYNCHRONIZED)), 4);
    };

    std::string header = "E0 81 80 00";
    std::string cto1 = " 33 01 07 01 00 00 00 00 00 00 02 03 28 01 00 03 00 01 00 00";
    std::string cto2 = " 33 01 07 01 00 00 01 00 00 00 02 03 28 01 00 04 00 81 00 00";

    auto rsp = header + cto1 + cto2;

    TestEventRead("C0 01 02 03 06", rsp, update);
}

TEST_CASE(SUITE("ReadGrp2Var3TwoValuesWithDifferentTimeQuality"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01), DNPTime(0x4571, TimestampQuality::SYNCHRONIZED)), 3);
        db.Update(Binary(true, Flags(0x01), DNPTime(0x4579, TimestampQuality::UNSYNCHRONIZED)), 4);
    };

    std::string header = "E0 81 80 00";
    std::string cto1 = " 33 01 07 01 71 45 00 00 00 00 02 03 28 01 00 03 00 01 00 00";
    std::string cto2 = " 33 02 07 01 79 45 00 00 00 00 02 03 28 01 00 04 00 81 00 00";

    auto rsp = header + cto1 + cto2;

    TestEventRead("C0 01 02 03 06", rsp, update);
}

TEST_CASE(SUITE("ReadGrp2Var3TwoValuesWithDifferentInvalidOrUnsynchronizedAreReportedTogether"))
{
    auto update = [](IUpdateHandler& db) {
        db.Update(Binary(false, Flags(0x01), DNPTime(0x4571, TimestampQuality::UNSYNCHRONIZED)), 3);
        db.Update(Binary(true, Flags(0x01), DNPTime(0x4579, TimestampQuality::INVALID)), 4);
    };

    auto rsp = "E0 81 80 00 33 02 07 01 71 45 00 00 00 00 02 03 28 02 00 03 00 01 00 00 04 00 81 08 00";

    TestEventRead("C0 01 02 03 06", rsp, update);
}

TEST_CASE(SUITE("reports octet string events w/ same size in same header"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(5);
    OutstationTestObject t(config, configure::by_count_of::octet_string(5));
    t.LowerLayerUp();

    auto update = [](IUpdateHandler& db) {
        uint8_t bytes[2] = {0xCA, 0xFE};
        OctetString data(opendnp3::Buffer(bytes, 2));

        db.Update(data, 2);
        db.Update(data, 1);
    };

    t.Transaction(update);

    t.SendToOutstation("C0 01 3C 02 06"); // Read class 1
    REQUIRE(t.lower->PopWriteAsHex() == "E0 81 80 00 6F 02 28 02 00 02 00 CA FE 01 00 CA FE");
}

TEST_CASE(SUITE("reports octet string events w/ different sizes in separate headers"))
{
    OutstationConfig config;
    config.eventBufferConfig = EventBufferConfig::AllTypes(5);
    OutstationTestObject t(config, configure::by_count_of::octet_string(5));
    t.LowerLayerUp();

    auto update = [](IUpdateHandler& db) {
        {
            uint8_t bytes[2] = {0xCA, 0xFE};
            OctetString data(opendnp3::Buffer(bytes, 2));
            db.Update(data, 0);
        }
        {
            uint8_t bytes[4] = {0xCA, 0xFE, 0xBA, 0xBE};
            OctetString data(opendnp3::Buffer(bytes, 4));
            db.Update(data, 0);
        }
    };

    t.Transaction(update);

    t.SendToOutstation("C0 01 3C 02 06"); // Read class 1
    REQUIRE(t.lower->PopWriteAsHex() == "E0 81 80 00 6F 02 28 01 00 00 00 CA FE 6F 04 28 01 00 00 00 CA FE BA BE");
}
