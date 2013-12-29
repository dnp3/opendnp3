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
#include <boost/test/unit_test.hpp>

#include <openpal/Exception.h>
#include <openpal/Serialization.h>

#include <queue>

#include "TestHelpers.h"
#include "MasterTestObject.h"
#include "MeasurementComparisons.h"

using namespace opendnp3;
using namespace std::chrono;
using namespace openpal;

void TestForIntegrityPoll(MasterTestObject& t, bool aSucceed = true)
{
	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read());
	if(aSucceed) t.RespondToMaster("C0 81 00 00");
	else t.master.OnSolFailure();
}

void DoControlSelectAndOperate(MasterTestObject& t, std::function<void (CommandResponse)> callback)
{
	TestForIntegrityPoll(t);
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); // check that the master sends no more packets

	ControlRelayOutputBlock bo(ControlCode::PULSE); bo.status = CommandStatus::SUCCESS;
	t.master.GetCommandProcessor()->SelectAndOperate(bo, 1, callback);
	BOOST_REQUIRE(t.mts.DispatchOne());

	// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
	std::string crob = "0C 01 17 01 01 01 01 64 00 00 00 64 00 00 00 00";

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 03 " + crob);
}

template <class T>
void TestAnalogOutputExecution(const std::string& setpointhex, T ao)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); // check that the master sends no more packets

	t.master.GetCommandProcessor()->SelectAndOperate(ao, 1, [](CommandResponse cr) {});
	BOOST_REQUIRE(t.mts.DispatchOne());

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 03 " + setpointhex); // SELECT
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //nore more packets
}

BOOST_AUTO_TEST_SUITE(MasterTestSuite)

BOOST_AUTO_TEST_CASE(InitialState)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);	

	BOOST_REQUIRE_THROW(t.master.OnLowerLayerDown(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnSolSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnUnsolSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnSolFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnUnsolFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnPartialResponse(APDUResponseRecord()), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnFinalResponse(APDUResponseRecord()), InvalidStateException);
	BOOST_REQUIRE_THROW(t.master.OnUnsolResponse(APDUResponseRecord()), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(IntegrityOnStartup)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0);
}

BOOST_AUTO_TEST_CASE(StateTransitionSuccessFailure)
{
	MasterConfig cfg; cfg.IntegrityRate = TimeDuration::Seconds(1);
	MasterTestObject t(cfg);
	t.BindStateListener();
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.states.size(), 1);
	BOOST_REQUIRE(t.states.front() == StackState::COMMS_DOWN);
	t.states.pop_front();
	t.master.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.states.size(), 0);

	TestForIntegrityPoll(t);
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.states.size(), 1);
	BOOST_REQUIRE(t.states.front() == StackState::COMMS_UP);
	t.states.pop_front();

	t.mts.AdvanceTime(TimeDuration::Seconds(2));
	BOOST_REQUIRE(t.mts.DispatchOne());
	TestForIntegrityPoll(t, false);

	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.states.size(), 1);
	BOOST_REQUIRE(t.states.front() == StackState::COMMS_DOWN);
	t.states.pop_front();

	t.mts.AdvanceTime(TimeDuration::Seconds(10));
	BOOST_REQUIRE(t.mts.DispatchOne());
	TestForIntegrityPoll(t);

	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.states.size(), 1);
	BOOST_REQUIRE(t.states.front() == StackState::COMMS_UP);
	t.states.pop_front();

	t.master.OnLowerLayerDown();

	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.states.size(), 1);
	BOOST_REQUIRE(t.states.front() == StackState::COMMS_DOWN);
	t.states.pop_front();
}

BOOST_AUTO_TEST_CASE(UnsolDisableEnableOnStartup)
{
	MasterConfig master_cfg;
	master_cfg.DoUnsolOnStartup = true;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	// create a generic response packet with no IIN bits set
	std::string rsp = "C0 81 00 00";

	// disable unsol on grp 60 var2, var3, var4
	BOOST_REQUIRE_EQUAL("C0 15 3C 02 06 3C 03 06 3C 04 06", t.Read());
	t.RespondToMaster(rsp);

	TestForIntegrityPoll(t);

	// enable unsol
	BOOST_REQUIRE_EQUAL("C0 14 3C 02 06 3C 03 06 3C 04 06", t.Read());
	t.RespondToMaster(rsp);

	// check that the master sends no more packets
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0);
}

