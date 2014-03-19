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

#include <openpal/Serialization.h>

#include <queue>

#include "Exception.h"

#include "MasterTestObject.h"
#include "MeasurementComparisons.h"

using namespace opendnp3;
using namespace openpal;

const std::string INTEGRITY("C0 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06");

void TestForIntegrityPoll(MasterTestObject& t, bool aSucceed = true)
{
	REQUIRE(INTEGRITY ==  t.Read());
	if(aSucceed) t.RespondToMaster("C0 81 00 00");
	else t.master.OnSolFailure();
}

void TestForIntegrityAndRespond(MasterTestObject& t, const std::string& response)
{
	REQUIRE(INTEGRITY ==  t.Read());
	t.RespondToMaster(response);
}

void DoControlSelectAndOperate(MasterTestObject& t, std::function<void (CommandResponse)> callback)
{
	TestForIntegrityPoll(t);
	REQUIRE(t.app.NumAPDU() ==  0); // check that the master sends no more packets

	ControlRelayOutputBlock bo(ControlCode::PULSE); bo.status = CommandStatus::SUCCESS;
	t.master.GetCommandProcessor()->SelectAndOperate(bo, 1, callback);
	REQUIRE(t.mts.DispatchOne());

	// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
	std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";

	REQUIRE(t.Read() ==  "C0 03 " + crob);
}

template <class T>
void TestAnalogOutputExecution(const std::string& setpointhex, T ao)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	REQUIRE(t.app.NumAPDU() ==  0); // check that the master sends no more packets

	t.master.GetCommandProcessor()->SelectAndOperate(ao, 1, [](CommandResponse cr) {});
	REQUIRE(t.mts.DispatchOne());

	REQUIRE(t.Read() ==  "C0 03 " + setpointhex); // SELECT
	REQUIRE(t.app.NumAPDU() ==  0); //nore more packets
}

#define SUITE(name) "MasterTestSuite - " name

TEST_CASE(SUITE("InitialState"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);

	t.master.OnLowerLayerDown();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnSolSendSuccess();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnUnsolSendSuccess();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnSolFailure();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnUnsolFailure();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnPartialResponse(APDUResponseRecord());
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnFinalResponse(APDUResponseRecord());
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.master.OnUnsolResponse(APDUResponseRecord());
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
}

