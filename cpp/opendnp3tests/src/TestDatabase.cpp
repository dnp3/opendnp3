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

#include "DatabaseTestObject.h"

#include <limits>

using namespace std;
using namespace openpal;
using namespace opendnp3;

template <class T>
void TestDataEvent(bool aIsEvent, const T& val1, const T& val2, double aDeadband)
{
	if(aIsEvent) {
		BOOST_REQUIRE(val1.ShouldGenerateEvent(val2, aDeadband, val1.GetValue()));
	}
	else {
		BOOST_REQUIRE_FALSE(val1.ShouldGenerateEvent(val2, aDeadband, val1.GetValue()));
	}
}

template <class T>
void TestBufferForEvent(bool aIsEvent, const T& arNewVal, DatabaseTestObject& test, std::deque< PointInfo <T> >& arQueue)
{
	Transaction tr(&test.db);
	test.db.Update(arNewVal, 0);

	if(aIsEvent) {
		BOOST_REQUIRE_EQUAL(arQueue.size(), 1);
		BOOST_REQUIRE_EQUAL(arNewVal, arQueue.front().mValue);
		BOOST_REQUIRE_EQUAL(0, arQueue.front().mIndex);
		arQueue.pop_front();
	}
	else {
		BOOST_REQUIRE_EQUAL(arQueue.size(), 0);
	}
}

BOOST_AUTO_TEST_SUITE(TestDatabase)
// Show that updating a value with an invalid index throws an exception
BOOST_AUTO_TEST_CASE(IndexOutOfBounds)
{
	DatabaseTestObject t;
	t.db.Configure(DT_BINARY, 1);

	Transaction tr(&t.db);
	Binary b;
	BOOST_REQUIRE_THROW(t.db.Update(b, 1), IndexOutOfBoundsException);
}

// tests for the various analog event conditions

BOOST_AUTO_TEST_CASE(AnalogEventZeroDeadband)
{
	TestDataEvent(true, Analog(0), Analog(1), 0);
}

BOOST_AUTO_TEST_CASE(AnalogEventOnDeadband)
{
	TestDataEvent(false, Analog(0), Analog(1), 1);
}

BOOST_AUTO_TEST_CASE(AnalogEventNegative)
{
	TestDataEvent(true, Analog(-34), Analog(-36), 1);
}

BOOST_AUTO_TEST_CASE(AnalogNoEventNegative)
{
	TestDataEvent(false, Analog(-34), Analog(-36), 2);
}

BOOST_AUTO_TEST_CASE(AnalogMaxRangeNoEvent)
{
	TestDataEvent(false, Analog(Analog::MAX_VALUE), Analog(0), std::numeric_limits<Analog::Type>::max());
	TestDataEvent(false, Analog(0), Analog(Analog::MAX_VALUE), std::numeric_limits<Analog::Type>::max());
}

BOOST_AUTO_TEST_CASE(AnalogMaxRangeEvent)
{
	TestDataEvent(true, Analog(Analog::MAX_VALUE), Analog(Analog::MIN_VALUE), std::numeric_limits<Analog::Type>::max());
	TestDataEvent(true, Analog(Analog::MIN_VALUE), Analog(Analog::MAX_VALUE), std::numeric_limits<Analog::Type>::max());
}

BOOST_AUTO_TEST_CASE(CounterMaxRangeNoEvent)
{
	TestDataEvent(false, Counter(Counter::MAX_VALUE), Counter(Counter::MIN_VALUE), std::numeric_limits<uint32_t>::max());
	TestDataEvent(false, Counter(Counter::MIN_VALUE), Counter(Counter::MAX_VALUE), std::numeric_limits<uint32_t>::max());
}

BOOST_AUTO_TEST_CASE(CounterMaxRangeEvent)
{
	TestDataEvent(true, Counter(Counter::MAX_VALUE), Counter(Counter::MIN_VALUE), std::numeric_limits<uint32_t>::max() - 1);
	TestDataEvent(true, Counter(Counter::MIN_VALUE), Counter(Counter::MAX_VALUE), std::numeric_limits<uint32_t>::max() - 1);
}

// Next 3 tests prove that "no change" doesn't get forwared to IEventBuffer
BOOST_AUTO_TEST_CASE(BinaryNoChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, 0, PC_CLASS_1);
	TestBufferForEvent(false, Binary(false), t, t.buffer.mBinaryEvents);
}

BOOST_AUTO_TEST_CASE(AnalogNoChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_ANALOG, 1);
	t.db.SetClass(DT_ANALOG, 0, PC_CLASS_1);
	TestBufferForEvent(false, Analog(0), t, t.buffer.mAnalogEvents);
}

BOOST_AUTO_TEST_CASE(CounterNoChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_COUNTER, 1);
	t.db.SetClass(DT_COUNTER, 0, PC_CLASS_1);
	TestBufferForEvent(false, Counter(0), t, t.buffer.mCounterEvents);
}

// Next 3 tests prove that a change detection will forward to the buffer
BOOST_AUTO_TEST_CASE(BinaryChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, 0, PC_CLASS_1);
	TestBufferForEvent(true, Binary(false, BQ_ONLINE), t, t.buffer.mBinaryEvents);
}

BOOST_AUTO_TEST_CASE(AnalogChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_ANALOG, 1);
	t.db.SetClass(DT_ANALOG, 0, PC_CLASS_1);
	TestBufferForEvent(true, Analog(0, AQ_ONLINE), t, t.buffer.mAnalogEvents);
}

BOOST_AUTO_TEST_CASE(CounterChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_COUNTER, 1);
	t.db.SetClass(DT_COUNTER, 0, PC_CLASS_1);
	TestBufferForEvent(true, Counter(0, CQ_ONLINE), t, t.buffer.mCounterEvents);
}

//show that the last reported change gets recorded correctly and applied correctly for each type
BOOST_AUTO_TEST_CASE(AnalogLastReportedChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_ANALOG, 1);
	t.db.SetClass(DT_ANALOG, 0, PC_CLASS_1);
	t.db.SetDeadband(DT_ANALOG, 0, 5); //value must change by more than 5 before being reported

	TestBufferForEvent(false, Analog(-2), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(false, Analog(5), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(true, Analog(6), t, t.buffer.mAnalogEvents); //change by 6, so 6 should get recorded
	TestBufferForEvent(false, Analog(1), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(true, Analog(-1), t, t.buffer.mAnalogEvents);
}

BOOST_AUTO_TEST_CASE(CounterLastReportedChange)
{
	DatabaseTestObject t;
	t.db.Configure(DT_COUNTER, 1);
	t.db.SetClass(DT_COUNTER, 0, PC_CLASS_1);
	t.db.SetDeadband(DT_COUNTER, 0, 5); //value must change by more than 5 before being reported

	TestBufferForEvent(false, Counter(1), t, t.buffer.mCounterEvents);
	TestBufferForEvent(false, Counter(5), t, t.buffer.mCounterEvents);
	TestBufferForEvent(true, Counter(6), t, t.buffer.mCounterEvents); //change by 6, so 6 should get recorded
	TestBufferForEvent(false, Counter(1), t, t.buffer.mCounterEvents);
	TestBufferForEvent(true, Counter(0), t, t.buffer.mCounterEvents);
}

BOOST_AUTO_TEST_SUITE_END()