BOOST_AUTO_TEST_CASE(RestartAndTimeBits)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100;

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read()); ; //integrity
	t.RespondToMaster("C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN
	t.RespondToMaster("C0 81 10 00"); // need time

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); ; //Delay measure
	t.fixedUTC.mTimeSinceEpoch += 100; //advance time by 100ms so that the master sees 100ms for a response
	t.RespondToMaster("C0 81 10 00 34 02 07 01 0A 00"); // still need time, 52 Var 2, delay == 10ms

	// Write group 50 var 1
	// 200-100-10/2 = 45 => 45 + 200 - 0xF5
	BOOST_REQUIRE_EQUAL("C0 02 32 01 07 01 F5 00 00 00 00 00", t.Read());
	t.RespondToMaster("C0 81 00 00"); // time bit is now clear

	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); // no more packets
}

BOOST_AUTO_TEST_CASE(RestartFailure)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100; //100 ms since epoch

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read()); //integrity
	t.RespondToMaster("C0 81 90 00"); // need time and device restart

	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN

	t.master.OnSolFailure();
	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN
}

BOOST_AUTO_TEST_CASE(RestartLayerDown)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read()); //integrity
	t.RespondToMaster("C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN

	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN
}

BOOST_AUTO_TEST_CASE(DelayMeasLayerDown)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read()); //integrity
	t.RespondToMaster("C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN
	t.RespondToMaster("C0 81 10 00"); // need time

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); //Delay measure
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); //Delay measure
}

BOOST_AUTO_TEST_CASE(DelayMeasFailure)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read()); //integrity
	t.RespondToMaster("C0 81 90 00"); // need time and device restart

	// Device restart should happen before time task
	BOOST_REQUIRE_EQUAL("C0 02 50 01 00 07 07 00", t.Read()); //write IIN
	t.RespondToMaster("C0 81 10 00"); // need time

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); //Delay measure
	t.master.OnSolFailure();
	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); //Delay measure
}

BOOST_AUTO_TEST_CASE(RestartBadResponses)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	t.fixedUTC.mTimeSinceEpoch = 100; //100 ms since epoch

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read()); //integrity
	t.RespondToMaster("C0 81 10 00"); // need time

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); // Delay measure
	t.RespondToMaster("C0 81 10 00"); // no header

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); // retry
	t.RespondToMaster("C0 81 10 00 3C 01 06"); // wrong header

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); // retry
	t.RespondToMaster("C0 81 10 00 34 02 07 02 0A 00 03 00"); // too many objects

	BOOST_REQUIRE_EQUAL("C0 17", t.Read()); ; //Delay measure
	t.fixedUTC.mTimeSinceEpoch += 100; //advance time by 100ms so that the master sees 100ms for a response
	t.RespondToMaster("C0 81 10 00 34 02 07 01 90 01"); // still need time, 52 Var 2, delay == 400ms

	// Write group 50 var 1
	// 400 > 200, so delay is 0 + 200
	BOOST_REQUIRE_EQUAL("C0 02 32 01 07 01 C8 00 00 00 00 00", t.Read());
	t.RespondToMaster("C0 81 00 00"); // time bit is now clear

	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); // no more packets
}

BOOST_AUTO_TEST_CASE(ControlExecutionClosedState)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);

	auto pCmdProcessor = t.master.GetCommandProcessor();

	ControlRelayOutputBlock bo(ControlCode::PULSE);

	for(int i = 0; i < 10; ++i) {
		CommandResponse rsp;
		pCmdProcessor->SelectAndOperate(bo, 1, [&](CommandResponse r) {
			rsp = r;
		});
		t.mts.Dispatch();
		BOOST_REQUIRE(CommandResponse(CommandResult::NO_COMMS) == rsp);
	}

}