TEST_CASE(SUITE("IntegrityOnStartup"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	REQUIRE(t.app.NumAPDU() ==  0);
}

TEST_CASE(SUITE("UnsolDisableEnableOnStartup"))
{
	MasterConfig master_cfg;
	master_cfg.DoUnsolOnStartup = true;
	MasterTestObject t(master_cfg);
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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100; //100 ms since epoch

	TestForIntegrityAndRespond(t, "C0 81 90 00"); // need time and device restart

	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN

	t.master.OnSolFailure();
	REQUIRE("C0 02 50 01 00 07 07 00" ==  t.Read()); //write IIN
}

TEST_CASE(SUITE("RestartLayerDown"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
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

TEST_CASE(SUITE("ControlExecutionClosedState"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);

	auto pCmdProcessor = t.master.GetCommandProcessor();

	ControlRelayOutputBlock bo(ControlCode::PULSE);

	for(int i = 0; i < 10; ++i)
	{
		CommandResponse rsp;
		pCmdProcessor->SelectAndOperate(bo, 1, [&](CommandResponse r)
		{
			rsp = r;
		});
		t.mts.Dispatch();
		REQUIRE((CommandResponse(CommandResult::NO_COMMS) == rsp));
	}

}

TEST_CASE(SUITE("SelectAndOperate"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	REQUIRE(t.app.NumAPDU() ==  0); // check that the master sends no more packets

	ControlRelayOutputBlock bo(ControlCode::PULSE); bo.status = CommandStatus::SUCCESS;

	std::vector<CommandResponse> rsps;
	t.master.GetCommandProcessor()->SelectAndOperate(bo, 1, [&](CommandResponse rsp)
	{
		rsps.push_back(rsp);
	});
	t.mts.Dispatch();

	// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
	std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";

	REQUIRE(t.Read() ==  "C0 03 " + crob); // SELECT
	t.RespondToMaster("C0 81 00 00 " + crob);

	t.mts.DispatchOne();

	REQUIRE(t.Read() ==  "C0 04 " + crob); // OPERATE
	t.RespondToMaster("C0 81 00 00 " + crob);

	t.mts.DispatchOne();

	REQUIRE(t.app.NumAPDU() ==  0); //nore more packets
	REQUIRE(1 ==  rsps.size());
	REQUIRE((CommandResponse::OK(CommandStatus::SUCCESS) == rsps[0]));
}

TEST_CASE(SUITE("ControlExecutionSelectFailure"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;

	DoControlSelectAndOperate(t, [&](CommandResponse cr)
	{
		rsps.push_back(cr);
	});
	t.master.OnSolFailure();

	t.mts.DispatchOne();

	REQUIRE(1 ==  rsps.size());
	REQUIRE((CommandResponse(CommandResult::TIMEOUT) == rsps[0]));
}

TEST_CASE(SUITE("ControlExecutionSelectLayerDown"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;
	DoControlSelectAndOperate(t, [&](CommandResponse cr)
	{
		rsps.push_back(cr);
	});
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();

	t.mts.DispatchOne();

	REQUIRE(1 ==  rsps.size());
	REQUIRE((CommandResponse(CommandResult::TIMEOUT) == rsps[0]));
}

TEST_CASE(SUITE("ControlExecutionSelectErrorResponse"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;
	DoControlSelectAndOperate(t, [&](CommandResponse cr)
	{
		rsps.push_back(cr);
	});
	t.RespondToMaster("C0 81 00 00 0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 04"); // not supported

	t.mts.DispatchOne();

	REQUIRE(1 ==  rsps.size());
	REQUIRE((CommandResponse::OK(CommandStatus::NOT_SUPPORTED) == rsps[0]));
}

TEST_CASE(SUITE("ControlExecutionSelectPartialResponse"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;
	DoControlSelectAndOperate(t, [&](CommandResponse cr)
	{
		rsps.push_back(cr);
	});

	t.RespondToMaster("80 81 00 00 0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00", false);

	REQUIRE(t.mts.DispatchOne());

	REQUIRE(1 ==  rsps.size());
	REQUIRE((CommandResponse(CommandResult::BAD_RESPONSE) == rsps[0]));
}

TEST_CASE(SUITE("DeferredControlExecution"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	// check that a read request was made on startup
	TestForIntegrityPoll(t);

	//issue a command while the master is waiting for a response from the slave
	ControlRelayOutputBlock bo(ControlCode::PULSE); bo.status = CommandStatus::SUCCESS;
	t.master.GetCommandProcessor()->SelectAndOperate(bo, 1, [](CommandResponse) {});
	REQUIRE(t.mts.DispatchOne());

	t.RespondToMaster("C0 81 00 00"); //now master gets response to integrity
	std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";
	REQUIRE(t.Read() ==  "C0 03 " + crob); //select
}

TEST_CASE(SUITE("CloseWhileWaitingForCommandResponse"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	REQUIRE(t.app.NumAPDU() == 0); // check that the master sends no more packets

	AnalogOutputInt16 ao(100);
	std::deque<CommandResponse> responses;

	t.master.GetCommandProcessor()->DirectOperate(ao, 1, [&](CommandResponse cr) { responses.push_back(cr); });
	REQUIRE(t.mts.DispatchOne());	

	REQUIRE(t.Read() == "C0 05 29 02 28 01 00 01 00 64 00 00"); // DIRECT OPERATE
	REQUIRE(t.app.NumAPDU() == 0); //nore more packets	
	REQUIRE(responses.empty());

	t.master.OnLowerLayerDown();
	REQUIRE(1 == responses.size());
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
}

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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityAndRespond(t, "C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	REQUIRE((Binary(true, BQ_ONLINE) == t.meas.GetBinary(2)));
}

TEST_CASE(SUITE("SolicitedResponseFailure"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();


	TestForIntegrityPoll(t, false);

	t.mts.AdvanceTime(TimeDuration(master_cfg.TaskRetryRate));
	REQUIRE(t.mts.DispatchOne());

	TestForIntegrityPoll(t);
}

TEST_CASE(SUITE("SolicitedResponseLayerDown"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	REQUIRE(t.Read() ==  INTEGRITY);
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	REQUIRE(t.Read() ==  INTEGRITY);
}

TEST_CASE(SUITE("SolicitedMultiFragResponse"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);

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
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);

	// Group 111 (0x6F) Variation (length), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	t.SendUnsolToMaster("C0 82 00 00 6F 05 17 01 04 68 65 6C 6C 6F");

	REQUIRE(t.mts.DispatchOne());

	REQUIRE("hello" ==  t.meas.GetEventOctetString(4).AsString());
}

TEST_CASE(SUITE("ParsesOctetStringResponseWithNoCharacters"))
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	// octet strings shouldn't be found in class 0 polls, but we'll test that we can process them anyway
	// Group 110 (0x6E) Variation(length), start = 3, stop = 3
	TestForIntegrityAndRespond(t, "C0 81 00 00 6E 00 00 03 03");

	REQUIRE("" ==  t.meas.GetOctetString(3).AsString());
}




