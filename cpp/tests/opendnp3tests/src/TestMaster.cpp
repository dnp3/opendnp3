/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include <catch.hpp>

#include "MasterTestObject.h"
#include "MeasurementComparisons.h"
#include "HexConversions.h"
#include "MockCommandCallback.h"
#include "APDUHexBuilders.h"

#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/APDUBuilders.h>
#include <opendnp3/app/PointClass.h>

using namespace openpal;
using namespace opendnp3;


#define SUITE(name) "MasterTestSuite - " name

TEST_CASE(SUITE("InitialState"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	MasterTestObject t(params);

	t.master.OnLowerLayerUp();
	t.master.OnLowerLayerDown();
}

TEST_CASE(SUITE("IntegrityOnStartup"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("SolicitedResponseWithData"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.unsolClassMask = 0;
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	t.master.OnSendResult(true);
	REQUIRE(t.exe.NumPendingTimers() == 1);
	t.SendToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true	
	REQUIRE(t.meas.NumTotal() == 1);
	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetBinary(2)));
}

TEST_CASE(SUITE("IntegrityPollCanRepeat"))
{	
	MasterTestObject t(NoStartupTasks());
	t.master.OnLowerLayerUp();

	auto scan = t.master.AddClassScan(~0, TimeDuration::Seconds(10));	

	t.exe.AdvanceTime(TimeDuration::Seconds(9));
	REQUIRE(t.exe.RunMany() == 0);
	t.exe.AdvanceTime(TimeDuration::Seconds(1));
	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	t.master.OnSendResult(true);
	t.SendToMaster(hex::EmptyResponse(0));

	// 2nd poll
	REQUIRE(t.exe.NumPendingTimers() == 1);
	REQUIRE(t.exe.NextTimerExpiration().milliseconds == 20000);
	t.exe.AdvanceTime(TimeDuration::Seconds(10));
	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(1));
}

TEST_CASE(SUITE("UnsolDisableEnableOnStartup"))
{
	MasterParams params;
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();
	
	REQUIRE(t.exe.RunMany() > 0);

	// disable unsol on grp 60 var2, var3, var4
	REQUIRE(t.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ALL_EVENT_CLASSES));
	t.master.OnSendResult(true);
	t.SendToMaster(hex::EmptyResponse(0));

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(1));
	t.master.OnSendResult(true);
	t.SendToMaster(hex::EmptyResponse(1));

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::ENABLE_UNSOLICITED, 2, ALL_EVENT_CLASSES));
	t.master.OnSendResult(true);
	t.SendToMaster(hex::EmptyResponse(2));

	t.exe.RunMany();

	REQUIRE(t.exe.NumPendingTimers() == 0);	
}

TEST_CASE(SUITE("TimeoutDuringStartup"))
{
	MasterParams params;
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ALL_EVENT_CLASSES));
	t.master.OnSendResult(true);

	// timeout the task
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany());

	REQUIRE(t.lower.NumWrites() == 0);
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany());

	// repeat the disable unsol
	REQUIRE(t.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 1, ALL_EVENT_CLASSES));	
}

TEST_CASE(SUITE("SolicitedResponseTimeout"))
{	
	MasterTestObject t(NoStartupTasks());
	auto scan = t.master.AddClassScan(ALL_CLASSES, TimeDuration::Seconds(5));
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.AdvanceToNextTimer());
	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	t.master.OnSendResult(true);
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.exe.NumPendingTimers() == 1);
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(1));
}

TEST_CASE(SUITE("SolicitedResponseLayerDown"))
{
	MasterTestObject t(NoStartupTasks());
	auto scan = t.master.AddClassScan(ALL_CLASSES, TimeDuration::Seconds(5));
	t.master.OnLowerLayerUp();	

	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	t.master.OnLowerLayerDown();

	t.master.OnLowerLayerUp();	

	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("SolicitedMultiFragResponse"))
{
	auto config = NoStartupTasks();
	config.startupIntergrityClassMask = ALL_CLASSES;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() ==  hex::IntegrityPoll(0));
	t.master.OnSendResult(true);
	t.SendToMaster("80 81 00 00 01 02 00 02 02 81"); // partial response FIR = 1, FIN = 0	
	REQUIRE(1 == t.meas.NumStatic());
	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetBinary(2)));
	REQUIRE(0 == t.lower.NumWrites());
	t.SendToMaster("41 81 00 00 01 02 00 03 03 02"); // final response FIR = 0, FIN = 1
	REQUIRE(2 == t.meas.NumStatic());
	REQUIRE((Binary(false, BQ_RESTART) == t.meas.GetBinary(3)));	
}

