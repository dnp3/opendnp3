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
#include "utils/CallbackQueue.h"
#include "utils/MasterTestFixture.h"
#include "utils/MeasurementComparisons.h"

#include <ser4cpp/util/HexConversions.h>

#include "dnp3mocks/MockTaskCallback.h"

#include <app/APDUBuilders.h>
#include <app/APDUResponse.h>
#include <catch.hpp>

using namespace opendnp3;
using namespace ser4cpp;

#define SUITE(name) "MasterTestSuite - " name

TEST_CASE(SUITE("notifies application of state changes"))
{
    MasterParams params;
    params.disableUnsolOnStartup = false;
    MasterTestFixture t(params);

    REQUIRE(t.application->stateChanges.empty());
    t.context->OnLowerLayerUp();

    REQUIRE(t.application->stateChanges.size() == 1);
    REQUIRE(t.application->stateChanges.front() == MockMasterApplication::State::OPEN);
    t.application->stateChanges.pop_front();

    t.context->OnLowerLayerDown();

    REQUIRE(t.application->stateChanges.size() == 1);
    REQUIRE(t.application->stateChanges.front() == MockMasterApplication::State::CLOSED);
    t.application->stateChanges.pop_front();
}

TEST_CASE(SUITE("IntegrityOnStartup"))
{
    MasterParams params;
    params.disableUnsolOnStartup = false;
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("SolicitedResponseWithData"))
{
    MasterParams params;
    params.disableUnsolOnStartup = false;
    params.unsolClassMask = ClassField::None();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();
    t.SendToMaster("C0 81 00 00 01 02 00 02 02 81"); // group 2 var 1, index = 2, 0x81 = Online, true
    REQUIRE(t.meas->TotalReceived() == 1);
    REQUIRE((Binary(true, Flags(0x01)) == t.meas->binarySOE[2].meas));
}

TEST_CASE(SUITE("UnsolDisableEnableOnStartup"))
{
    MasterParams params;
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    // disable unsol on grp 60 var2, var3, var4
    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses()));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(0));

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(1));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(1));

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::ENABLE_UNSOLICITED, 2, ClassField::AllEventClasses()));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(2));

    t.exe->run_many();

    REQUIRE(t.exe->num_pending_timers() == 1);
}

TEST_CASE(SUITE("TimeoutDuringStartup"))
{
    MasterParams params;
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses()));
    t.context->OnTxReady();

    // timeout the task
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->run_many());

    REQUIRE(t.lower->NumWrites() == 0);
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->run_many());

    // repeat the disable unsol
    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 1, ClassField::AllEventClasses()));
}

TEST_CASE(SUITE("SolicitedResponseTimeout"))
{
    MasterTestFixture t(NoStartupTasks());
    auto scan = t.context->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(5), t.meas);
    t.context->OnLowerLayerUp();

    t.exe->run_many();
    REQUIRE(t.exe->advance_to_next_timer());
    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.exe->num_pending_timers() == 1);
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(1));
}

TEST_CASE(SUITE("Retries use exponential backoff"))
{
    MasterParams params;
    params.disableUnsolOnStartup = false;
    MasterTestFixture t(params);
    const auto startTime = t.exe->get_time();
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();

    // time out the response
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->get_time() - startTime == std::chrono::milliseconds(5000));
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex().empty());

    // advance to the retry
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->get_time() - startTime == std::chrono::milliseconds(10000));
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(1));
    t.context->OnTxReady();

    // time out the response
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->get_time() - startTime == std::chrono::milliseconds(15000));
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex().empty());

    // advance to the retry
    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->get_time() - startTime
            == std::chrono::milliseconds(25000)); // this time the retry doubles to 10 seconds
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(2));
    t.context->OnTxReady();
}

TEST_CASE(SUITE("AllObjectsScan"))
{
    MasterTestFixture t(NoStartupTasks());
    auto scan = t.context->AddAllObjectsScan(GroupVariationID(110, 0), TimeDuration::Seconds(1), t.meas);
    t.context->OnLowerLayerUp();

    t.exe->run_many();
    REQUIRE(t.exe->advance_to_next_timer());
    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == "C0 01 6E 00 06");
}

