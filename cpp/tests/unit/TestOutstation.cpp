/*
 )* Copyright 2013-2020 Automatak, LLC
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
using namespace ser4cpp;

#define SUITE(name) "OutstationTestSuite - " name

TEST_CASE(SUITE("UnsupportedFunction"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 10");                        // func = initialize application (16)
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + func not supported
}

TEST_CASE(SUITE("ApplicationIINBits"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.application->appIIN.deviceTrouble = true;
    t.application->appIIN.localControl = true;
    t.application->appIIN.configCorrupt = true;
    t.application->appIIN.needTime = true;

    t.SendToOutstation("C0 01"); // blank read
    // All 4 bits + restart
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 F0 20");
}

TEST_CASE(SUITE("ReadUnknownObject"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    // from the conformance tests, respond with IIN 2-1
    t.SendToOutstation("C0 01 00 00 06");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 02"); // IIN = device restart + unknown object
}

TEST_CASE(SUITE("ColdRestart"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    // try first with support turned off
    t.SendToOutstation("C0 0D");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + function not supported
    t.OnTxReady();

    t.application->coldRestartSupport = RestartMode::SUPPORTED_DELAY_FINE;
    t.application->coldRestartTimeDelay = 1;

    t.SendToOutstation("C1 0D");
    REQUIRE(t.lower->PopWriteAsHex() == "C1 81 80 00 34 02 07 01 01 00");
}

TEST_CASE(SUITE("WarmRestart"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    // try first with support turned off
    t.SendToOutstation("C0 0E");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + function not supported
    t.OnTxReady();

    t.application->warmRestartSupport = RestartMode::SUPPORTED_DELAY_COARSE;
    t.application->warmRestartTimeDelay = 65535;

    t.SendToOutstation("C1 0E");
    REQUIRE(t.lower->PopWriteAsHex() == "C1 81 80 00 34 01 07 01 FF FF");
}

TEST_CASE(SUITE("NoResponseToNoAckCodes"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    // outstation shouldn't respond to any of these
    std::vector<FunctionCode> codes;
    codes.push_back(FunctionCode::DIRECT_OPERATE_NR);
    codes.push_back(FunctionCode::FREEZE_AT_TIME_NR);
    codes.push_back(FunctionCode::FREEZE_CLEAR_NR);
    codes.push_back(FunctionCode::FREEZE_AT_TIME_NR);

    uint8_t sequence = 0;

    for (auto code : codes)
    {
        uint8_t bytes[2];
        AppControlField control(true, true, false, false, sequence);
        bytes[0] = control.ToByte();
        bytes[1] = static_cast<uint8_t>(code);
        auto request = HexConversions::to_hex(bytes, 2, true);

        t.SendToOutstation(request);
        REQUIRE(t.lower->PopWriteAsHex().empty());

        ++sequence;
    }
}

TEST_CASE(SUITE("WriteIIN"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation(hex::ClearRestartIIN(0));
    REQUIRE(t.lower->PopWriteAsHex() == hex::EmptyResponse(0));
}

TEST_CASE(SUITE("WriteIINEnabled"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 02 50 01 00 07 07 01");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 04");
}

TEST_CASE(SUITE("WriteIINWrongBit"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 02 50 01 00 04 04 01");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 04");
}

TEST_CASE(SUITE("WriteNonWriteObject"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 02 01 02 00 07 07 00");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 01");
}

TEST_CASE(SUITE("DelayMeasure"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 17"); // delay measure
    REQUIRE(t.lower->PopWriteAsHex()
            == "C0 81 80 00 34 02 07 01 00 00"); // response, Grp51Var2, count 1, value == 00 00
}

TEST_CASE(SUITE("DelayMeasureExtraData"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 17 DE AD BE EF");            // delay measure
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 04"); // param error
}

TEST_CASE(SUITE("WriteTimeDate - NonLAN"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C1 02 32 01 07 01 D2 04 00 00 00 00"); // write Grp50Var1, value = 1234 ms after epoch
    REQUIRE(t.lower->PopWriteAsHex() == "C1 81 80 00");
    REQUIRE(t.application->timestamps.size() == 1);
    REQUIRE(t.application->timestamps.front().msSinceEpoch == 1234);
}

TEST_CASE(SUITE("WriteTimeDateLAN - Rejects write without prior record current time"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C1 02 32 03 07 01 D2 04 00 00 00 00"); // write Grp50Var3, value = 1234 ms after epoch
    REQUIRE(t.lower->PopWriteAsHex() == "C1 81 80 04");        // param error
}

TEST_CASE(SUITE("WriteTimeDateLAN - Rejects write with bad sequence number"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 18"); // record current time
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
    t.OnTxReady();

    t.SendToOutstation("C2 02 32 03 07 01 D2 04 00 00 00 00"); // write Grp50Var3, value = 1234 ms after epoch
    REQUIRE(t.lower->PopWriteAsHex() == "C2 81 80 04");        // param error
}

TEST_CASE(SUITE("WriteTimeDateLAN - Accepts write with correct sequence number"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.AdvanceTime(TimeDuration::Milliseconds(10));
    t.SendToOutstation("C0 18"); // record current time
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
    t.OnTxReady();

    t.AdvanceTime(TimeDuration::Milliseconds(20));
    t.SendToOutstation("C1 02 32 03 07 01 D2 04 00 00 00 00"); // write Grp50Var3, value = 1234 ms after epoch
    REQUIRE(t.lower->PopWriteAsHex() == "C1 81 80 00");        // param error
    REQUIRE(t.application->timestamps.size() == 1);
    REQUIRE(t.application->timestamps.front().msSinceEpoch == 1254); // 1234 + difference of 20
}

TEST_CASE(SUITE("WriteTimeDateNotAsking"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.application->allowTimeWrite = false;
    t.SendToOutstation("C0 02 32 01 07 01 D2 04 00 00 00 00"); // write Grp50Var1, value = 1234 ms after epoch
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 04");        // param error
    REQUIRE(t.application->timestamps.empty());
}

TEST_CASE(SUITE("WriteTimeDateMultipleObjects"))
{
    OutstationConfig cfg;
    OutstationTestObject t(cfg);
    t.LowerLayerUp();

    t.SendToOutstation(
        "C0 02 32 01 07 02 D2 04 00 00 00 00 D2 04 00 00 00 00"); // write Grp50Var1, value = 1234 ms after epoch
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 04");           // param error +  need time still set
    REQUIRE(t.application->timestamps.empty());
}

TEST_CASE(SUITE("BlankIntegrityPoll"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");
}

TEST_CASE(SUITE("MixedVariationAssignments"))
{
    OutstationTestObject t(OutstationConfig(),
                           configure::from({{0, configure::analog(StaticAnalogVariation::Group30Var1)},
                                            {1, configure::analog(StaticAnalogVariation::Group30Var2)}}));

    t.LowerLayerUp();

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

    // check that the response uses both g30v1 & g30v2
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00 1E 02 00 01 01 02 00 00");
}

TEST_CASE(SUITE("TypesCanBeOmittedFromClass0ViaConfig"))
{
    OutstationConfig config;
    config.params.typesAllowedInClass0 = StaticTypeBitField::AllTypes().Except(StaticTypeBitmask::DoubleBinaryInput);
    OutstationTestObject t(config,
                           configure::database_by_sizes(1, 1, 0, 0, 0, 0, 0, 0, 0)); // 1 binary and 1 double binary

    t.LowerLayerUp();
    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 02 00 00 00 02");
}

TEST_CASE(SUITE("octet strings can be returned as part of a class 0 scan"))
{
    OutstationConfig config;
    config.params.typesAllowedInClass0 = StaticTypeBitField::AllTypes();
    OutstationTestObject t(config, configure::by_count_of::octet_string(3));

    t.LowerLayerUp();
    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 6E 01 00 00 02 00 00 00");
}

TEST_CASE(SUITE("static octet strings switch headers if length differs"))
{
    OutstationConfig config;
    config.params.typesAllowedInClass0 = StaticTypeBitField::AllTypes();
    OutstationTestObject t(config, configure::by_count_of::octet_string(2));

    OctetString data;
    REQUIRE(data.Set("a"));
    REQUIRE(t.context.GetUpdateHandler().Update(data, 0));
    REQUIRE(data.Set("bb"));
    REQUIRE(t.context.GetUpdateHandler().Update(data, 1));

    t.LowerLayerUp();
    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 6E 01 00 00 00 61 6E 02 00 01 01 62 62");
}

TEST_CASE(SUITE("octet strings can be returned by reading g110v0"))
{
    OutstationConfig config;
    config.params.typesAllowedInClass0 = StaticTypeBitField::AllTypes();
    OutstationTestObject t(config, configure::by_count_of::octet_string(3));

    t.LowerLayerUp();
    t.SendToOutstation("C0 01 6E 00 06"); // g110v0
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 6E 01 00 00 02 00 00 00");
}

TEST_CASE(SUITE("ReadClass0MultiFragAnalog"))
{
    OutstationConfig config;
    config.params.maxTxFragSize = 20; // override to use a fragment length of 20
    OutstationTestObject t(config, configure::by_count_of::analog_input(8));
    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        for (uint16_t i = 0; i < 8; i++)
        {
            db.Update(Analog(0, Flags(0x01)), i);
        }
    });

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

    // Response should be (30,1)x2 per fragment, quality ONLINE, value 0
    // 4 fragment response, first 3 fragments should be confirmed, last one shouldn't be
    REQUIRE(t.lower->PopWriteAsHex() == "A0 81 80 00 1E 01 00 00 01 01 00 00 00 00 01 00 00 00 00");
    t.OnTxReady();
    t.SendToOutstation("C0 00");
    REQUIRE(t.lower->PopWriteAsHex() == "21 81 80 00 1E 01 00 02 03 01 00 00 00 00 01 00 00 00 00");
    t.OnTxReady();
    t.SendToOutstation("C1 00");
    REQUIRE(t.lower->PopWriteAsHex() == "22 81 80 00 1E 01 00 04 05 01 00 00 00 00 01 00 00 00 00");
    t.OnTxReady();
    t.SendToOutstation("C2 00");
    REQUIRE(t.lower->PopWriteAsHex() == "43 81 80 00 1E 01 00 06 07 01 00 00 00 00 01 00 00 00 00");
    t.OnTxReady();
    t.SendToOutstation("C3 00");

    REQUIRE(t.lower->PopWriteAsHex().empty());
}

TEST_CASE(SUITE("ReadFuncNotSupported"))
{
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();

    t.SendToOutstation("C0 01 0C 01 06");               // try to read 12/1 (control block)
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 01"); // restart/func not supported
}

void NewTestStaticRead(const std::string& request, const std::string& response)
{
    OutstationConfig config;
    OutstationTestObject t(config, configure::by_count_of::all_types(1));
    t.LowerLayerUp();

    t.SendToOutstation(request);
    REQUIRE(t.lower->PopWriteAsHex() == response);
}

// ---- Group50Var4 TimeAndInterval support ----- //

void TestTimeAndIntervalRead(const std::string& request)
{
    OutstationConfig config;
    OutstationTestObject t(config, configure::by_count_of::time_and_interval(1));
    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) { db.Update(TimeAndInterval(DNPTime(9), 3, IntervalUnits::Days), 0); });

    t.SendToOutstation(request);
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 32 04 00 00 00 09 00 00 00 00 00 03 00 00 00 05");
}

TEST_CASE(SUITE("TimeAndIntervalViaIntegrity"))
{
    TestTimeAndIntervalRead(hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("TimeAndIntervalViaDirectRequest"))
{
    TestTimeAndIntervalRead("C0 01 32 04 06");
}

TEST_CASE(SUITE("TimeAndIntervalViaDirectRangeRequest"))
{
    TestTimeAndIntervalRead("C0 01 32 04 00 00 00");
}

TEST_CASE(SUITE("TestTimeAndIntervalWrite"))
{
    OutstationConfig config;
    OutstationTestObject t(config, configure::by_count_of::time_and_interval(1));
    t.LowerLayerUp();

    t.application->supportsWriteTimeAndInterval = true;

    // write g50v4 using 2-octet count & index prefix
    t.SendToOutstation("C0 02 32 04 28 01 00 07 00 09 00 00 00 00 00 03 00 00 00 05");
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00");

    REQUIRE(t.application->timeAndIntervals.size() == 1);
    REQUIRE(t.application->timeAndIntervals[0].index == 7);
    REQUIRE(t.application->timeAndIntervals[0].value.time.value == 9);
}

// ---- Static data reads ----- //

TEST_CASE(SUITE("ReadGrp1Var0ViaIntegrity"))
{
    NewTestStaticRead("C0 01 01 00 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp1Var2ViaIntegrity"))
{
    NewTestStaticRead("C0 01 01 02 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, packed format
}

TEST_CASE(SUITE("ReadGrp10Var0ViaIntegrity"))
{
    NewTestStaticRead("C0 01 0A 00 06", "C0 81 80 00 0A 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp20Var0ViaIntegrity"))
{
    NewTestStaticRead("C0 01 14 00 06",
                      "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("RreadGrp20Var1ViaIntegrity"))
{
    NewTestStaticRead("C0 01 14 01 06",
                      "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("RreadGrp20Var5ViaIntegrity"))
{
    NewTestStaticRead("C0 01 14 05 06", "C0 81 80 00 14 05 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var0ViaIntegrity"))
{
    NewTestStaticRead("C0 01 1E 00 06",
                      "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var1ViaIntegrity"))
{
    NewTestStaticRead("C0 01 1E 01 06",
                      "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var3ViaIntegrity"))
{
    NewTestStaticRead("C0 01 1E 03 06", "C0 81 80 00 1E 03 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp40Var0ViaIntegrity"))
{
    NewTestStaticRead("C0 01 28 00 06",
                      "C0 81 80 00 28 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadByRangeHeader"))
{
    OutstationConfig config;
    OutstationTestObject t(config, configure::by_count_of::analog_input(10));
    t.LowerLayerUp();

    t.Transaction([](IUpdateHandler& db) {
        db.Update(Analog(42, Flags(0x01)), 5);
        db.Update(Analog(41, Flags(0x01)), 6);
    });

    t.SendToOutstation("C2 01 1E 02 00 05 06"); // read 30 var 2, [05 : 06]
    REQUIRE(t.lower->PopWriteAsHex() == "C2 81 80 00 1E 02 00 05 06 01 2A 00 01 29 00");
}

template<class PointType>
void TestStaticType(const OutstationConfig& config,
                    const DatabaseConfig& database,
                    PointType value,
                    const std::string& rsp)
{
    OutstationTestObject t(config, database);

    t.LowerLayerUp();

    t.Transaction([value](IUpdateHandler& db) { db.Update(PointType(value), 0); });

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

    REQUIRE((t.lower->PopWriteAsHex() == rsp));
}

template<class T> void TestStaticCounter(StaticCounterVariation variation, T value, const std::string& response)
{
    auto database = configure::by_count_of::counter(1);
    database.counter[0].svariation = variation;
    TestStaticType<Counter>(OutstationConfig(), std::move(database), Counter(value), response);
}

TEST_CASE(SUITE("ReadGrp1Var1"))
{
    OutstationConfig cfg;
    DatabaseConfig database = configure::by_count_of::binary_input(10);

    for (auto& item : database.binary_input)
    {
        item.second.svariation = StaticBinaryVariation::Group1Var1;
    }

    OutstationTestObject t(cfg, std::move(database));

    // set all the values to ONLINE so that they don't get promoted
    t.Transaction([](IUpdateHandler& handler) { handler.Modify(FlagsType::BinaryInput, 0, 9, 0x01); });

    t.LowerLayerUp();

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 01 00 00 09 00 00");
}

TEST_CASE(SUITE("Group1Var1 is promoted to Group1Var2 if quality not online"))
{
    DatabaseConfig database = configure::by_count_of::binary_input(2);

    for (auto& item : database.binary_input)
    {
        item.second.svariation = StaticBinaryVariation::Group1Var1;
    }

    OutstationTestObject t(OutstationConfig(), std::move(database));

    t.LowerLayerUp();

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 02 00 00 01 02 02");
}

TEST_CASE(SUITE("ReadGrp20Var1"))
{
    TestStaticCounter(StaticCounterVariation::Group20Var1, 5, "C0 81 80 00 14 01 00 00 00 01 05 00 00 00");
}

TEST_CASE(SUITE("ReadGrp20Var2"))
{
    TestStaticCounter(StaticCounterVariation::Group20Var2, 5, "C0 81 80 00 14 02 00 00 00 01 05 00");
}

TEST_CASE(SUITE("ReadGrp20Var5"))
{
    TestStaticCounter(StaticCounterVariation::Group20Var5, 5, "C0 81 80 00 14 05 00 00 00 05 00 00 00");
}

TEST_CASE(SUITE("ReadGrp20Var6"))
{
    TestStaticCounter(StaticCounterVariation::Group20Var6, 5, "C0 81 80 00 14 06 00 00 00 05 00");
}

template<class T> void TestStaticAnalog(StaticAnalogVariation variation, T value, const std::string& response)
{
    auto database = configure::by_count_of::analog_input(1);
    database.analog_input[0].svariation = variation;
    TestStaticType<Analog>(OutstationConfig(), std::move(database), Analog(value), response);
}

TEST_CASE(SUITE("ReadGrp30Var2"))
{
    TestStaticAnalog(StaticAnalogVariation::Group30Var2, 100, "C0 81 80 00 1E 02 00 00 00 01 64 00");
}

TEST_CASE(SUITE("ReadGrp30Var3"))
{
    TestStaticAnalog(StaticAnalogVariation::Group30Var3, 65536, "C0 81 80 00 1E 03 00 00 00 00 00 01 00");
}

TEST_CASE(SUITE("ReadGrp30Var4"))
{
    TestStaticAnalog(StaticAnalogVariation::Group30Var4, 100, "C0 81 80 00 1E 04 00 00 00 64 00");
}

TEST_CASE(SUITE("ReadGrp30Var5"))
{
    TestStaticAnalog(StaticAnalogVariation::Group30Var5, 95.6, "C0 81 80 00 1E 05 00 00 00 01 33 33 BF 42");
}

TEST_CASE(SUITE("ReadGrp30Var6"))
{
    TestStaticAnalog(StaticAnalogVariation::Group30Var6, -20, "C0 81 80 00 1E 06 00 00 00 01 00 00 00 00 00 00 34 C0");
}

template<class T> void TestStaticBinaryOutputStatus(T value, const std::string& response)
{
    OutstationConfig cfg;
    OutstationTestObject t(cfg, configure::by_count_of::binary_output_status(1));
    t.LowerLayerUp();

    t.Transaction([value](IUpdateHandler& db) { db.Update(BinaryOutputStatus(value, opendnp3::Flags(0x01)), 0); });

    t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
    REQUIRE((t.lower->PopWriteAsHex() == response));
}

TEST_CASE(SUITE("ReadGrp10Var2"))
{
    TestStaticBinaryOutputStatus<bool>(true, "C0 81 80 00 0A 02 00 00 00 81");
}

template<class T>
void TestStaticAnalogOutputStatus(StaticAnalogOutputStatusVariation variation, T value, const std::string& response)
{
    auto database = configure::by_count_of::analog_output_status(1);
    database.analog_output_status[0].svariation = variation;

    TestStaticType<AnalogOutputStatus>(OutstationConfig(), std::move(database), AnalogOutputStatus(value), response);
}

TEST_CASE(SUITE("ReadGrp40Var1"))
{
    TestStaticAnalogOutputStatus(StaticAnalogOutputStatusVariation::Group40Var1, 100,
                                 "C0 81 80 00 28 01 00 00 00 01 64 00 00 00");
}

TEST_CASE(SUITE("ReadGrp40Var2"))
{
    TestStaticAnalogOutputStatus(StaticAnalogOutputStatusVariation::Group40Var2, 100,
                                 "C0 81 80 00 28 02 00 00 00 01 64 00");
}

TEST_CASE(SUITE("ReadGrp40Var3"))
{
    TestStaticAnalogOutputStatus(StaticAnalogOutputStatusVariation::Group40Var3, 95.6,
                                 "C0 81 80 00 28 03 00 00 00 01 33 33 BF 42");
}

TEST_CASE(SUITE("ReadGrp40Var4"))
{
    TestStaticAnalogOutputStatus(StaticAnalogOutputStatusVariation::Group40Var4, -20.0,
                                 "C0 81 80 00 28 04 00 00 00 01 00 00 00 00 00 00 34 C0");
}

TEST_CASE(SUITE("read g1v2 using qualifer 0x17"))
{
    OutstationConfig config;
    config.params.allowUnsolicited = false;
    OutstationTestObject t(config, configure::database_by_sizes(3, 0, 0, 0, 0, 0, 0, 0, 0));

    t.LowerLayerUp();
    t.SendToOutstation("C0 01 01 02 17 02 00 02"); // Read g1v2 indices 0 and 2
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 02 00 00 00 02 01 02 00 02 02 02");
}

TEST_CASE(SUITE("read g1v2 using qualifer 0x28"))
{
    OutstationConfig config;
    config.params.allowUnsolicited = false;
    OutstationTestObject t(config, configure::database_by_sizes(3, 0, 0, 0, 0, 0, 0, 0, 0));

    t.LowerLayerUp();
    t.SendToOutstation("C0 01 01 02 28 02 00 00 00 02 00"); // Read g1v2 indices 0 and 2
    REQUIRE(t.lower->PopWriteAsHex() == "C0 81 80 00 01 02 00 00 00 02 01 02 00 02 02 02");
}
