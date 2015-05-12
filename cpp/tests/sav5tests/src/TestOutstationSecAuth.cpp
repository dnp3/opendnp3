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

#include "OutstationSecAuthTest.h"

#include <dnp3mocks/APDUHexBuilders.h>

#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace secauthv5;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "OutstationSecAuthTestSuite - " name\

void TestSessionKeyChange(OutstationSecAuthTest& test, User user, KeyWrapAlgorithm keyWrap, HMACMode hmacMode);
void SetMockKeyWrapData(MockCryptoProvider& crypto, KeyWrapAlgorithm keyWrap, const std::string& lastStatusRsp);

TEST_CASE(SUITE("ChangeSessionKeys-AES128-SHA256-16"))
{	
	OutstationSecAuthTest test;	
	test.AddUser(User::Default(), UpdateKeyMode::AES128, 0xFF);
	test.LowerLayerUp();
	TestSessionKeyChange(test, User::Default(), KeyWrapAlgorithm::AES_128,HMACMode::SHA256_TRUNC_16);
}

TEST_CASE(SUITE("ChangeSessionKeys-AES256-SHA256-16"))
{
	OutstationSecAuthTest test;	
	test.AddUser(User::Default(), UpdateKeyMode::AES256, 0xFF);
	test.LowerLayerUp();
	TestSessionKeyChange(test, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);
}

TEST_CASE(SUITE("ChangeSessionKeys-AES256-SHA1-8"))
{
	OutstationAuthSettings settings;
	settings.hmacMode = HMACMode::SHA1_TRUNC_8; // use a non-default HMAC mode

	OutstationSecAuthTest test(settings);
	test.AddUser(User::Default(), UpdateKeyMode::AES256, 0xFF);
	test.LowerLayerUp();
	TestSessionKeyChange(test, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA1_TRUNC_8);
}

TEST_CASE(SUITE("Critical requests are challenged when session keys are not initialized"))
{
	OutstationAuthSettings settings;
	settings.challengeSize = 5; // try a non-default challenge size

	OutstationSecAuthTest test(settings);
	
	test.LowerLayerUp();

	test.SendToOutstation(hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses()));

	auto challenge = hex::ChallengeResponse(
		IINBit::DEVICE_RESTART,
		0, // app-seq
		1, // csq
		User::DEFAULT_ID,
		HMACType::HMAC_SHA256_TRUNC_16,
		ChallengeReason::CRITICAL,
		"AA AA AA AA AA"
	);

	REQUIRE(test.lower.PopWriteAsHex() == challenge);
}

TEST_CASE(SUITE("Non-critical requests are not challenged"))
{
	OutstationAuthSettings settings;
	settings.functions.authRead = false;

	OutstationSecAuthTest test(settings);
	test.LowerLayerUp();

	test.SendToOutstation(hex::EventPoll(0));
	REQUIRE(test.lower.PopWriteAsHex() == hex::EmptyResponse(0, IINField(IINBit::DEVICE_RESTART)));
}

TEST_CASE(SUITE("Critical requests can be challenged and processed"))
{
	OutstationAuthSettings settings;	
	OutstationSecAuthTest test(settings);

	test.AddUser(User::Default(), UpdateKeyMode::AES256, 0xFF);
	
	test.LowerLayerUp();

	TestSessionKeyChange(test, User::Default(), KeyWrapAlgorithm::AES_256, HMACMode::SHA256_TRUNC_16);

	test.SendToOutstation(hex::ClassTask(FunctionCode::READ, 1, ClassField::AllEventClasses()));

	auto challenge = hex::ChallengeResponse(
		IINBit::DEVICE_RESTART,
		1, // app-seq
		1, // csq
		User::DEFAULT_ID,
		HMACType::HMAC_SHA256_TRUNC_16,
		ChallengeReason::CRITICAL,
		hex::repeat(0xAA, 4)
	);

	REQUIRE(test.lower.PopWriteAsHex() == challenge);
	test.outstation.OnSendResult(true);
	
	test.SendToOutstation(hex::ChallengeReply(1, 1, User::DEFAULT_ID, hex::repeat(0xFF, 16)));

	REQUIRE(test.lower.PopWriteAsHex() == hex::EmptyResponse(1, IINField(IINBit::DEVICE_RESTART)));
}

void TestSessionKeyChange(OutstationSecAuthTest& test, User user, KeyWrapAlgorithm keyWrap, HMACMode hmacMode)
{
	REQUIRE(test.lower.HasNoData());

	test.SendToOutstation(hex::RequestKeyStatus(0, 1));

	auto keyStatusRsp = hex::KeyStatusResponse(
		IINBit::DEVICE_RESTART,
		0, // seq
		1, // ksq
		user.GetId(),
		keyWrap,
		KeyStatus::NOT_INIT,
		HMACType::NO_MAC_VALUE,
		hex::repeat(0xAA, 4), // challenge
		"");  // no hmac


	REQUIRE(test.lower.PopWriteAsHex() == keyStatusRsp);
	test.outstation.OnSendResult(true);
	REQUIRE(test.lower.HasNoData());	

	SetMockKeyWrapData(test.crypto, keyWrap, SkipBytesHex(keyStatusRsp, 10));

	// --- session key change request ---	
	// the key wrap data doesn't matter b/c we've mocked the unwrap call above
	test.SendToOutstation(hex::KeyChangeRequest(0, 1, 1, "DE AD BE EF"));

	auto keyStatusRspFinal = hex::KeyStatusResponse(
		IINBit::DEVICE_RESTART,
		0, // seq
		2, // ksq
		user.GetId(), // user
		keyWrap,
		KeyStatus::OK,
		ToHMACType(hmacMode),
		hex::repeat(0xAA, 4), // challenge
		RepeatHex(0xFF, GetTruncationSize(hmacMode)));  // fixed value from hmac mock

	REQUIRE(test.lower.PopWriteAsHex() == keyStatusRspFinal);
	test.outstation.OnSendResult(true);
	REQUIRE(test.lower.HasNoData());
}

void SetMockKeyWrapData(MockCryptoProvider& crypto, KeyWrapAlgorithm keyWrap, const std::string& statusData)
{
	switch (keyWrap)
	{
	case(KeyWrapAlgorithm::AES_128) :
		crypto.aes128.hexOutput = hex::KeyWrapData(16, 0xBB, statusData);
		break;
	case(KeyWrapAlgorithm::AES_256) :
		crypto.aes256.hexOutput = hex::KeyWrapData(32, 0xBB, statusData);
		break;
	default:
		throw std::logic_error("bad param");
	}
}