TEST_CASE(SUITE("ClassScanCanRepeat"))
{
    MasterParams params = NoStartupTasks();
    MasterTestFixture t(params);
    const auto startTime = t.exe->get_time();
    t.context->OnLowerLayerUp();

    t.exe->run_many();

    auto scan = t.context->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(10), t.meas);

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(0));

    t.exe->run_many();

    // 2nd poll
    REQUIRE(t.exe->num_pending_timers() == 1);
    REQUIRE(t.exe->next_timer_expiration_abs() - startTime == std::chrono::seconds(10));
    t.exe->advance_time(std::chrono::seconds(10));
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(1));
}

TEST_CASE(SUITE("SolicitedResponseLayerDown"))
{
    MasterTestFixture t(NoStartupTasks());
    auto scan = t.context->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(5), t.meas);
    t.context->OnLowerLayerUp();

    t.exe->run_many();

    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnLowerLayerDown();

    t.context->OnLowerLayerUp();
    t.exe->run_many();

    REQUIRE(t.exe->advance_to_next_timer());
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("SolicitedMultiFragResponse"))
{
    auto config = NoStartupTasks();
    config.startupIntegrityClassMask = ClassField::AllClasses();
    MasterTestFixture t(config);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();
    t.SendToMaster("80 81 00 00 01 02 00 02 02 81"); // partial response FIR = 1, FIN = 0
    REQUIRE(1 == t.meas->TotalReceived());
    REQUIRE((Binary(true, Flags(0x01)) == t.meas->binarySOE[2].meas));
    REQUIRE(0 == t.lower->NumWrites());
    t.SendToMaster("41 81 00 00 01 02 00 03 03 02"); // final response FIR = 0, FIN = 1
    REQUIRE(2 == t.meas->TotalReceived());
    REQUIRE((Binary(false, Flags(0x02)) == t.meas->binarySOE[3].meas));
}

TEST_CASE(SUITE("EventPoll"))
{
    MasterTestFixture t(NoStartupTasks());

    auto class12 = t.context->AddClassScan(ClassField(ClassField::CLASS_1 | ClassField::CLASS_2),
                                           TimeDuration::Milliseconds(10), t.meas);
    auto class3 = t.context->AddClassScan(ClassField(ClassField::CLASS_3), TimeDuration::Milliseconds(20), t.meas);

    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == "C0 01 3C 02 06 3C 03 06");
    t.context->OnTxReady();
    t.SendToMaster("C0 81 00 00 02 01 17 01 02 81"); // group 2 var 1, index = 2, 0x81 = Online, true

    REQUIRE(t.meas->TotalReceived() == 1);
    REQUIRE((Binary(true, Flags(0x01)) == t.meas->binarySOE[2].meas));

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == "C1 01 3C 04 06");
    t.context->OnTxReady();
    t.SendToMaster("C1 81 00 00 02 01 17 01 03 01"); // group 2 var 1, index = 3, 0x81 = Online, true

    REQUIRE(t.meas->TotalReceived() == 2);
    REQUIRE((Binary(false, Flags(0x01)) == t.meas->binarySOE[3].meas));
}

TEST_CASE(SUITE("ParsesOctetStringResponseWithFiveCharacters"))
{
    MasterTestFixture t(NoStartupTasks());
    t.context->OnLowerLayerUp();

    // Group 111 (0x6F) Variation (length), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C,
    // 0x6C, 0x6F]
    t.SendToMaster("D0 82 00 00 6F 05 17 01 04 68 65 6C 6C 6F");

    auto buffer = t.meas->octetStringSOE[4].meas.ToBuffer();
    REQUIRE("68 65 6C 6C 6F" == HexConversions::to_hex(rseq_t(buffer.data, buffer.length)));
}

TEST_CASE(SUITE("ParsesOctetStringResponseSizeOfOne"))
{
    MasterTestFixture t(NoStartupTasks());
    t.context->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(1), t.meas);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();

    // octet strings shouldn't be found in class 0 polls, but we'll test that we can process them anyway
    // Group 110 (0x6E) Variation(length), start = 3, stop = 3
    t.SendToMaster("C0 81 00 00 6E 01 00 03 03 AA");

    auto buffer = t.meas->octetStringSOE[3].meas.ToBuffer();
    REQUIRE("AA" == HexConversions::to_hex(rseq_t(buffer.data, buffer.length)));
}

