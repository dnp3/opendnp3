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

#include "fixtures/MasterSecAuthFixture.h"

#include <dnp3mocks/APDUHexBuilders.h>
#include <dnp3mocks/CallbackQueue.h>

#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace secauth;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "MasterSessionKeySuite - " name

TEST_CASE(SUITE("ChangeSessionKeys-AES128-SHA1-10"))
{
	MasterParams params;
	User user = User::Default();
	MasterSecAuthFixture fixture(params);
	REQUIRE(fixture.ConfigureUser(User::Default()));

	fixture.context.OnLowerLayerUp();

	AppSeqNum seq;

	fixture.TestSessionKeyExchange(seq, user);

	// next task should be diable unsol w/ this configuration
	REQUIRE(fixture.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, seq, ClassField::AllEventClasses()));
}

TEST_CASE(SUITE("Session keys are refreshed at the cofigured interval"))
{
	MasterParams params;
	// don't do any other tasks after the key exchange
	params.disableUnsolOnStartup = false;
	params.unsolClassMask = ClassField();
	params.startupIntegrityClassMask = ClassField();

	User user = User::Default();
	MasterAuthSettings settings;
	settings.sessionChangeInterval = TimeDuration::Seconds(5);

	MasterSecAuthFixture fixture(params, settings);
	REQUIRE(fixture.ConfigureUser(User::Default()));

	fixture.context.OnLowerLayerUp();


	AppSeqNum seq;
	fixture.TestSessionKeyExchange(seq, user);

	REQUIRE(fixture.lower.HasNoData());

	// check that advancing the timer starts up the next session key change
	REQUIRE(fixture.exe.AdvanceToNextTimer());
	REQUIRE(fixture.exe.GetTime().milliseconds == 5000);

	fixture.TestSessionKeyExchange(seq, user);
}

TEST_CASE(SUITE("Master calls back application with result of session key change"))
{
	MasterParams params;
	User user = User(7);
	MasterSecAuthFixture fixture(params);
	fixture.ConfigureUser(user);

	fixture.context.OnLowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyExchange(seq, user);

	REQUIRE(fixture.application.completions.size() == 1);
	auto info = fixture.application.completions.front();
	REQUIRE(info.result == TaskCompletion::SUCCESS);
	REQUIRE(info.type == MasterTaskType::SET_SESSION_KEYS);
	REQUIRE(info.user.GetId() == 7);
}

TEST_CASE(SUITE("Master authenticates using default user"))
{
	MasterParams params;
	User user = User::Default();
	MasterSecAuthFixture fixture(params);
	fixture.ConfigureUser(user);

	fixture.context.OnLowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyExchange(seq, user);

	// next task should be diable unsol w/ this configuration
	auto disableUnsol = hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 2, ClassField::AllEventClasses());
	auto challenge = hex::ChallengeResponse(
	                     IINField::Empty(),
	                     2, // seq
	                     0, // csq
	                     0, // unknown user
	                     HMACType::HMAC_SHA1_TRUNC_10,
	                     ChallengeReason::CRITICAL,
	                     hex::repeat(0xFF, 4));

	fixture.TestRequestAndReply(disableUnsol, challenge);

	auto challengeReply = hex::ChallengeReply(seq, 0, user.GetId(), hex::repeat(0xFF, 10));
	auto response = hex::EmptyResponse(seq);

	fixture.TestRequestAndReply(challengeReply, response);

	seq.Increment();

	// next task should be integrity poll
	REQUIRE(fixture.lower.PopWriteAsHex() == hex::IntegrityPoll(seq));
}

TEST_CASE(SUITE("Non-default user can initiate scan prior to the key change"))
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.startupIntegrityClassMask = ClassField();
	params.unsolClassMask = ClassField();

	User joe(42);

	MasterSecAuthFixture fixture(params);

	fixture.ConfigureUser(User::Default());
	fixture.ConfigureUser(joe);

	fixture.context.OnLowerLayerUp();

	fixture.context.ScanRange(GroupVariationID(30, 1), 1, 2, TaskConfig::With(joe));

	AppSeqNum seq;
	fixture.TestSessionKeyExchange(seq, User::Default());
	fixture.TestSessionKeyExchange(seq, joe);

	fixture.application.completions.clear();

	auto request = "C4 01 1E 01 01 01 00 02 00";
	auto reply = "C4 81 00 00";

	fixture.TestRequestAndReply(request, reply);

	REQUIRE(fixture.application.completions.size() == 1);
	auto info = fixture.application.completions.front();
	REQUIRE(info.result == TaskCompletion::SUCCESS);
	REQUIRE(info.type == MasterTaskType::USER_TASK);
	REQUIRE(info.user.GetId() == joe.GetId());

}

