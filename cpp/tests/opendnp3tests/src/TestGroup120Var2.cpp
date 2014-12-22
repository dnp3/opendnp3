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

#include <opendnp3/objects/Group120Var2.h>

#include <openpal/util/ToHex.h>
#include <openpal/container/DynamicBuffer.h>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "Group120Var2TestSuite - " name

TEST_CASE(SUITE("Parser rejects empty buffer"))
{
	HexSequence buffer("");

	Group120Var2 output;
	bool success = Group120Var2::Read(buffer.ToReadOnly(), output);	
	REQUIRE(!success);
}

TEST_CASE(SUITE("Parser identifies data field"))
{
	HexSequence buffer("04 00 00 00 09 01 AB BA");

	Group120Var2 output;	
	REQUIRE(Group120Var2::Read(buffer.ToReadOnly(), output));
	REQUIRE(output.seq == 4);
	REQUIRE(output.user == 265);
	REQUIRE(toHex(output.data) == "AB BA");
}

TEST_CASE(SUITE("Parser allows empty data field"))
{
	HexSequence buffer("04 00 00 00 09 01");

	Group120Var2 output;
	REQUIRE(Group120Var2::Read(buffer.ToReadOnly(), output));
	REQUIRE(output.seq == 4);
	REQUIRE(output.user == 265);
	REQUIRE(output.data.IsEmpty());
}

TEST_CASE(SUITE("Parser rejects one less than min length"))
{
	HexSequence buffer("04 00 00 00 09");
	Group120Var2 output;
	REQUIRE(!Group120Var2::Read(buffer.ToReadOnly(), output));	
}