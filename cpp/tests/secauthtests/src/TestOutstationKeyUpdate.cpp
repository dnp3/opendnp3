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

#define SUITE(name) "OutstationKeyUpdateTestSuite - " name

TEST_CASE(SUITE("Rejects user status change when authority is not configured"))
{		
	OutstationSecAuthFixture fixture;	
	fixture.LowerLayerUp();

	AppSeqNum seq;
	uint16_t statusChangeSeq = 0;

	auto userStatusChangeRequest = hex::UserStatusChangeRequest(
		seq,
		KeyChangeMethod::AES_256_SHA256_HMAC,
		UserOperation::OP_ADD,
		statusChangeSeq,
		UserRoleToType(UserRole::OPERATOR),
		365,
		"Jim",
		"",
		"DEADBEEF"
		);

	auto response = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, statusChangeSeq, User::UNKNOWN_ID, 0, AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED, DNPTime(0), "");

	REQUIRE(fixture.SendAndReceive(userStatusChangeRequest) == response);
}

TEST_CASE(SUITE("Rejects user status change with incorrect HMAC"))
{
	OutstationSecAuthFixture fixture;	
	fixture.context.ConfigureAuthority(0, AuthorityKey(0xFF));
	fixture.LowerLayerUp();

	AppSeqNum seq;
	uint16_t statusChangeSeq = 0;
	fixture.crypto.sha256.fillByte = 0xBB; // force the HMACS to be different

	auto userStatusChangeRequest = hex::UserStatusChangeRequest(
		seq,
		KeyChangeMethod::AES_256_SHA256_HMAC,
		UserOperation::OP_ADD,
		statusChangeSeq,
		UserRoleToType(UserRole::OPERATOR),
		365,
		"Jim",
		"",
		hex::repeat(0xAA, AuthSizes::MAX_HMAC_OUTPUT_SIZE)
		);

	auto response = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, statusChangeSeq, User::UNKNOWN_ID, 0, AuthErrorCode::INVALID_CERTIFICATION_DATA, DNPTime(0), "");
	
	
	REQUIRE(fixture.SendAndReceive(userStatusChangeRequest) == response);
	REQUIRE(fixture.context.security.stats.GetValue(SecurityStatIndex::AUTHENTICATION_FAILURES) == 1);
	REQUIRE(fixture.context.security.otherStats.badStatusChangeSeqNum == 0);
}

TEST_CASE(SUITE("Rejects authenticated message w/ bad SCSN"))
{
	OutstationSecAuthFixture fixture;
	fixture.context.ConfigureAuthority(1, AuthorityKey(0xFF)); // expecitng SCSN >= 1
	fixture.LowerLayerUp();

	AppSeqNum seq;
	uint16_t statusChangeSeq = 0;
	fixture.crypto.sha256.fillByte = 0xAA;

	auto userStatusChangeRequest = hex::UserStatusChangeRequest(
		seq,
		KeyChangeMethod::AES_256_SHA256_HMAC,
		UserOperation::OP_ADD,
		statusChangeSeq,
		UserRoleToType(UserRole::OPERATOR),
		365,
		"Jim",
		"",
		hex::repeat(0xAA, AuthSizes::MAX_HMAC_OUTPUT_SIZE)
		);

	auto response = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, statusChangeSeq, User::UNKNOWN_ID, 0, AuthErrorCode::INVALID_CERTIFICATION_DATA, DNPTime(0), "");
	
	REQUIRE(fixture.SendAndReceive(userStatusChangeRequest) == response);
	REQUIRE(fixture.context.security.stats.GetValue(SecurityStatIndex::AUTHENTICATION_FAILURES) == 0);
	REQUIRE(fixture.context.security.otherStats.badStatusChangeSeqNum == 1);
}

TEST_CASE(SUITE("Accepts authenticated message w/ good SCSN"))
{
	OutstationSecAuthFixture fixture;
	fixture.context.ConfigureAuthority(2, AuthorityKey(0xFF)); // expecitng SCSN >= 2
	fixture.LowerLayerUp();
	
	AppSeqNum seq;
	fixture.TestAddUserStatusChange("Jim", seq, 3);
}

TEST_CASE(SUITE("Replies with auth error if now prior status change before begin update key change"))
{
	OutstationSecAuthFixture fixture;
	fixture.context.ConfigureAuthority(2, AuthorityKey(0xFF));
	fixture.LowerLayerUp();

	AppSeqNum seq;
	auto request = hex::BeginUpdateKeyChangeRequest(seq, KeyChangeMethod::AES_256_SHA256_HMAC, "Jim", hex::repeat(0xFF, 4));
	auto response = hex::AuthErrorResponse(IINBit::DEVICE_RESTART, seq, 0, User::UNKNOWN_ID, 0, AuthErrorCode::UNKNOWN_USER, DNPTime(0), "");
	
	REQUIRE(fixture.SendAndReceive(request) == response);
}

TEST_CASE(SUITE("Replies with g120v12 if the user exists and assigns a user #"))
{
	OutstationSecAuthFixture fixture;
	fixture.context.ConfigureAuthority(2, AuthorityKey(0xFF));
	fixture.LowerLayerUp();

	auto BOB = "Bob";

	AppSeqNum seq;
	fixture.TestAddUserStatusChange(BOB, seq, 4);

	auto request = hex::BeginUpdateKeyChangeRequest(seq, KeyChangeMethod::AES_256_SHA256_HMAC, BOB, hex::repeat(0xFF, 4));
	auto response = hex::BeginUpdateKeyChangeResponse(seq, 0, 2, hex::repeat(0xAA, 4));

	REQUIRE(fixture.SendAndReceive(request) == response);
}





