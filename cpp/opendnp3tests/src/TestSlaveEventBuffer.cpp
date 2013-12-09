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
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"

#include <openpal/Exception.h>

#include <opendnp3/SlaveEventBuffer.h>

#include "RandomizedBuffer.h"

#include <limits>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(SlaveEventBufferSuite)
void PushEvents(SlaveEventBuffer& b, size_t aNumEvent, size_t aNumIndices)
{
	for(size_t i = 0; i < aNumEvent; ++i) {
		Event<Analog> e(Analog(i), i % aNumIndices, PC_CLASS_1);
		b.Update(e);
	}
}

template <class T>
void OverflowTest(const T& first, const T& second)
{
	EventMaxConfig cfg(1, 1, 1, 0);
	SlaveEventBuffer b(cfg);
	b.Update(Event<T>(first, 0, PC_CLASS_1));
	BOOST_REQUIRE_FALSE(b.IsOverflow());
	BOOST_REQUIRE(b.HasEventData());
	BOOST_REQUIRE_EQUAL(b.NumType(Convert(T::MeasEnum)), 1);
	b.Update(Event<T>(second, 1, PC_CLASS_1));
	BOOST_REQUIRE(b.IsOverflow());
}

BOOST_AUTO_TEST_CASE(AnalogInsertion)
{
	const size_t NUM_EVENT = 100;
	const size_t NUM_INDICES = 10;

	EventMaxConfig cfg(0, NUM_INDICES, 0, 0);

	SlaveEventBuffer b(cfg);

	/* Fill the entire buffer with events, but do not overflow */
	PushEvents(b, NUM_INDICES, NUM_INDICES);
	BOOST_REQUIRE_FALSE(b.IsOverflow());
	BOOST_REQUIRE_EQUAL(b.NumType(BufferType::ANALOG), NUM_INDICES);

	/* Overflow the event buffer */
	PushEvents(b, NUM_EVENT, NUM_INDICES);
	BOOST_REQUIRE(b.IsOverflow());
	BOOST_REQUIRE_EQUAL(b.NumType(BufferType::ANALOG), NUM_INDICES);

	b.Select(BufferType::ANALOG, PC_CLASS_1); //select all the events
	BOOST_REQUIRE_EQUAL(b.NumType(BufferType::ANALOG), NUM_INDICES);
	PushEvents(b, NUM_EVENT, NUM_INDICES);
	BOOST_REQUIRE(b.IsOverflow());
	BOOST_REQUIRE_EQUAL(b.NumType(BufferType::ANALOG), 2 * NUM_INDICES);

	//now deselect
	b.Deselect();
	BOOST_REQUIRE_EQUAL(b.NumType(BufferType::ANALOG), 2 * NUM_INDICES);
	BOOST_REQUIRE(b.IsOverflow());
}

BOOST_AUTO_TEST_CASE(OverflowAnalog)
{
	OverflowTest(Analog(5), Analog(6));
}

BOOST_AUTO_TEST_CASE(OverflowBinary)
{
	OverflowTest(Binary(true), Binary(false));
}

BOOST_AUTO_TEST_CASE(OverflowCounter)
{
	OverflowTest(Counter(1), Counter(6));
}

BOOST_AUTO_TEST_CASE(ClassSelect)
{
	EventMaxConfig cfg(10, 10, 10, 0);
	SlaveEventBuffer b(cfg);
	b.Update(Event<Binary>(Binary(true), 0, PC_CLASS_1));
	b.Update(Event<Binary>(Binary(false), 0, PC_CLASS_1));
	b.Update(Event<Analog>(Analog(5), 0, PC_CLASS_1));
	b.Update(Event<Analog>(Analog(6), 1, PC_CLASS_1));
	b.Update(Event<Counter>(Counter(1), 0, PC_CLASS_1));
	b.Update(Event<Counter>(Counter(3), 1, PC_CLASS_1));

	b.Select(PC_CLASS_1, 1);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::BINARY), 1);
	b.Deselect();

	b.Select(PC_CLASS_1, 3);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::BINARY), 2);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::ANALOG), 1);
	b.Deselect();

	b.Select(PC_CLASS_1, 5);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::BINARY), 2);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::ANALOG), 2);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::COUNTER), 1);
	b.Deselect();

	b.Select(PC_CLASS_1, 6);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::BINARY), 2);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::ANALOG), 2);
	BOOST_REQUIRE_EQUAL(b.NumSelected(BufferType::COUNTER), 2);

	BOOST_REQUIRE(!b.IsFull(BufferType::BINARY));
	BOOST_REQUIRE(!b.IsFull(BufferType::ANALOG));
	BOOST_REQUIRE(!b.IsFull(BufferType::COUNTER));
}

/* TODO - Write a Vto Only test here */
/*
BOOST_AUTO_TEST_CASE(SlaveVtoEvents)
{

}
*/

BOOST_AUTO_TEST_CASE(SimpleNegativeTests)
{
	EventMaxConfig cfg(5, 5, 5, 5);
	SlaveEventBuffer b(cfg);

	BOOST_REQUIRE_FALSE(b.NumSelected(BufferType::INVALID));
	BOOST_REQUIRE_FALSE(b.NumType(BufferType::INVALID));
	BOOST_REQUIRE_FALSE(b.Select(BufferType::INVALID, PC_CLASS_1, 1));
	BOOST_REQUIRE_FALSE(b.IsFull(BufferType::INVALID));
}
BOOST_AUTO_TEST_SUITE_END()
