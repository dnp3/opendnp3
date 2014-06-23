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

using namespace opendnp3;
using namespace openpal;

// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";

#define SUITE(name) "MasterCommandRequestsTestSuite - " name

TEST_CASE(SUITE("ControlExecutionClosedState"))
{
	MasterParams params;
	MasterTestObject t(params);

	auto pCmdProcessor = &t.master.GetCommandProcessor();

	ControlRelayOutputBlock bo(ControlCode::PULSE);
	MockCommandCallback callback;

	for(int i = 0; i < 10; ++i)
	{		
		pCmdProcessor->SelectAndOperate(bo, 1, &callback);
		t.exe.RunMany();
		REQUIRE(1 == callback.responses.size());
		REQUIRE((CommandResponse(CommandResult::NO_COMMS) == callback.responses.front()));
		callback.responses.pop_front();
	}

}

TEST_CASE(SUITE("SelectAndOperate"))
{	
	MasterTestObject t(NoStartupTasks());
	t.master.OnLowerLayerUp();

	ControlRelayOutputBlock bo(ControlCode::PULSE);

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(bo, 1, &callback);
	t.exe.RunMany();

	// Group 12 Var1, 1 byte count/index, index = 1, time on/off = 1000, CommandStatus::SUCCESS
	std::string crob = "0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00";

	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 03 " + crob); // SELECT
	t.master.OnSendResult(true);
	t.SendToMaster("C0 81 00 00 " + crob);

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == "C1 04 " + crob); // OPERATE
	t.master.OnSendResult(true);
	t.SendToMaster("C1 81 00 00 " + crob);

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == ""); //nore more packets
	REQUIRE(1 == callback.responses.size());
	REQUIRE((CommandResponse::OK(CommandStatus::SUCCESS) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectTimeout"))
{
	auto config = NoStartupTasks();
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == "C0 03 " + crob); // SELECT
	t.master.OnSendResult(true);

	t.exe.AdvanceTime(config.responseTimeout);
	t.exe.RunMany();

	REQUIRE(1 == callback.responses.size());
	REQUIRE((CommandResponse(CommandResult::TIMEOUT) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectLayerDown"))
{
	auto config = NoStartupTasks();
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == "C0 03 " + crob); // SELECT
	t.master.OnSendResult(true);

	t.master.OnLowerLayerDown();

	REQUIRE(1 == callback.responses.size());
	REQUIRE((CommandResponse(CommandResult::NO_COMMS) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectErrorResponse"))
{
	auto config = NoStartupTasks();
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.RunMany();
	t.master.OnSendResult(true);
	t.SendToMaster("C0 81 00 00 0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 04"); // not supported

	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(1 ==  callback.responses.size());
	REQUIRE((CommandResponse::OK(CommandStatus::NOT_SUPPORTED) == callback.responses.front()));
}

TEST_CASE(SUITE("ControlExecutionSelectPartialResponse"))
{
	auto config = NoStartupTasks();
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(ControlRelayOutputBlock(ControlCode::PULSE), 1, &callback);
	t.exe.RunMany();
	t.master.OnSendResult(true);

	t.SendToMaster("80 81 00 00 0C 01 28 01 00 01 00 01 01 64 00 00 00 64 00 00 00 00");

	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(1 ==  callback.responses.size());
	REQUIRE((CommandResponse(CommandResult::BAD_RESPONSE) == callback.responses.front()));
}

TEST_CASE(SUITE("DeferredControlExecution"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.unsolClassMask = 0;
	MasterTestObject t(params);
	t.master.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany() > 0);

	// check that a read request was made on startup
	REQUIRE(t.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	t.master.OnSendResult(true);
	
	//issue a command while the master is waiting for a response from the outstation
	ControlRelayOutputBlock bo(ControlCode::PULSE);
	MockCommandCallback callback;
	t.master.GetCommandProcessor().SelectAndOperate(bo, 1, &callback);
	REQUIRE(t.exe.RunMany() > 0);

	t.SendToMaster("C0 81 00 00"); //now master gets response to integrity

	REQUIRE(t.exe.RunMany() > 0);
	
	REQUIRE(t.lower.PopWriteAsHex() == "C1 03 " + crob); //select
}


TEST_CASE(SUITE("CloseWhileWaitingForCommandResponse"))
{
	auto config = NoStartupTasks();
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();	

	AnalogOutputInt16 ao(100);
	MockCommandCallback callback;

	t.master.GetCommandProcessor().DirectOperate(ao, 1, &callback);
	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == "C0 05 29 02 28 01 00 01 00 64 00 00"); // DIRECT OPERATE
	REQUIRE(t.lower.NumWrites() == 0); //nore more packets
	REQUIRE(callback.responses.empty());
	t.master.OnLowerLayerDown();
	REQUIRE(1 == callback.responses.size());		
}

template <class T>
void TestAnalogOutputExecution(const std::string& hex, const T& command)
{
	auto config = NoStartupTasks();
	MasterTestObject t(config);
	t.master.OnLowerLayerUp();

	MockCommandCallback callback;

	t.master.GetCommandProcessor().SelectAndOperate(command, 1, &callback);
	REQUIRE(t.exe.RunMany() > 0);

	REQUIRE(t.lower.PopWriteAsHex() == "C0 03 " + hex);
	t.master.OnSendResult(true);
	REQUIRE(callback.responses.empty());
	t.SendToMaster("C0 81 00 00 " + hex);

	t.exe.RunMany();

	REQUIRE(t.lower.PopWriteAsHex() == "C1 04 " + hex);
	t.master.OnSendResult(true);
	REQUIRE(callback.responses.empty());
	t.SendToMaster("C1 81 00 00 " + hex);

	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(callback.responses.size() == 1);
	REQUIRE(callback.responses.front() == CommandResponse::OK(CommandStatus::SUCCESS));
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