TEST_CASE(SUITE("EventPoll"))
{	
	MasterTestObject t(NoStartupTasks());

	auto class12 = t.master.AddClassScan(CLASS_1 | CLASS_2, TimeDuration::Milliseconds(10));
	auto class3 = t.master.AddClassScan(CLASS_3, TimeDuration::Milliseconds(20));

	t.master.OnLowerLayerUp();		
	
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 01 3C 02 06 3C 03 06");
	t.master.OnSendResult(true);
	t.SendToMaster("C0 81 00 00 02 01 17 01 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	REQUIRE(t.meas.NumEvent() == 1);
	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetEventBinary(2)));

	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == "C1 01 3C 04 06");
	t.master.OnSendResult(true);
	t.SendToMaster("C1 81 00 00 02 01 17 01 03 01"); //group 2 var 1, index = 3, 0x81 = Online, true

	REQUIRE(t.meas.NumEvent() == 2);
	REQUIRE((Binary(false, BQ_ONLINE) == t.meas.GetEventBinary(3)));
}

TEST_CASE(SUITE("ParsesOctetStringResponseWithFiveCharacters"))
{	
	MasterTestObject t(NoStartupTasks());	
	t.master.OnLowerLayerUp();	

	// Group 111 (0x6F) Variation (length), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	t.SendToMaster("D0 82 00 00 6F 05 17 01 04 68 65 6C 6C 6F");	

	REQUIRE("68 65 6C 6C 6F" ==  toHex(t.meas.GetEventOctetString(4).ToReadOnly()));
}

TEST_CASE(SUITE("ParsesOctetStringResponseSizeOfOne"))
{			
	MasterTestObject t(NoStartupTasks());
	t.master.AddClassScan(~0, TimeDuration::Seconds(1));
	t.master.OnLowerLayerUp();	

	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	t.master.OnSendResult(true);

	// octet strings shouldn't be found in class 0 polls, but we'll test that we can process them anyway
	// Group 110 (0x6E) Variation(length), start = 3, stop = 3
	t.SendToMaster("C0 81 00 00 6E 01 00 03 03 AA");

	REQUIRE("AA" ==  toHex(t.meas.GetOctetString(3).ToReadOnly()));
}

TEST_CASE(SUITE("RestartDuringStartup"))
{

	MasterParams params;
	params.startupIntergrityClassMask = 0; //disable integrity poll
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();	

	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ALL_EVENT_CLASSES));
	t.master.OnSendResult(false);

	t.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::DEVICE_RESTART)));

	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == hex::ClearRestartIIN(1));
	t.master.OnSendResult(false);
	t.SendToMaster(hex::EmptyResponse(1));

	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::ENABLE_UNSOLICITED, 2, ALL_EVENT_CLASSES));	
}

TEST_CASE(SUITE("RestartAndTimeBits"))
{
	auto params = NoStartupTasks();
	params.timeSyncMode = TimeSyncMode::SerialTimeSync;
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();

	t.timeSource.time = 100;
	t.exe.RunMany();

	REQUIRE(t.lower.NumWrites() == 0);

	t.SendToMaster(hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART) | IINField(IINBit::NEED_TIME)));

	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == hex::UnsolConfirm(0));
	t.master.OnSendResult(true);	

	REQUIRE(t.lower.PopWriteAsHex() == hex::ClearRestartIIN(0));
	t.master.OnSendResult(true);
	t.SendToMaster(hex::EmptyResponse(0, IINField::Empty));

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == hex::MeasureDelay(1));
	t.master.OnSendResult(true);
	t.timeSource.time += 100; //advance time by 100ms so that the master sees 100ms for a response
	t.SendToMaster("C1 81 10 00 34 02 07 01 0A 00"); // still need time, 52 Var 2, delay == 10ms

	t.exe.RunMany();

	// Write group 50 var 1
	// 200-100-10/2 = 45 => 45 + 200 - 0xF5
	REQUIRE(t.lower.PopWriteAsHex() == "C2 02 32 01 07 01 F5 00 00 00 00 00");
	t.master.OnSendResult(true);
	t.SendToMaster(hex::EmptyResponse(0, IINField::Empty)); // time bit is now clear

	t.exe.RunMany();

	REQUIRE(t.lower.NumWrites() ==  0); // no more packets
}