TEST_CASE(SUITE("ParsesGroup2Var3Correctly"))
{
    auto config = NoStartupTasks();
    config.startupIntegrityClassMask = ClassField(~0);
    MasterTestFixture t(config);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();

    // g51v1, t = 3,
    // g2v3, index 7, t = 2, true/online
    // g2v3, index 8, t = 3, true/online
    t.SendToMaster("C0 81 00 00 33 01 07 01 03 00 00 00 00 00 02 03 17 01 07 81 02 00 02 03 17 01 08 81 03 00");

    REQUIRE(t.meas->binarySOE.size() == 2);
    {
        auto record = t.meas->binarySOE[7];
        REQUIRE(record.meas.time.value == 5);
        REQUIRE(record.info.gv == GroupVariation::Group2Var3);
    }
    {
        auto record = t.meas->binarySOE[8];
        REQUIRE(record.meas.time.value == 6);
        REQUIRE(record.info.gv == GroupVariation::Group2Var3);
    }
}

TEST_CASE(SUITE("ParsesGroup2Var3TimeQualityCorrectly"))
{
    auto config = NoStartupTasks();
    config.startupIntegrityClassMask = ClassField(~0);
    MasterTestFixture t(config);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();

    // g51v1, t = 3,
    // g2v3, index 7, t = 2, Synchronized, true/online
    // g2v3, index 8, t = 3, Unsychronized, true/online
    t.SendToMaster("C0 81 00 00 33 01 07 01 03 00 00 00 00 00 02 03 17 01 07 81 02 00 33 02 07 01 03 00 00 00 00 00 02 "
                   "03 17 01 08 81 03 00");

    REQUIRE(t.meas->binarySOE.size() == 2);
    {
        auto record = t.meas->binarySOE[7];
        REQUIRE(record.meas.time == DNPTime(5, TimestampQuality::SYNCHRONIZED));
        REQUIRE(record.info.gv == GroupVariation::Group2Var3);
    }
    {
        auto record = t.meas->binarySOE[8];
        REQUIRE(record.meas.time == DNPTime(6, TimestampQuality::UNSYNCHRONIZED));
        REQUIRE(record.info.gv == GroupVariation::Group2Var3);
    }
}

TEST_CASE(SUITE("ParsesGroup50Var4"))
{
    auto config = NoStartupTasks();
    config.startupIntegrityClassMask = ClassField(~0);
    MasterTestFixture t(config);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();

    t.SendToMaster("C0 81 00 00 32 04 00 00 00 09 00 00 00 00 00 03 00 00 00 05");

    REQUIRE(1 == t.meas->timeAndIntervalSOE.size());
    REQUIRE(t.meas->timeAndIntervalSOE[0].meas.interval == 3);
    REQUIRE(t.meas->timeAndIntervalSOE[0].meas.time.value == 9);
    REQUIRE(t.meas->timeAndIntervalSOE[0].meas.GetUnitsEnum() == IntervalUnits::Days);
}

TEST_CASE(SUITE("RestartViaNullUnsol"))
{
    MasterParams params;

    // disable all the automated behaviors except for restart IIN
    params.disableUnsolOnStartup = false;
    params.startupIntegrityClassMask = ClassField::None();
    params.unsolClassMask = ClassField::None();
    params.startupIntegrityClassMask = ClassField::None(); // disable integrity poll

    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->NumWrites() == 0);
    t.SendToMaster("F0 82 80 00");
    REQUIRE(t.lower->PopWriteAsHex() == hex::Confirm(0, true));
    t.context->OnTxReady();
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == "C0 02 50 01 00 07 07 00");
}

TEST_CASE(SUITE("DisableAutomatedRestartClear"))
{
    MasterParams params;

    // disable all the automated behaviors
    params.ignoreRestartIIN = true;
    params.disableUnsolOnStartup = false;
    params.startupIntegrityClassMask = ClassField::None();
    params.unsolClassMask = ClassField::None();
    params.startupIntegrityClassMask = ClassField::None(); // disable integrity poll

    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->NumWrites() == 0);
    t.SendToMaster("F0 82 80 00");
    REQUIRE(t.lower->PopWriteAsHex() == hex::Confirm(0, true));
    t.context->OnTxReady();
    REQUIRE(t.lower->NumWrites() == 0);
}

TEST_CASE(SUITE("RestartDuringStartup"))
{

    MasterParams params;
    params.startupIntegrityClassMask = ClassField::None(); // disable integrity poll
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses()));
    t.context->OnTxReady();

    t.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::DEVICE_RESTART)));

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::ClearRestartIIN(1));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(1));

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::ENABLE_UNSOLICITED, 2, ClassField::AllEventClasses()));
}

