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

#include <opendnp3/objects/Group120Var1.h>

#include <openpal/util/ToHex.h>
#include <openpal/container/DynamicBuffer.h>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "Group120Var1TestSuite - " name

TEST_CASE(SUITE("Parser rejects empty buffer"))
{
	HexSequence buffer("");

	Group120Var1 output;
	bool success = Group120Var1::Read(buffer.ToReadOnly(), output);
	REQUIRE(!success);
}

TEST_CASE(SUITE("Parser accepts minimum of four bytes of challenge data"))
{
	// SEQ = 1, USER = 7, HMAC = 5 (SHA-1-8), REASON = 1, challenge = 0xDEADBEEF
	HexSequence buffer("01 00 00 00 07 00 05 01 DE AD BE EF");

	Group120Var1 output;	
	REQUIRE(Group120Var1::Read(buffer.ToReadOnly(), output));
	REQUIRE(output.challengeSeqNum == 1);
	REQUIRE(output.userNum == 7);
	REQUIRE(output.hmacType == HMACType::HMAC_SHA1_TRUNC_8);
	REQUIRE(output.reason == ChallengeReason::CRITICAL);
	REQUIRE(toHex(output.challengeData) == "DE AD BE EF");
}

TEST_CASE(SUITE("Parser accepts empty challenge data"))
{
	// SEQ = 1, USER = 7, HMAC = 5 (SHA-1-8), REASON = 1, challenge length of 3
	HexSequence buffer("01 00 00 00 07 00 05 01");

	Group120Var1 output;
	REQUIRE(Group120Var1::Read(buffer.ToReadOnly(), output));
	REQUIRE(output.challengeData.Size() == 0);
}

TEST_CASE(SUITE("Parser rejects one less than minimum required data"))
{
	// SEQ = 1, USER = 7, HMAC = 5 (SHA-1-8), REASON = ???? missing
	HexSequence buffer("01 00 00 00 07 00 05");

	Group120Var1 output;
	REQUIRE(!Group120Var1::Read(buffer.ToReadOnly(), output));	
}

TEST_CASE(SUITE("Formatter returns false when insufficient space"))
{
	std::string challengeData = "DE AD BE EF AB BA"; // 6 bytes

	HexSequence buffer(challengeData);
	DynamicBuffer output(64);
	
	Group120Var1 challenge(9, 3, HMACType::HMAC_SHA256_TRUNC_16, ChallengeReason::CRITICAL, buffer.ToReadOnly());
	const uint32_t OBJECT_SIZE = 14;
	REQUIRE(challenge.Size() == OBJECT_SIZE);

	auto dest = output.GetWriteBufferView();
	REQUIRE(Group120Var1::Write(challenge, dest));
	REQUIRE(dest.Size() == (output.Size() - OBJECT_SIZE));

	REQUIRE(toHex(output.ToReadOnly().Take(OBJECT_SIZE)) == "09 00 00 00 03 00 04 01 DE AD BE EF AB BA");
}