BOOST_AUTO_TEST_CASE(SelectAndOperate)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); // check that the master sends no more packets

	ControlRelayOutputBlock bo(ControlCode::PULSE); bo.status = CommandStatus::SUCCESS;

	std::vector<CommandResponse> rsps;
	t.master.GetCommandProcessor()->SelectAndOperate(bo, 1, [&](CommandResponse rsp) {
		rsps.push_back(rsp);
	});
	t.mts.Dispatch();

	// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
	std::string crob = "0C 01 17 01 01 01 01 64 00 00 00 64 00 00 00 00";

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 03 " + crob); // SELECT
	t.RespondToMaster("C0 81 00 00 " + crob);

	t.mts.DispatchOne();

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 04 " + crob); // OPERATE
	t.RespondToMaster("C0 81 00 00 " + crob);

	t.mts.DispatchOne();

	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //nore more packets
	BOOST_REQUIRE_EQUAL(1, rsps.size());
	BOOST_REQUIRE(CommandResponse::OK(CommandStatus::SUCCESS) == rsps[0]);
}

BOOST_AUTO_TEST_CASE(ControlExecutionSelectFailure)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;

	DoControlSelectAndOperate(t, [&](CommandResponse cr) {
		rsps.push_back(cr);
	});
	t.master.OnSolFailure();

	t.mts.DispatchOne();

	BOOST_REQUIRE_EQUAL(1, rsps.size());
	BOOST_REQUIRE(CommandResponse(CommandResult::TIMEOUT) == rsps[0]);
}

BOOST_AUTO_TEST_CASE(ControlExecutionSelectLayerDown)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;
	DoControlSelectAndOperate(t, [&](CommandResponse cr) {
		rsps.push_back(cr);
	});
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();

	t.mts.DispatchOne();

	BOOST_REQUIRE_EQUAL(1, rsps.size());
	BOOST_REQUIRE(CommandResponse(CommandResult::TIMEOUT) == rsps[0]);
}

BOOST_AUTO_TEST_CASE(ControlExecutionSelectErrorResponse)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;
	DoControlSelectAndOperate(t, [&](CommandResponse cr) {
		rsps.push_back(cr);
	});
	t.RespondToMaster("C0 81 00 00 0C 01 17 01 01 01 01 64 00 00 00 64 00 00 00 04"); // not supported

	t.mts.DispatchOne();

	BOOST_REQUIRE_EQUAL(1, rsps.size());
	BOOST_REQUIRE(CommandResponse::OK(CommandStatus::NOT_SUPPORTED) == rsps[0]);
}

BOOST_AUTO_TEST_CASE(ControlExecutionSelectPartialResponse)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	std::vector<CommandResponse> rsps;
	DoControlSelectAndOperate(t, [&](CommandResponse cr) {
		rsps.push_back(cr);
	});
	t.RespondToMaster("80 81 00 00 0C 01 17 01 01 01 01 64 00 00 00 64 00 00 00 00", false);

	BOOST_REQUIRE_EQUAL(0, rsps.size());

	t.RespondToMaster("C0 81 00 00 0C 01 17 01 01 01 01 64 00 00 00 64 00 00 00 04"); // not supported

	t.mts.DispatchOne();

	BOOST_REQUIRE_EQUAL(1, rsps.size());
	BOOST_REQUIRE(CommandResponse::OK(CommandStatus::NOT_SUPPORTED) == rsps[0]);
}

BOOST_AUTO_TEST_CASE(DeferredControlExecution)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	// check that a read request was made on startup
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06"); ;

	//issue a command while the master is waiting for a response from the slave
	ControlRelayOutputBlock bo(ControlCode::PULSE); bo.status = CommandStatus::SUCCESS;
	t.master.GetCommandProcessor()->SelectAndOperate(bo, 1, [](CommandResponse) {});
	BOOST_REQUIRE(t.mts.DispatchOne());

	t.RespondToMaster("C0 81 00 00"); //now master gets response to integrity
	std::string crob = "0C 01 17 01 01 01 01 64 00 00 00 64 00 00 00 00";
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 03 " + crob); //select
}

BOOST_AUTO_TEST_CASE(SingleSetpointExecution)// Group 41 Var4
{
	// 100.0
	TestAnalogOutputExecution("29 03 17 01 01 00 00 C8 42 00", AnalogOutputFloat32(100.0f));

	// 95.6
	TestAnalogOutputExecution("29 03 17 01 01 33 33 BF 42 00", AnalogOutputFloat32(95.6f));
}