TEST_CASE(SUITE("performs non-LAN time synchronization"))
{
    auto params = NoStartupTasks();
    params.timeSyncMode = TimeSyncMode::NonLAN;
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    t.application->time = 100;
    t.exe->run_many();

    REQUIRE(t.lower->NumWrites() == 0);

    t.SendToMaster(hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART) | IINField(IINBit::NEED_TIME)));

    REQUIRE(t.lower->PopWriteAsHex() == hex::UnsolConfirm(0));
    t.context->OnTxReady();
    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::ClearRestartIIN(0));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(0, IINField::Empty()));

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::MeasureDelay(1));
    t.context->OnTxReady();
    t.application->time += 100; // advance time by 100ms so that the master sees 100ms for a response
    t.SendToMaster("C1 81 10 00 34 02 07 01 0A 00"); // still need time, 52 Var 2, delay == 10ms

    t.exe->run_many();

    // Write group 50 var 1
    // 200-100-10/2 = 45 => 45 + 200 - 0xF5
    REQUIRE(t.lower->PopWriteAsHex() == "C2 02 32 01 07 01 F5 00 00 00 00 00");
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(0, IINField::Empty())); // time bit is now clear

    t.exe->run_many();

    REQUIRE(t.lower->NumWrites() == 0); // no more packets
}

TEST_CASE(SUITE("performs LAN time synchronization"))
{
    auto params = NoStartupTasks();
    params.timeSyncMode = TimeSyncMode::LAN;
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    t.application->time = 100;
    t.exe->run_many();

    REQUIRE(t.lower->NumWrites() == 0);

    t.SendToMaster(hex::NullUnsolicited(0, IINField(IINBit::NEED_TIME)));
    REQUIRE(t.lower->PopWriteAsHex() == hex::UnsolConfirm(0));
    t.context->OnTxReady();

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::RecordCurrentTime(0));
    t.context->OnTxReady();
    t.application->time += 100; // advance time by 100ms so that the master sees 100ms for a response
    t.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::NEED_TIME)));

    REQUIRE(t.lower->PopWriteAsHex() == "C1 02 32 03 07 01 64 00 00 00 00 00");
    t.context->OnTxReady();
    t.exe->run_many();
    REQUIRE(t.lower->NumWrites() == 0);
}

TEST_CASE(SUITE("ReceiveCTOSynchronized"))
{
    auto params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    t.SendToMaster("D0 82 00 00 33 01 07 01 03 00 00 00 00 00 02 03 28 01 00 07 00 81 01 00");

    REQUIRE(t.meas->TotalReceived() == 1);
    auto record = t.meas->binarySOE[7];
    bool equal
        = record.meas == Binary(true, Flags(0x01), DNPTime(0x04, TimestampQuality::SYNCHRONIZED)); // timestamp is 4
    REQUIRE(equal);
    REQUIRE(record.info.tsquality == TimestampQuality::SYNCHRONIZED);
}

TEST_CASE(SUITE("ReceiveCTOUnsynchronized"))
{
    auto params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    // same as above, but with Group 51 Var 2
    t.SendToMaster("D0 82 00 00 33 02 07 01 03 00 00 00 00 00 02 03 28 01 00 07 00 81 01 00");

    REQUIRE(t.meas->TotalReceived() == 1);
    auto record = t.meas->binarySOE[7];
    bool equal
        = record.meas == Binary(true, Flags(0x01), DNPTime(0x04, TimestampQuality::UNSYNCHRONIZED)); // timestamp is 4
    REQUIRE(equal);
    REQUIRE(record.info.tsquality == TimestampQuality::UNSYNCHRONIZED);
}

TEST_CASE(SUITE("ReceiveIINinResponses"))
{
    auto params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    auto scan = t.context->AddClassScan(ClassField(~0), TimeDuration::Seconds(1), t.meas);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
    t.context->OnTxReady();

    // response with IIN retart bit
    t.SendToMaster("C0 81 80 00");

    REQUIRE(t.application->rxIIN.size() == 1);
    REQUIRE(t.application->rxIIN[0].IsSet(IINBit::DEVICE_RESTART));
}

TEST_CASE(SUITE("ReceiveIINUnsol"))
{
    auto params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    t.SendToMaster(hex::NullUnsolicited(0, IINField(IINBit::DEVICE_TROUBLE)));

    REQUIRE(t.application->rxIIN.size() == 1);
    REQUIRE(t.application->rxIIN[0].IsSet(IINBit::DEVICE_TROUBLE));
}

