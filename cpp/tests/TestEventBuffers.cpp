
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"

#include <opendnp3/EventBuffers.h>
#include <opendnp3/EventTypes.h>
#include <opendnp3/VtoData.h>

#include <iostream>
#include <limits>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(SingleEventBufferSuite)
BOOST_AUTO_TEST_CASE(SingleIndexSorting)
{
	SingleEventBuffer<AnalogEvent> b(3);

	b.Update(Analog(0), PC_CLASS_1, 0);
	b.Update(Analog(0), PC_CLASS_1, 1);

	BOOST_REQUIRE_EQUAL(b.Size(), 2);

	Analog a(0); a.SetTime(2); //newer timestamp
	b.Update(a, PC_CLASS_1, 0);  //repeat index 0

	BOOST_REQUIRE_EQUAL(b.Size(), 2);
	b.Select(PC_CLASS_1);
	BOOST_REQUIRE_EQUAL(b.Begin()->mValue.GetTime(), 2); //prove the newest value was kept
}
BOOST_AUTO_TEST_SUITE_END()

// index is irrelevant in these tests, only insertion order matters
BOOST_AUTO_TEST_SUITE(InsertionOrderedEventBufferSuite)
typedef EventInfo<int> intevt;

BOOST_AUTO_TEST_CASE(InsertionOrderSorting)
{
	const size_t NUM = 3;

	InsertionOrderedEventBuffer<intevt> b(NUM);

	int vals[NUM] = {2, 1, 0};

	for(size_t i = 0; i < NUM; ++i) b.Update(vals[i], PC_CLASS_1, 0);

	{
		//now verify the order
		b.Select(PC_CLASS_1);
		EvtItr<intevt>::Type itr = b.Begin();

		for(size_t i = 0; i < b.NumSelected(); ++i) {
			BOOST_REQUIRE_EQUAL(itr->mValue, vals[i]);
			++itr;
		}
	}

	b.Deselect();

	{
		//now verify the order
		b.Select(PC_CLASS_1);
		EvtItr<intevt>::Type itr = b.Begin();

		for(size_t i = 0; i < b.NumSelected(); ++i) {
			BOOST_REQUIRE_EQUAL(itr->mValue, vals[i]);
			++itr;
		}
	}

}

BOOST_AUTO_TEST_CASE(ResetEventsProperlyOnFailure)
{
	const uint8_t NUM = 100;

	const uint8_t dataSize = 255;

	const uint8_t numEvents = NUM;

	InsertionOrderedEventBuffer<VtoEvent> b(NUM);

	VtoData info;

	size_t numResults;

	VtoDataEventIter itr;

	for (uint8_t i = 0; i < numEvents; i++) {
		uint8_t trash[dataSize];
		uint8_t j;
		for (j = 0; j < dataSize; ++j)
			trash[j] = i;

		info.Copy(trash, dataSize);
		b.Update(info, PC_CLASS_1, dataSize);
	}

	numResults = b.Size();
	BOOST_REQUIRE_EQUAL(numResults, numEvents);

	numResults = b.Select(PC_CLASS_1);
	BOOST_REQUIRE_EQUAL(numResults, numEvents);

	itr = b.Begin();
	for (size_t i = 0; i < b.NumSelected(); ++i) {
		const uint8_t* value = itr->mValue.mpData;
		for (size_t j = 0; j < dataSize; ++j) {
	//		BOOST_REQUIRE_EQUAL(value[j], i);
		}
		++itr;
	}
	b.Deselect();

	numResults = b.Select(PC_CLASS_1);
	BOOST_REQUIRE_EQUAL(numResults, numEvents);

	itr = b.Begin();
	for (size_t i = 0; i < b.NumSelected(); ++i) {
		const uint8_t* value = itr->mValue.mpData;
		for (size_t j = 0; j < dataSize; ++j) {
//			BOOST_REQUIRE_EQUAL(value[j], i);
		}
		++itr;
	}
	b.Deselect();
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TimeOrderedEventBufferSuite)
BOOST_AUTO_TEST_CASE(TimeBasedSorting)
{
	const size_t NUM = 3;
	TimeOrderedEventBuffer<BinaryEvent> b(NUM);

	millis_t times[NUM] = {3, 2, 1};

	Binary v; // false, restart, t = 0

	for(size_t i = 0; i < NUM; ++i) {
		v.SetTime(times[i]);
		b.Update(v, PC_CLASS_1, 0);
	}

	BOOST_REQUIRE_EQUAL(b.Size(), NUM);


	//now verify the order
	b.Select(PC_CLASS_1);
	EvtItr<BinaryEvent>::Type itr = b.Begin();

	for(size_t i = 0; i < b.NumSelected(); ++i) {
		BOOST_REQUIRE_EQUAL(itr->mValue.GetTime(), times[NUM - i - 1]); //reverse order of array above
		++itr;
	}
}

BOOST_AUTO_TEST_CASE(EventOverflow)
{
	TimeOrderedEventBuffer<BinaryEvent> buffer(1);
	buffer.Update(Binary(true), PC_CLASS_1, 0); 
	BOOST_REQUIRE(buffer.IsFull());
	BOOST_REQUIRE_FALSE(buffer.IsOverflown());
	buffer.Update(Binary(false), PC_CLASS_1, 1);
	BOOST_REQUIRE(buffer.IsOverflown());
	BOOST_REQUIRE_EQUAL(1, buffer.Size());	
}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