BOOST_AUTO_TEST_CASE(DoubleSetpointExecution)
{
	TestAnalogOutputExecution("29 04 17 01 01 00 00 00 E7 FF FF 58 48 00", AnalogOutputDouble64(SingleFloat::Max * 100.0));
}

BOOST_AUTO_TEST_CASE(Int32SetpointExecution)
{
	// Group 41 Var1, Int32, 65536
	TestAnalogOutputExecution("29 01 17 01 01 00 00 01 00 00", AnalogOutputInt32(65536));
}

BOOST_AUTO_TEST_CASE(Int16SetpointExecution)
{
	// Group 41 Var2, Int16, 100
	TestAnalogOutputExecution("29 02 17 01 01 64 00 00", AnalogOutputInt16(100));
}

BOOST_AUTO_TEST_CASE(SolicitedResponseWithData)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06"); ;
	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	BOOST_REQUIRE(t.mts.DispatchOne()); //disptach measurement callback

	BOOST_REQUIRE(Binary(true, BQ_ONLINE) == t.meas.GetBinary(2));
}

BOOST_AUTO_TEST_CASE(SolicitedResponseFailure)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06"); ;
	t.master.OnSolFailure();
	t.mts.AdvanceTime(TimeDuration(master_cfg.TaskRetryRate));
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06"); ;
}

BOOST_AUTO_TEST_CASE(SolicitedResponseLayerDown)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06"); ;
	t.master.OnLowerLayerDown();
	t.master.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06"); ;
}

BOOST_AUTO_TEST_CASE(SolicitedMultiFragResponse)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06");

	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81", false); //trigger partial response

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch measurement callback

	BOOST_REQUIRE(Binary(true, BQ_ONLINE) == t.meas.GetBinary(2));

	BOOST_REQUIRE_EQUAL(0, t.app.NumAPDU());

	t.RespondToMaster("C0 81 00 00 01 02 00 03 03 02");
	BOOST_REQUIRE(t.mts.DispatchOne()); //disptch measurement callback
	BOOST_REQUIRE(Binary(false, BQ_RESTART) ==  t.meas.GetBinary(3));
}

BOOST_AUTO_TEST_CASE(EventPoll)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);

	t.master.AddClassScan(PC_CLASS_1 | PC_CLASS_2, TimeDuration::Milliseconds(10), TimeDuration::Seconds(1));
	t.master.AddClassScan(PC_CLASS_3, TimeDuration::Milliseconds(10), TimeDuration::Seconds(1));

	t.master.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 01 06");
	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	BOOST_REQUIRE(t.mts.DispatchOne());

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 02 06 3C 03 06");
	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	BOOST_REQUIRE(t.mts.DispatchOne());

	BOOST_REQUIRE_EQUAL(t.Read(), "C0 01 3C 04 06");
	t.RespondToMaster("C0 81 00 00 01 02 00 02 02 81"); //group 2 var 1, index = 2, 0x81 = Online, true

	BOOST_REQUIRE(t.mts.DispatchOne());

	BOOST_REQUIRE(Binary(true, BQ_ONLINE) == t.meas.GetBinary(2));
}

BOOST_AUTO_TEST_CASE(ParsesOctetStringResponseWithFiveCharacters)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();

	TestForIntegrityPoll(t);

	// Group 111 (0x6F) Variation (length), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	t.SendUnsolToMaster("C0 82 00 00 6F 05 17 01 04 68 65 6C 6C 6F");

	BOOST_REQUIRE(t.mts.DispatchOne());

	BOOST_REQUIRE_EQUAL("hello", t.meas.GetOctetString(4).AsString());
}

BOOST_AUTO_TEST_CASE(ParsesOctetStringResponseWithNoCharacters)
{
	MasterConfig master_cfg;
	MasterTestObject t(master_cfg);
	t.master.OnLowerLayerUp();	

	// octet strings shouldn't be found in class 0 polls, but we'll test that we can process them anyway
	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", t.Read());

	// Group 110 (0x6E) Variation (length), start = 3, stop = 3
	t.RespondToMaster("C0 81 00 00 6E 00 00 03 03");

	BOOST_REQUIRE(t.mts.DispatchOne());

	BOOST_REQUIRE_EQUAL("", t.meas.GetOctetString(3).AsString());
}

BOOST_AUTO_TEST_SUITE_END() //end suite


