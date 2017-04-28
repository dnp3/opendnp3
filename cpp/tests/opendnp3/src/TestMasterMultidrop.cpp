/*
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

#include "mocks/MasterTestObject.h"

#include <testlib/HexConversions.h>
#include <dnp3mocks/APDUHexBuilders.h>

using namespace openpal;
using namespace opendnp3;

// helpers

void ExpectRequestAndCauseResponseTimeout(MasterTestObject&, const std::string& expected);
void ExpectRequestAndRespond(MasterTestObject&, const std::string& expected, const std::string& response);

#define SUITE(name) "MasterMultidropTestSuite - " name

TEST_CASE(SUITE("Multidrop scheduling is priroity based"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;

	const auto executor = std::make_shared<testlib::MockExecutor>();
	const auto scheduler = std::make_shared<opendnp3::MasterSchedulerBackend>(executor);
	const auto log = std::make_shared<testlib::MockLogHandlerImpl>();

	MasterTestObject t1(params, "s1", log, executor, scheduler);
	MasterTestObject t2(params, "s2", log, executor, scheduler);

	t1.context->OnLowerLayerUp();
	t2.context->OnLowerLayerUp();

	REQUIRE(executor->RunMany() > 0);

	REQUIRE(t1.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
	REQUIRE(t2.lower->PopWriteAsHex() == "");

	t1.context->OnSendResult(true);
	t1.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::DEVICE_RESTART)));

	REQUIRE(executor->RunMany() > 0);

	// The IIN clear has higher priority than the integrity poll, so it is run first

	REQUIRE(t1.lower->PopWriteAsHex() == hex::ClearRestartIIN(1));
	REQUIRE(t2.lower->PopWriteAsHex() == "");

	t1.context->OnSendResult(true);
	t1.SendToMaster(hex::EmptyResponse(1));

	REQUIRE(executor->RunMany() > 0);

	// Finally, the 2nd master gets to run it's integrity poll

	REQUIRE(t1.lower->PopWriteAsHex() == "");
	REQUIRE(t2.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("Shutting down a master causes 2nd master to run scheduled task"))
{

	MasterParams params;
	params.disableUnsolOnStartup = false;

	const auto executor = std::make_shared<testlib::MockExecutor>();
	const auto scheduler = std::make_shared<opendnp3::MasterSchedulerBackend>(executor);
	const auto log = std::make_shared<testlib::MockLogHandlerImpl>();

	MasterTestObject t1(params, "s1", log, executor, scheduler);
	MasterTestObject t2(params, "s2", log, executor, scheduler);

	t1.context->OnLowerLayerUp();
	t2.context->OnLowerLayerUp();

	REQUIRE(executor->RunMany() > 0);

	REQUIRE(t1.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
	REQUIRE(t2.lower->PopWriteAsHex() == "");

	t1.context->OnSendResult(true);
	// instead of sending a reply, shutdown the first master
	t1.context->OnLowerLayerDown();

	REQUIRE(executor->RunMany() > 0);

	REQUIRE(t1.lower->PopWriteAsHex() == "");
	REQUIRE(t2.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
}

TEST_CASE(SUITE("Scheduler executes other session's tasks if a session is timing out"))
{
	MasterParams params;

	const auto executor = std::make_shared<testlib::MockExecutor>();
	const auto scheduler = std::make_shared<opendnp3::MasterSchedulerBackend>(executor);
	const auto log = std::make_shared<testlib::MockLogHandlerImpl>();

	MasterTestObject t1(params, "s1", log, executor, scheduler);
	MasterTestObject t2(params, "s2", log, executor, scheduler);

	t1.context->OnLowerLayerUp();
	t2.context->OnLowerLayerUp();

	REQUIRE(executor->RunMany() > 0);

	ExpectRequestAndCauseResponseTimeout(t1, hex::DisableUnsol(0));
	ExpectRequestAndRespond(t2, hex::DisableUnsol(0), hex::EmptyResponse(0));

	// now session 2 should be able to run its integrity poll
	ExpectRequestAndRespond(t2, hex::IntegrityPoll(1), hex::EmptyResponse(1));

}

TEST_CASE(SUITE("Scheduler still does integrity polls if exception scan set to high interval"))
{
	const auto executor = std::make_shared<testlib::MockExecutor>();
	const auto scheduler = std::make_shared<opendnp3::MasterSchedulerBackend>(executor);
	const auto log = std::make_shared<testlib::MockLogHandlerImpl>();


	MasterTestObject t1(NoStartupTasks(), "s1", log, executor, scheduler);
	MasterTestObject t2(NoStartupTasks(), "s2", log, executor, scheduler);

	auto integrity1 = t1.context->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(10));
	auto event2 = t2.context->AddClassScan(ClassField::AllEventClasses(), TimeDuration::Seconds(100000));
	auto integrity2 = t2.context->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(15));


	t1.context->OnLowerLayerUp();
	t2.context->OnLowerLayerUp();

	REQUIRE(executor->RunMany() > 0);

	// All of the configured scans should execute immediately, in the order the layers go online, followed by the order they were added
	ExpectRequestAndRespond(t1, hex::IntegrityPoll(0), hex::EmptyResponse(0));
	ExpectRequestAndRespond(t2, hex::EventPoll(0), hex::EmptyResponse(0));
	ExpectRequestAndRespond(t2, hex::IntegrityPoll(1), hex::EmptyResponse(1));

	// ---- after all of the initial polls, they should then execute based on the expired periods ----

	// first up is the integrity poll for S1
	REQUIRE(executor->AdvanceToNextTimer());
	REQUIRE(executor->GetTime().milliseconds == 10000);
	REQUIRE(executor->RunMany() > 0);
	ExpectRequestAndRespond(t1, hex::IntegrityPoll(1), hex::EmptyResponse(1));

	// next is the integrity poll for S2
	REQUIRE(executor->AdvanceToNextTimer());
	REQUIRE(executor->GetTime().milliseconds == 15000);
	REQUIRE(executor->RunMany() > 0);
	ExpectRequestAndRespond(t2, hex::IntegrityPoll(2), hex::EmptyResponse(2));

	// then the poll for S1 occurs again at t = 20000
	REQUIRE(executor->AdvanceToNextTimer());
	REQUIRE(executor->GetTime().milliseconds == 20000);
	REQUIRE(executor->RunMany() > 0);
	ExpectRequestAndRespond(t1, hex::IntegrityPoll(2), hex::EmptyResponse(2));
}

void ExpectRequestAndCauseResponseTimeout(MasterTestObject& session, const std::string& expected)
{
	REQUIRE(session.lower->PopWriteAsHex() == expected);
	REQUIRE(session.context->OnSendResult(true));
	REQUIRE(session.exe->AdvanceToNextTimer());
	REQUIRE(session.exe->RunMany() > 0);
}

void ExpectRequestAndRespond(MasterTestObject& session, const std::string& expected, const std::string& response)
{
	REQUIRE(session.lower->PopWriteAsHex() == expected);
	REQUIRE(session.context->OnSendResult(true));
	session.SendToMaster(response);
	REQUIRE(session.exe->RunMany() > 0);
}