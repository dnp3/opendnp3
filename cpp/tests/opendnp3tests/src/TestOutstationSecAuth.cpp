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
#include "APDUHexBuilders.h"

#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace secauthv5;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "OutstationSecAuthTestSuite - " name

TEST_CASE(SUITE("CanChangeSessionKeys"))
{	
	OutstationSecAuthTest test;	
	test.AddUser(User::Default(), UpdateKeyMode::AES128, 0xFF);
	test.LowerLayerUp();

	REQUIRE(test.lower.HasNoData());
	
	test.SendToOutstation(hex::RequestKeyStatus(0, 1));
	
	auto keyStatusRsp = hex::KeyStatusResponse(
		0, // seq
		1, // ksq
		1, // user
		KeyWrapAlgorithm::AES_128,
		KeyStatus::NOT_INIT,
		HMACType::NO_MAC_VALUE,
		"AA AA AA AA", // challenge
		"");  // no hmac
				

	REQUIRE(test.lower.PopWriteAsHex() == keyStatusRsp);
	test.outstation.OnSendResult(true);
	REQUIRE(test.lower.HasNoData());
	
	// mock the key wrap output data, make the keys all 0xBB
	test.crypto.aes128.hexOutput = hex::KeyWrapData(
		16, // 128-bit keys
		0xBB,
		SkipBytesHex(keyStatusRsp, 10)  // skip to the actual object response 10 bytes in
	);

	// --- session key change request ---	
	// the key wrap data doesn't matter b/c we've mocked the unwrap call above
	test.SendToOutstation(hex::KeyChangeRequest(0, 1, 1, "DE AD BE EF"));

	auto keyStatusRspFinal = hex::KeyStatusResponse(
		0, // seq
		2, // ksq
		1, // user
		KeyWrapAlgorithm::AES_128,
		KeyStatus::OK,
		HMACType::HMAC_SHA256_TRUNC_16,
		"AA AA AA AA", // challenge
		RepeatHex(0xFF, 16));  // fixed value from hmac mock
	
	REQUIRE(test.lower.PopWriteAsHex() == keyStatusRspFinal);
}
