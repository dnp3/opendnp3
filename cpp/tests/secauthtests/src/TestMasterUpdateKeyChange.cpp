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

#include <dnp3mocks/MockTaskCallback.h>
#include <dnp3mocks/CallbackQueue.h>
#include <dnp3mocks/APDUHexBuilders.h>


#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace secauth;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "MasterUpdateKeyChangeChangeSuite - " name

TEST_CASE(SUITE("well-formed response results in successful callback"))
{		
	MasterParams params;
	MasterSecAuthFixture fixture(params);
	fixture.context.OnLowerLayerUp();	

	CallbackQueue<BeginUpdateKeyChangeResult> queue;
	MockTaskCallback tcallback;

	fixture.context.BeginUpdateKeyChange("jim", TaskConfig::With(tcallback), queue.Callback());
		
	auto request = "C0 20 78 0B 5B 01 0C 00 04 03 00 04 00 6A 69 6D AA AA AA AA";
	auto reply = "C0 83 00 00 78 0C 5B 01 0C 00 01 00 00 00 07 00 04 00 DE AD BE EF";

	fixture.TestRequestAndReply(request, reply);

	REQUIRE(queue.responses.size() == 1);
	auto& data = queue.responses.front();

	REQUIRE(data.result == TaskCompletion::SUCCESS);

	// test all the values
	REQUIRE(data.keyChangeSequenceNum == 1);
	REQUIRE(data.user.GetId() == 7);
	REQUIRE(ToHex(data.masterChallengeData.ToReadOnly()) == "AA AA AA AA");
	REQUIRE(ToHex(data.outstationChallengeData.ToReadOnly()) == "DE AD BE EF");	

	REQUIRE(tcallback.results.size() == 1);
	REQUIRE(tcallback.results.front() == TaskCompletion::SUCCESS);
}



