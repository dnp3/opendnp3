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

#include "BufferHelpers.h"
#include "HexConversions.h"

#include <opendnp3/objects/Group120.h>

#include <openpal/util/ToHex.h>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "SecAuthParsingTestSuite - " name

TEST_CASE(SUITE("EmptyBufferReturnsFalse"))
{
	HexSequence buffer("");

	Group120Var1 output;
	bool success = Group120Var1::Read(buffer.ToReadOnly(), output, nullptr);
	REQUIRE(!success);
}

TEST_CASE(SUITE("Accepts minimum of four bytes of challenge data"))
{
	// SEQ = 1, USER = 7, HMAC = 5 (SHA-1-8), REASON = 1, challenge = 0xDEADBEEF
	HexSequence buffer("01 00 00 00 07 00 05 01 DE AD BE EF");

	Group120Var1 output;	
	REQUIRE(Group120Var1::Read(buffer.ToReadOnly(), output, nullptr));
	REQUIRE(output.challengeSeqNum == 1);
	REQUIRE(output.userNum == 7);
	REQUIRE(output.hmacType == HMACType::HMAC_SHA1_TRUNC_8);
	REQUIRE(output.reason == ChallengeReason::CRITICAL);
	REQUIRE(toHex(output.challengeData) == "DE AD BE EF");
}

TEST_CASE(SUITE("Rejects three bytes of challenge data"))
{
	// SEQ = 1, USER = 7, HMAC = 5 (SHA-1-8), REASON = 1, challenge length of 3
	HexSequence buffer("01 00 00 00 07 00 05 01 DE AD BE");

	Group120Var1 output;
	REQUIRE(!Group120Var1::Read(buffer.ToReadOnly(), output, nullptr));
}