TEST_CASE(SUITE("EventScanOnEventsAvailableIIN"))
{
    auto params = NoStartupTasks();
    params.eventScanOnEventsAvailableClassMask = ClassField(ClassField::CLASS_1 | ClassField::CLASS_2);
    MasterTestFixture t(params);

    t.context->OnLowerLayerUp();

    t.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::CLASS1_EVENTS)));

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::EventPoll(0, params.eventScanOnEventsAvailableClassMask));
    t.context->OnTxReady();

    t.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::CLASS2_EVENTS)));

    t.exe->run_many();

    REQUIRE(t.lower->PopWriteAsHex() == hex::EventPoll(1, params.eventScanOnEventsAvailableClassMask));
    t.context->OnTxReady();

    t.SendToMaster(hex::EmptyResponse(1, IINField(IINBit::CLASS3_EVENTS)));

    t.exe->run_many();

    REQUIRE(t.lower->NumWrites() == 0);
}

TEST_CASE(SUITE("AdhocScanWorksWithUnsolicitedDisabled"))
{
    MasterParams params = NoStartupTasks();
    params.disableUnsolOnStartup = true;
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    t.context->ScanClasses(ClassField::AllEventClasses(), t.meas);

    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses()));
    t.context->OnTxReady();
    t.SendToMaster(hex::EmptyResponse(0));

    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == hex::ClassTask(FunctionCode::READ, 1, ClassField::AllEventClasses()));
}

TEST_CASE(SUITE("AdhocScanFailsImmediatelyIfMasterOffline"))
{
    MasterParams params = NoStartupTasks();
    MasterTestFixture t(params);

    auto callback = std::make_shared<MockTaskCallback>();
    t.context->ScanClasses(ClassField::AllEventClasses(), t.meas, TaskConfig::With(callback));

    REQUIRE(callback->numStart == 0);
    REQUIRE(callback->results.size() == 1);
    REQUIRE(callback->results[0] == TaskCompletion::FAILURE_NO_COMMS);
    REQUIRE(callback->numDestroyed == 1);
}

TEST_CASE(SUITE("MasterWritesTimeAndInterval"))
{
    MasterParams params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    auto callback = std::make_shared<MockTaskCallback>();

    t.context->Write(TimeAndInterval(DNPTime(3), 4, IntervalUnits::Days), 7, TaskConfig::With(callback));
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == "C0 02 32 04 28 01 00 07 00 03 00 00 00 00 00 04 00 00 00 05");
    t.context->OnTxReady();
    t.SendToMaster("C0 81 00 00");
    REQUIRE(t.lower->PopWriteAsHex().empty());

    REQUIRE(callback->numStart == 1);
    REQUIRE(callback->results.size() == 1);
    REQUIRE(callback->results[0] == TaskCompletion::SUCCESS);
}

TEST_CASE(SUITE("Cold restart fails with empty response"))
{
    MasterParams params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    CallbackQueue<RestartOperationResult> queue;

    t.context->Restart(RestartType::COLD, queue.Callback());
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == "C0 0D"); // cold restart
    t.context->OnTxReady();
    t.SendToMaster("C0 81 00 00");
    REQUIRE(t.lower->PopWriteAsHex().empty());

    REQUIRE(queue.responses.size() == 1);
    REQUIRE(queue.responses[0].summary == TaskCompletion::FAILURE_BAD_RESPONSE);
}

TEST_CASE(SUITE("Warm restart fails with empty response"))
{
    MasterParams params = NoStartupTasks();
    MasterTestFixture t(params);
    t.context->OnLowerLayerUp();

    CallbackQueue<RestartOperationResult> queue;

    t.context->Restart(RestartType::WARM, queue.Callback());
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.lower->PopWriteAsHex() == "C0 0E"); // warm restart
    t.context->OnTxReady();
    t.SendToMaster("C0 81 00 00 34 01 07 01 BB BB");
    REQUIRE(t.lower->PopWriteAsHex().empty());

    REQUIRE(queue.responses.size() == 1);
    REQUIRE(queue.responses[0].summary == TaskCompletion::SUCCESS);
    REQUIRE(queue.responses[0].restartTime == TimeDuration::Milliseconds(0xBBBB));
}
