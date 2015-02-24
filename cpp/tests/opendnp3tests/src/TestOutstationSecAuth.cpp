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

using namespace std;
using namespace opendnp3;
using namespace secauthv5;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "OutstationSecAuthTestSuite - " name

TEST_CASE(SUITE("CanChangeSessionKeys"))
{	
	OutstationSecAuthTest test;
	test.log.WriteToStdIo();
	test.AddUser(User::Default(), UpdateKeyMode::AES128, 0xFF);
	test.LowerLayerUp();

	REQUIRE(test.lower.HasNoData());

	// -- request key status, user = 1 ---
	test.SendToOutstation("C0 20 78 04 07 01 01 00");

	//
	// --- key status response ---
	//
	// KSQ = 0x01000000
	// user = 0x0100
	// key wrap = 0x01 (aes128)
	// key status = 0x02 (not init)
	// MAC = 0x00 (no mac)
	// challenge length = 0x0400
	// challenge data = 0xAAAAAAAA
	//
	auto status = "C0 83 00 00 78 05 5B 01 0F 00 01 00 00 00 01 00 01 02 00 04 00 AA AA AA AA";
	REQUIRE(test.lower.PopWriteAsHex() == status);
	test.outstation.OnSendResult(true);
	REQUIRE(test.lower.HasNoData());

	// -- session key change request --
	test.SendToOutstation("C0 20 78 06 5B 01 CC CC 01 00 00 00 01 00 FF FF FF FF");

	auto result = "FF";
	REQUIRE(test.lower.PopWriteAsHex() == result);	
}
