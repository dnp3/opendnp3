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

TEST_CASE(SUITE("ChangeSessionKeys-AES128-SHA256-16"))
{		
	MasterParams params;
	User user(5);
	MasterSecAuthFixture fixture(params, user);

	auto MOCK_KEY_WRAP_DATA = "DEADBEEF";	
	auto MOCK_HMAC_VALUE = "FFFFFFFFFFFFFFFFFFFF";
	fixture.crypto.aes128.hexOutput = MOCK_KEY_WRAP_DATA; // set mock key wrap data

	fixture.master.OnLowerLayerUp();

	REQUIRE(fixture.exe.RunMany() > 0);	
	REQUIRE(fixture.lower.PopWriteAsHex() == hex::RequestKeyStatus(0, user.GetId()));
	fixture.master.OnSendResult(true);

	fixture.SendToMaster(hex::KeyStatusResponse(
		IINField::Empty(),
		0, // seq
		0, // ksq
		user.GetId(),
		KeyWrapAlgorithm::AES_128,
		KeyStatus::NOT_INIT,
		HMACType::HMAC_SHA1_TRUNC_10,
		"FF FF FF FF", // challenge
		"" // no hmac
	));

	REQUIRE(fixture.lower.PopWriteAsHex() == hex::KeyChangeRequest(1, 0, user.GetId(), MOCK_KEY_WRAP_DATA));
	fixture.master.OnSendResult(true);

	fixture.SendToMaster(hex::KeyStatusResponse(
		IINField::Empty(),
		1, // seq
		0, // ksq
		user.GetId(),
		KeyWrapAlgorithm::AES_128,
		KeyStatus::OK,
		HMACType::HMAC_SHA1_TRUNC_10,
		"FF FF FF FF",	// challenge
		MOCK_HMAC_VALUE
	));
	REQUIRE(fixture.exe.RunMany() > 0);

	REQUIRE(fixture.lower.PopWriteAsHex() == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 2, ClassField::AllEventClasses()));
}

