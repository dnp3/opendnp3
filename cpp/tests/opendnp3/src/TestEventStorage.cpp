/*
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

#include "opendnp3/outstation/EventStorage.h"

#include "mocks/MockEventWriteHandler.h"

using namespace opendnp3;

#define SUITE(name) "EventStorageTestSuite - " name

TEST_CASE(SUITE("can construct"))
{
	EventStorage storage(
	    EventBufferConfig::AllTypes(10)
	);
}

TEST_CASE(SUITE("can load and write Binary"))
{
	EventStorage storage(EventBufferConfig::AllTypes(10));
	REQUIRE_FALSE(
	    storage.Update(
	        Event<BinarySpec>(Binary(true), 0, EventClass::EC1, EventBinaryVariation::Group2Var1)
	    )
	);

	// select events by class
	REQUIRE(storage.Select(EventClass::EC1) == 1);

	// set up the expected order
	MockEventWriteHandler handler;
	handler.Expect(EventBinaryVariation::Group2Var1, 1);

	REQUIRE(storage.Write(handler) == 1);

	handler.AssertEmpty();
}




