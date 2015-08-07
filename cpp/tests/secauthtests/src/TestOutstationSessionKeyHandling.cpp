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

#include "fixtures/OutstationSecAuthFixture.h"

#include <dnp3mocks/APDUHexBuilders.h>

#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace secauth;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "OutstationSessionKeyTestSuite - " name

TEST_CASE(SUITE("ChangeSessionKeys-AES128-SHA256-16"))
{		
	OutstationSecAuthFixture fixture;
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_128);
	fixture.LowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_128, HMACMode::SHA256_TRUNC_16);
}

TEST_CASE(SUITE("ChangeSessionKeys-AES256-SHA256-16"))
{	
	OutstationSecAuthFixture fixture;
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256);
	fixture.LowerLayerUp();
	
	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);
}

TEST_CASE(SUITE("ChangeSessionKeys-AES256-SHA1-8"))
{	
	OutstationAuthSettings settings;
	settings.hmacMode = HMACMode::SHA1_TRUNC_8; // use a non-default HMAC mode

	OutstationSecAuthFixture fixture(settings);	
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256);
	fixture.LowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA1_TRUNC_8);
}

TEST_CASE(SUITE("Critical requests are challenged when session keys are not initialized"))
{	
	OutstationAuthSettings settings;
	settings.challengeSize = 5; // try a non-default challenge size

	OutstationSecAuthFixture fixture(settings);
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256);
	fixture.LowerLayerUp();

	auto request = hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses());

	auto challenge = hex::ChallengeResponse(
		IINBit::DEVICE_RESTART,
		0, // app-seq
		1, // csq
		User::UNKNOWN_ID,
		HMACType::HMAC_SHA256_TRUNC_16,
		ChallengeReason::CRITICAL,
		"AA AA AA AA AA"
	);

	REQUIRE(fixture.SendAndReceive(request) == challenge);

	REQUIRE(fixture.lower.HasNoData());
}

TEST_CASE(SUITE("Sessions keys ared invalidated after configured period"))
{	
	OutstationAuthSettings settings;
	settings.sessionKeyTimeout = TimeDuration::Minutes(5); // set to some known value	
	OutstationSecAuthFixture fixture(settings);	
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_128);
	fixture.LowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_128, HMACMode::SHA256_TRUNC_16);
	
	auto readRequest = hex::ClassTask(FunctionCode::READ, seq, ClassField::AllEventClasses());
	auto challenge = hex::ChallengeResponse(IINBit::DEVICE_RESTART, seq, 1, User::UNKNOWN_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, "AA AA AA AA");
	REQUIRE(fixture.SendAndReceive(readRequest) == challenge);
		
	// Advance the time source past the key timeout period
	fixture.exe.AddTime(openpal::TimeDuration::Minutes(6));

	auto challengeReply = hex::ChallengeReply(seq, 1, User::DEFAULT_ID, hex::repeat(0xFF, 16));
	auto errorResponse = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, 1, User::DEFAULT_ID, 0, AuthErrorCode::AUTHENTICATION_FAILED, DNPTime(0), "");
	REQUIRE(fixture.SendAndReceive(challengeReply) == errorResponse);

	REQUIRE(fixture.lower.HasNoData());
}

TEST_CASE(SUITE("Sessions keys are invalidated after configured period"))
{	
	OutstationAuthSettings settings;
	settings.sessionKeyTimeout = TimeDuration::Minutes(5); // set to some known value	
	OutstationSecAuthFixture fixture(settings);	
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_128);
	fixture.LowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_128, HMACMode::SHA256_TRUNC_16);
	
	auto poll = hex::ClassTask(FunctionCode::READ, seq, ClassField::AllEventClasses());
	auto challenge = hex::ChallengeResponse(IINBit::DEVICE_RESTART, seq, 1, User::UNKNOWN_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, "AA AA AA AA");
	REQUIRE(fixture.SendAndReceive(poll) == challenge);

	// Advance the time
	fixture.exe.AddTime(openpal::TimeDuration::Minutes(6));

	auto challengeReply = hex::ChallengeReply(seq, 1, User::DEFAULT_ID, hex::repeat(0xFF, 16));
	auto errorResp = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, 1, User::DEFAULT_ID, 0, AuthErrorCode::AUTHENTICATION_FAILED, DNPTime(0), "");
	REQUIRE(fixture.SendAndReceive(challengeReply) == errorResp);
	REQUIRE(fixture.lower.HasNoData());

	REQUIRE(fixture.context.security.otherStats.authFailuresDueToExpiredKeys == 1);
}