TEST_CASE(SUITE("Other tasks are blocked if user has no valid session keys"))
{
	MasterParams params;
	User user = User::Default();
	MasterSecAuthFixture fixture(params);
	fixture.ConfigureUser(user);
	fixture.context.OnLowerLayerUp();

	auto requestKeys = hex::RequestKeyStatus(0, user.GetId());
	auto badResponse = "C0 83 00 00";

	fixture.TestRequestAndReply(requestKeys, badResponse);

	// check that no other tasks run
	REQUIRE(fixture.lower.PopWriteAsHex() == "");
}

TEST_CASE(SUITE("Tasks for non-existant users are immediately failed"))
{
	MasterParams params;
	User user = User::Default();
	MasterSecAuthFixture fixture(params);
	fixture.ConfigureUser(user);

	fixture.context.OnLowerLayerUp();

	CallbackQueue<CommandResponse> queue;
	/// start a command request on some user that doesn't exist
	fixture.context.SelectAndOperate(ControlRelayOutputBlock(ControlCode::LATCH_ON), 1, queue.Callback(), TaskConfig::With(User(42)));

	REQUIRE(queue.responses.size() == 1);
	auto result = queue.responses.front();
	REQUIRE(result.GetResult() == TaskCompletion::FAILURE_NO_USER);
}

void TestTaskCompletionDueToAuthError(AuthErrorCode error, TaskCompletion completion)
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.startupIntegrityClassMask = ClassField();
	params.unsolClassMask = ClassField();

	User user = User::Default();
	MasterSecAuthFixture fixture(params);
	fixture.ConfigureUser(user);

	fixture.context.OnLowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyExchange(seq, user);

	fixture.application.completions.clear();

	/// start a command request on some user that doesn't exist
	fixture.context.ScanAllObjects(GroupVariationID(30, 1), TaskConfig(TaskId::Defined(7), nullptr, user));

	auto request = "C2 01 1E 01 06";
	auto challenge = hex::ChallengeResponse(IINField(), seq, 1, User::DEFAULT_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, hex::repeat(0xFF, 4));
	fixture.TestRequestAndReply(request, challenge);

	auto challengeReply = hex::ChallengeReply(seq, 1, User::DEFAULT_ID, hex::repeat(0xFF, 16));
	auto response = hex::AuthErrorResponse(IINField(), seq, 1, User::DEFAULT_ID, 1, error, DNPTime(0), "");

	fixture.TestRequestAndReply(challengeReply, response);

	REQUIRE(fixture.application.completions.size() == 1);
	auto info = fixture.application.completions.front();
	REQUIRE(info.result == completion);
	REQUIRE(info.user.GetId() == User::DEFAULT_ID);
	REQUIRE(info.type == MasterTaskType::USER_TASK);

	REQUIRE(fixture.exe.NumPendingTimers() == 1); // just the timer for the next task should be active, not the wait timer

	fixture.SendToMaster("C1 83 00 00");
}

TEST_CASE(SUITE("Tasks are failed when an authentication error occurs"))
{
	TestTaskCompletionDueToAuthError(AuthErrorCode::AUTHENTICATION_FAILED, TaskCompletion::FAILURE_BAD_AUTHENTICATION);
	TestTaskCompletionDueToAuthError(AuthErrorCode::INVALID_CERTIFICATION_DATA, TaskCompletion::FAILURE_BAD_AUTHENTICATION);
	TestTaskCompletionDueToAuthError(AuthErrorCode::AUTHORIZATION_FAILED, TaskCompletion::FAILURE_NOT_AUTHORIZED);
}

