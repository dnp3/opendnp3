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

#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace secauth;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "MasterSecAuthSuite - " name

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
		hex::repeat(0xFF,4));

	fixture.TestRequestAndReply(disableUnsol, challenge);

	auto challengeReply = hex::ChallengeReply(seq, 0, user.GetId(), hex::repeat(0xFF, 10));
	auto response = hex::EmptyResponse(seq);

	fixture.TestRequestAndReply(challengeReply, response);

	seq.Increment();

	// next task should be integrity poll
	REQUIRE(fixture.lower.PopWriteAsHex() == hex::IntegrityPoll(seq));
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