TEST_CASE(SUITE("Sessions keys are invalidated after configured number of authenticated messages for a user"))
{	
	OutstationAuthSettings settings;
	settings.maxAuthMsgCount = 1; // only allow a single authenticated message before invalidating keys
	OutstationSecAuthFixture fixture(settings);	
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_128);
	fixture.LowerLayerUp();

	AppSeqNum seq;
	uint32_t csq = 1;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_128, HMACMode::SHA256_TRUNC_16);

	{
		auto poll = hex::ClassTask(FunctionCode::READ, seq, ClassField::AllEventClasses());
		auto challenge = hex::ChallengeResponse(IINBit::DEVICE_RESTART, seq, csq, User::UNKNOWN_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, "AA AA AA AA");
		REQUIRE(fixture.SendAndReceive(poll) == challenge);

		auto challengeReply = hex::ChallengeReply(seq, csq, User::DEFAULT_ID, hex::repeat(0xFF, 16));
		auto response = hex::EmptyResponse(seq, IINBit::DEVICE_RESTART);
		REQUIRE(fixture.SendAndReceive(challengeReply) == response);
	}

	seq.Increment();
	++csq;

	{
		auto poll = hex::ClassTask(FunctionCode::READ, seq, ClassField::AllEventClasses());
		auto challenge = hex::ChallengeResponse(IINBit::DEVICE_RESTART, seq, csq, User::UNKNOWN_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, "AA AA AA AA");
		REQUIRE(fixture.SendAndReceive(poll) == challenge);

		auto challengeReply = hex::ChallengeReply(seq, csq, User::DEFAULT_ID, hex::repeat(0xFF, 16));
		auto error = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, csq, User::DEFAULT_ID, 0, AuthErrorCode::AUTHENTICATION_FAILED, DNPTime(0), "");
		REQUIRE(fixture.SendAndReceive(challengeReply) == error);
	}

	REQUIRE(fixture.lower.HasNoData());
	REQUIRE(fixture.context.security.otherStats.authFailuresDueToExpiredKeys == 1);		
}

TEST_CASE(SUITE("Non-critical requests are not challenged"))
{	
	OutstationAuthSettings settings;
	settings.functions.authRead = false;

	OutstationSecAuthFixture fixture(settings);
	fixture.LowerLayerUp();

	fixture.SendToOutstation(hex::EventPoll(0));
	REQUIRE(fixture.lower.PopWriteAsHex() == hex::EmptyResponse(0, IINField(IINBit::DEVICE_RESTART)));
}

TEST_CASE(SUITE("Critical requests can be challenged and processed"))
{		
	OutstationAuthSettings settings;	
	OutstationSecAuthFixture fixture(settings);	
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256);
	fixture.LowerLayerUp();

	AppSeqNum seq;

	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);

	auto poll = hex::ClassTask(FunctionCode::READ, seq, ClassField::AllEventClasses());
	auto challenge = hex::ChallengeResponse(
		IINBit::DEVICE_RESTART,
		seq,
		1, // csq
		User::UNKNOWN_ID,
		HMACType::HMAC_SHA256_TRUNC_16,
		ChallengeReason::CRITICAL,
		hex::repeat(0xAA, 4));

	REQUIRE(fixture.SendAndReceive(poll) == challenge);
	

	auto challengeReply = hex::ChallengeReply(seq, 1, User::DEFAULT_ID, hex::repeat(0xFF, 16));
	auto response = hex::EmptyResponse(seq, IINBit::DEVICE_RESTART);
	REQUIRE(fixture.SendAndReceive(challengeReply) == response);	


	REQUIRE(fixture.lower.HasNoData());
}

