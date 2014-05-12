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

#include "NewMasterTestObject.h"
#include "MeasurementComparisons.h"
#include "HexConversions.h"
#include "MockCommandCallback.h"

#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/APDUBuilders.h>

using namespace opendnp3;
using namespace openpal;

MasterParams NoStartupTasks()
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.startupIntergrityClassMask = 0;
	params.unsolClassMask = 0;
	return params;
}

std::string Integrity(uint8_t seq, int mask = ~0)
{
	StaticBuffer<100> buffer;
	APDURequest request(buffer.GetWriteBuffer());
	build::ReadIntegrity(request, mask, seq);
	return toHex(request.ToReadOnly());
}

std::string EmptyResponse(uint8_t seq)
{
	StaticBuffer<4> buffer;
	APDUResponse response(buffer.GetWriteBuffer());
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, seq));
	return toHex(response.ToReadOnly());
}

// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";

#define SUITE(name) "MasterTestSuite - " name

TEST_CASE(SUITE("InitialState"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	NewMasterTestObject t(params);

	t.master.OnLowerLayerUp();
	t.master.OnLowerLayerDown();
}


TEST_CASE(SUITE("IntegrityOnStartup"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	NewMasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.Dispatch() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == Integrity(0));	
}

TEST_CASE(SUITE("IntegrityPollLoadsMeasurements"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	NewMasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.Dispatch() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == Integrity(0));
	REQUIRE(t.exe.NumPendingTimers() == 0);
	t.master.OnSendResult(true);
	REQUIRE(t.exe.NumPendingTimers() == 1);
	t.SendToMaster("C0 81 00 00 01 02 00 00 00 81");	
	REQUIRE(t.meas.NumTotal() == 1);
	REQUIRE(Equals(t.meas.GetBinary(0), Binary(true)));	
}

/*
TEST_CASE(SUITE("IntegrityPollCanRepeat"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	NewMasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.Dispatch() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == Integrity(0));
	REQUIRE(t.exe.NumPendingTimers() == 0);
	t.master.OnSendResult(true);
	REQUIRE(t.exe.NumPendingTimers() == 1);
	t.SendToMaster(EmptyResponse(0));	
	t.exe.AdvanceTime(params.integrityPeriod);
	REQUIRE(t.exe.NumPendingTimers() == 0);
	REQUIRE(t.exe.Dispatch() > 0);
	REQUIRE(t.lower.PopWriteAsHex() == Integrity(1));
}

TEST_CASE(SUITE("UnsolDisableEnableOnStartup"))
{
	MasterConfig config;
	config.DoUnsolOnStartup = true;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	// create a generic response packet with no IIN bits set
	std::string rsp = "C0 81 00 00";

	// disable unsol on grp 60 var2, var3, var4
	REQUIRE("C0 15 3C 02 06 3C 03 06 3C 04 06" ==  t.Read());
	t.RespondToMaster(rsp);

	TestForIntegrityPoll(t);

	// enable unsol
	REQUIRE("C0 14 3C 02 06 3C 03 06 3C 04 06" ==  t.Read());
	t.RespondToMaster(rsp);

	// check that the master sends no more packets
	REQUIRE(t.app.NumAPDU() ==  0);
}

TEST_CASE(SUITE("RestartAndTimeBits"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100;

	TestForIntegrityAndRespond(t, "C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN
	t.RespondToMaster("C0 81 10 00"); // need time

	REQUIRE("C0 17" ==  t.Read()); ; //Delay measure
	t.fixedUTC.mTimeSinceEpoch += 100; //advance time by 100ms so that the master sees 100ms for a response
	t.RespondToMaster("C0 81 10 00 34 02 07 01 0A 00"); // still need time, 52 Var 2, delay == 10ms

	// Write group 50 var 1
	// 200-100-10/2 = 45 => 45 + 200 - 0xF5
	REQUIRE("C0 02 32 01 07 01 F5 00 00 00 00 00" ==  t.Read());
	t.RespondToMaster("C0 81 00 00"); // time bit is now clear

	REQUIRE(t.app.NumAPDU() ==  0); // no more packets
}

TEST_CASE(SUITE("RestartFailure"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100; //100 ms since epoch

	TestForIntegrityAndRespond(t, "C0 81 90 00"); // need time and device restart

	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN

	t.master.OnSolFailure();
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN
}

TEST_CASE(SUITE("RestartLayerDown"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	TestForIntegrityAndRespond(t, "C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN

	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN
}

TEST_CASE(SUITE("DelayMeasLayerDown"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	TestForIntegrityAndRespond(t, "C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN
	t.RespondToMaster("C0 81 10 00"); // need time

	REQUIRE("C0 17" ==  t.Read()); //Delay measure
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	REQUIRE("C0 17" ==  t.Read()); //Delay measure
}

TEST_CASE(SUITE("DelayMeasFailure"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	TestForIntegrityAndRespond(t, "C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN
	t.RespondToMaster("C0 81 10 00"); // need time

	REQUIRE("C0 17" ==  t.Read()); //Delay measure
	t.master.OnSolFailure();
	REQUIRE("C0 17" ==  t.Read()); //Delay measure
}

TEST_CASE(SUITE("RestartBadResponses"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100; //100 ms since epoch

	TestForIntegrityAndRespond(t, "C0 81 10 00"); //need time

	REQUIRE("C0 17" ==  t.Read()); // Delay measure
	t.RespondToMaster("C0 81 10 00"); // no header

	REQUIRE("C0 17" ==  t.Read()); // retry
	t.RespondToMaster("C0 81 10 00 3C 01 06"); // wrong header

	REQUIRE("C0 17" ==  t.Read()); // retry
	t.RespondToMaster("C0 81 10 00 34 02 07 02 0A 00 03 00"); // too many objects

	REQUIRE("C0 17" ==  t.Read()); ; //Delay measure
	t.fixedUTC.mTimeSinceEpoch += 100; //advance time by 100ms so that the master sees 100ms for a response
	t.RespondToMaster("C0 81 10 00 34 02 07 01 90 01"); // still need time, 52 Var 2, delay == 400ms

	// Write group 50 var 1
	// 400 > 200, so delay is 0 + 200
	REQUIRE("C0 02 32 01 07 01 C8 00 00 00 00 00" ==  t.Read());
	t.RespondToMaster("C0 81 00 00"); // time bit is now clear

	REQUIRE(t.app.NumAPDU() ==  0); // no more packets
}
*/

TEST_CASE(SUITE("ControlExecutionClosedState"))
{
	MasterParams params;
	NewMasterTestObject t(params);

	auto pCmdProcessor = &t.master.GetCommandProcessor();

	ControlRelayOutputBlock bo(ControlCode::PULSE);
	MockCommandCallback callback;

	for(int i = 0; i < 10; ++i)
	{		
		pCmdProcessor->SelectAndOperate(bo, 1, &callback);
		t.exe.Dispatch();
		REQUIRE(1 == callback.responses.size());
		REQUIRE((CommandResponse(CommandResult::NO_COMMS) == callback.responses.front()));
		callback.responses.pop_front();
	}

}

TEST_CASE(SUITE("SelectAndOperate"))
{	
	NewMasterTestObject t(NoStartupTasks());
	t.master.OnLowerLayerUp();

	ControlRelayOutputBlock bo(ControlCode::PULSE);

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(bo, 1, &callback);
	t.exe.Dispatch();

	// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
	std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";

	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 03 " + crob); // SELECT
	t.master.OnSendResult(true);
	t.SendToMaster("C0 81 00 00 " + crob);

	t.exe.Dispatch();

	REQUIRE(t.lower.PopWriteAsHex() == "C1 04 " + crob); // OPERATE
	t.master.OnSendResult(true);
	t.SendToMaster("C1 81 00 00 " + crob);

	t.exe.Dispatch();

	REQUIRE(t.lower.PopWriteAsHex() == ""); //nore more packets
	REQUIRE(1 == callback.responses.size());
	REQUIRE((CommandResponse::OK(CommandStatus::SUCCESS) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectTimeout"))
{
	auto config = NoStartupTasks();
	NewMasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.Dispatch();

	REQUIRE(t.lower.PopWriteAsHex() == "C0 03 " + crob); // SELECT
	t.master.OnSendResult(true);

	t.exe.AdvanceTime(config.responseTimeout);
	t.exe.Dispatch();

	REQUIRE(1 == callback.responses.size());
	REQUIRE((CommandResponse(CommandResult::TIMEOUT) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectLayerDown"))
{
	auto config = NoStartupTasks();
	NewMasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.Dispatch();

	REQUIRE(t.lower.PopWriteAsHex() == "C0 03 " + crob); // SELECT
	t.master.OnSendResult(true);

	t.master.OnLowerLayerDown();

	REQUIRE(1 == callback.responses.size());
	REQUIRE((CommandResponse(CommandResult::NO_COMMS) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectErrorResponse"))
{
	auto config = NoStartupTasks();
	NewMasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.Dispatch();
	t.master.OnSendResult(true);
	t.SendToMaster("C0 81 00 00 0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 04"); // not supported

	REQUIRE(t.exe.Dispatch() > 0);

	REQUIRE(1 ==  callback.responses.size());
	REQUIRE((CommandResponse::OK(CommandStatus::NOT_SUPPORTED) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectPartialResponse"))
{
	auto config = NoStartupTasks();
	NewMasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.Dispatch();
	t.master.OnSendResult(true);

	t.SendToMaster("80 81 00 00 0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00");

	REQUIRE(t.exe.Dispatch() > 0);

	REQUIRE(1 ==  callback.responses.size());
	REQUIRE((CommandResponse(CommandResult::BAD_RESPONSE) == callback.responses.front()));
}

TEST_CASE(SUITE("DeferredControlExecution"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.unsolClassMask = 0;
	NewMasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.Dispatch() > 0);

	// check that a read request was made on startup
	REQUIRE(t.lower.PopWriteAsHex() == Integrity(0));
	t.master.OnSendResult(true);
	
	//issue a command while the master is waiting for a response from the outstation
	ControlRelayOutputBlock bo(ControlCode::PULSE);
	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(bo, 1, &callback);
	REQUIRE(t.exe.Dispatch() > 0);

	t.SendToMaster("C0 81 00 00"); //now master gets response to integrity

	REQUIRE(t.exe.Dispatch() > 0);
	
	REQUIRE(t.lower.PopWriteAsHex() == "C1 03 " + crob); //select
}


TEST_CASE(SUITE("CloseWhileWaitingForCommandResponse"))
{
	auto config = NoStartupTasks();
	NewMasterTestObject t(config);
	t.master.OnLowerLayerUp();	

	AnalogOutputInt16 ao(100);
	MockCommandCallback callback;

	t.master.GetCommandProcessor().DirectOperate(ao, 1, &callback);
	REQUIRE(t.exe.Dispatch() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == "C0 05 29 02 28 01 00 01 00 64 00 00"); // DIRECT OPERATE
	REQUIRE(t.lower.NumWrites() == 0); //nore more packets
	REQUIRE(callback.responses.empty());
	t.master.OnLowerLayerDown();
	REQUIRE(1 == callback.responses.size());		
}

/*
TEST_CASE(SUITE("SingleSetpointExecution"))// Group 41 Var4
{
	// 100.0
	TestAnalogOutputExecution("29 03 28 01 00 01 00 00 00 C8 42 00", AnalogOutputFloat32(100.0f));

	// 95.6
	TestAnalogOutputExecution("29 03 28 01 00 01 00 33 33 BF 42 00", AnalogOutputFloat32(95.6f));
}

TEST_CASE(SUITE("DoubleSetpointExecution"))
{
	TestAnalogOutputExecution("29 04 28 01 00 01 00 00 00 00 E7 FF FF 58 48 00", AnalogOutputDouble64(SingleFloat::Max * 100.0));
}

TEST_CASE(SUITE("Int32SetpointExecution"))
{
	// Group 41 Var1, Int32, 65536
	TestAnalogOutputExecution("29 01 28 01 00 01 00 00 00 01 00 00", AnalogOutputInt32(65536));
}

TEST_CASE(SUITE("Int16SetpointExecution"))
{
	// Group 41 Var2, Int16, 100
	TestAnalogOutputExecution("29 02 28 01 00 01 00 64 00 00", AnalogOutputInt16(100));
}

TEST_CASE(SUITE("SolicitedResponseWithData"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	TestForIntegrityAndRespond(t, "C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetBinary(2)));
}

TEST_CASE(SUITE("SolicitedResponseFailure"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();


	TestForIntegrityPoll(t, false);

	t.mts.AdvanceTime(TimeDuration(config.TaskRetryRate));
	REQUIRE(t.mts.DispatchOne());

	TestForIntegrityPoll(t);
}

TEST_CASE(SUITE("SolicitedResponseLayerDown"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	REQUIRE(t.Read() ==  INTEGRITY);
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	REQUIRE(t.Read() ==  INTEGRITY);
}

TEST_CASE(SUITE("SolicitedMultiFragResponse"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	REQUIRE(t.Read() ==  INTEGRITY);

	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81", false); //trigger partial response

	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetBinary(2)));

	REQUIRE(0 ==  t.app.NumAPDU());

	t.RespondToMaster("C0 81 00 00 01 02 00 03 03 02");
	REQUIRE((Binary(false, BQ_RESTART) ==  t.meas.GetBinary(3)));
}

TEST_CASE(SUITE("EventPoll"))
{
	MasterConfig config;
	MasterTestObject t(config);

	t.master.AddClassScan(CLASS_1 | CLASS_2, TimeDuration::Milliseconds(10), TimeDuration::Seconds(1));
	t.master.AddClassScan(CLASS_3, TimeDuration::Milliseconds(10), TimeDuration::Seconds(1));

	t.master.OnLowerLayerUp();

	TestForIntegrityAndRespond(t, "C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	REQUIRE(t.Read() ==  "C0 01 3C 02 06 3C 03 06");
	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	REQUIRE(t.Read() ==  "C0 01 3C 04 06");
	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetBinary(2)));
}

TEST_CASE(SUITE("ParsesOctetStringResponseWithFiveCharacters"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);

	// Group 111 (0x6F) Variation (length), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	t.SendUnsolToMaster("C0 82 00 00 6F 05 17 01 04 68 65 6C 6C 6F");

	REQUIRE(t.mts.DispatchOne());

	REQUIRE("68 65 6C 6C 6F" ==  toHex(t.meas.GetEventOctetString(4).ToReadOnly()));
}

TEST_CASE(SUITE("ParsesOctetStringResponseSizeOfOne"))
{
	MasterConfig config;
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	// octet strings shouldn't be found in class 0 polls, but we'll test that we can process them anyway
	// Group 110 (0x6E) Variation(length), start = 3, stop = 3
	TestForIntegrityAndRespond(t, "C0 81 00 00 6E 01 00 03 03 AA");

	REQUIRE("AA" ==  toHex(t.meas.GetOctetString(3).ToReadOnly()));
}
*/


