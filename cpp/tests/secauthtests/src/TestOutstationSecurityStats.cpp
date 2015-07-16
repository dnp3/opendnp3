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

#define SUITE(name) "OutstationSecurityStatsTestSuite - " name

TEST_CASE(SUITE("ReadStaticSecurityStats"))
{
	OutstationAuthSettings settings;
	settings.functions.authRead = false;

	OutstationSecAuthFixture fixture(settings);
	fixture.LowerLayerUp();

	// The totalTx (index 5) and totalRx (index 6) are easy to test
	// The rx event gets processed when the request arrives, but the tx event isn't processed until after the repsonse is formatted

	fixture.SendToOutstation("C0 01 79 00 00 05 06");


	REQUIRE(fixture.lower.PopWriteAsHex() == "C0 81 80 00 79 01 00 05 06 01 00 00 00 00 00 00 01 00 00 01 00 00 00");
	fixture.OnSendResult(true);

	fixture.SendToOutstation("C1 01 79 00 00 05 06");

	// both counts are incremented by 1
	REQUIRE(fixture.lower.PopWriteAsHex() == "C1 81 80 00 79 01 00 05 06 01 00 00 01 00 00 00 01 00 00 02 00 00 00");
}

TEST_CASE(SUITE("ReadSecurityStatEvents"))
{
	OutstationAuthSettings settings;	
	settings.functions.authRead = false;
	settings.functions.authConfirm = false;
	
	/// override the default thresholds to make testing easier
	settings.statThresholds.Set(SecurityStatIndex::TOTAL_MESSAGES_TX, 2);
	settings.statThresholds.Set(SecurityStatIndex::TOTAL_MESSAGES_RX, 2);
	

	OutstationConfig config;
	config.eventBufferConfig.maxSecurityStatisticEvents = 100;
	
	OutstationSecAuthFixture fixture(settings, DatabaseTemplate(), config);
	fixture.LowerLayerUp();
		
	AppSeqNum seq;

	// should get no statistics reports on the first poll		
	fixture.SendToOutstation(hex::EventPoll(seq.Get(), ClassField::AllEventClasses()));
	REQUIRE(fixture.lower.PopWriteAsHex() == hex::EmptyResponse(seq.Get(), IINBit::DEVICE_RESTART));
	fixture.OnSendResult(true);		
	seq.Increment();
	
	fixture.SendToOutstation(hex::EventPoll(seq.Get(), ClassField::AllEventClasses()));
	REQUIRE(fixture.lower.PopWriteAsHex() == "E1 81 80 00 7A 01 28 01 00 06 00 01 00 00 02 00 00 00"); // total rx == 2
	fixture.OnSendResult(true);
	
	// confirm the event
	fixture.SendToOutstation(hex::Confirm(seq.Get(), false));
	seq.Increment();

	fixture.SendToOutstation(hex::EventPoll(seq.Get(), ClassField::AllEventClasses()));
	// total tx == 2, total rx == 4
	REQUIRE(fixture.lower.PopWriteAsHex() == "E2 81 80 00 7A 01 28 02 00 05 00 01 00 00 02 00 00 00 06 00 01 00 00 04 00 00 00"); 
}