TEST_CASE(SUITE("Outstation enforces permissions for critical functions"))
{			
	OutstationSecAuthFixture fixture;
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256, Permissions::AllowNothing());
	fixture.LowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);
		
	auto poll = hex::ClassTask(FunctionCode::READ, seq, ClassField::AllEventClasses());
	auto challenge = hex::ChallengeResponse(
		IINBit::DEVICE_RESTART,
		seq,
		1, // csq
		User::UNKNOWN_ID,
		HMACType::HMAC_SHA256_TRUNC_16,
		ChallengeReason::CRITICAL,
		hex::repeat(0xAA, 4)
		);

	REQUIRE(fixture.SendAndReceive(poll) == challenge);

	auto challengeReply = hex::ChallengeReply(seq, 1, User::DEFAULT_ID, hex::repeat(0xFF, 16));
	auto error = hex::AuthErrorResponse(
		IINBit::DEVICE_RESTART,
		seq,
		1,
		User::DEFAULT_ID,
		0,
		AuthErrorCode::AUTHORIZATION_FAILED,
		DNPTime(0),
		""
	);

	REQUIRE(fixture.SendAndReceive(challengeReply) == error);
	REQUIRE(fixture.lower.HasNoData());
}

TEST_CASE(SUITE("Chain of unauthorized requests is handled"))
{
	OutstationSecAuthFixture fixture;	
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256, Permissions::AllowNothing());
	fixture.LowerLayerUp();

	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);

	uint32_t csq = 1;
	const int ITERATIONS = 10;

	for (int i = 0; i < ITERATIONS; ++i)
	{		
		auto crob = hex::Control(FunctionCode::DIRECT_OPERATE, seq, ControlRelayOutputBlock(ControlCode::LATCH_ON), 0);		
		auto challenge = hex::ChallengeResponse(IINBit::DEVICE_RESTART, seq, csq, User::UNKNOWN_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, hex::repeat(0xAA, 4));		
		REQUIRE(fixture.SendAndReceive(crob) == challenge);

		auto challengeReply = hex::ChallengeReply(seq, csq, User::DEFAULT_ID, hex::repeat(0xFF, 16));
		auto error = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, csq, User::DEFAULT_ID, 0, AuthErrorCode::AUTHORIZATION_FAILED, DNPTime(0), "");
		REQUIRE(fixture.SendAndReceive(challengeReply) == error);

		REQUIRE(fixture.lower.HasNoData());

		++csq;
	}		
}

void TestUnauthorizedControl(OutstationSecAuthFixture& fixture, AppSeqNum& seq, uint32_t csq)
{
	
}

TEST_CASE(SUITE("Mixture of authenticated requests and unsolicited behaves as expected"))
{
	secauth::OutstationAuthSettings authConfig;
	authConfig.functions.authConfirm = false;
	auto db = DatabaseTemplate::BinaryOnly(1);
	OutstationConfig config;
	config.params.allowUnsolicited = true;	
	
		
	OutstationSecAuthFixture fixture(authConfig, db, config);
	//fixture.log.WriteToStdIo();
	fixture.AddUser(User::Default(), "bob", 0xFF, KeyWrapAlgorithm::AES_256, Permissions::AllowNothing());
	fixture.LowerLayerUp();	

	REQUIRE(fixture.lower.PopWriteAsHex() == hex::NullUnsolicited(0, IINBit::DEVICE_RESTART));
	fixture.OnSendResult(true);
	
	AppSeqNum seq;
	fixture.TestSessionKeyChange(seq, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);

	// confirm the null unsolicited
	fixture.SendToOutstation(hex::UnsolConfirm(0));

	uint32_t csq = 1;	
	
	for (int i = 0; i < 10; ++i)
	{		
		//std::cout << i << std::endl;

		auto crob = hex::Control(FunctionCode::DIRECT_OPERATE, seq, ControlRelayOutputBlock(ControlCode::LATCH_ON), 0);
		auto challenge = hex::ChallengeResponse(IINBit::DEVICE_RESTART, seq, csq, User::UNKNOWN_ID, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, hex::repeat(0xAA, 4));
		REQUIRE(fixture.SendAndReceive(crob) == challenge);

		auto challengeReply = hex::ChallengeReply(seq, csq, User::DEFAULT_ID, hex::repeat(0xFF, 16));
		auto error = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, csq, User::DEFAULT_ID, 0, AuthErrorCode::AUTHORIZATION_FAILED, DNPTime(0), "");
		REQUIRE(fixture.SendAndReceive(challengeReply) == error);

		REQUIRE(fixture.lower.HasNoData());

		++csq;
	}
}


