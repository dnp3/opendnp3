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
#include "MeasurementComparisons.h"
#include "DatabaseTestObject.h"

#include <opendnp3/MeasurementHelpers.h>

#include <limits>

using namespace std;
using namespace openpal;
using namespace opendnp3;

template <class T>
void TestBufferForEvent(bool aIsEvent, const T& arNewVal, DatabaseTestObject& test, std::deque< Event <T> >& arQueue)
{
	Transaction tr(&test.db);
	test.db.Update(arNewVal, 0);

	if(aIsEvent) {
		BOOST_REQUIRE_EQUAL(arQueue.size(), 1);
		BOOST_REQUIRE(arNewVal == arQueue.front().value);
		BOOST_REQUIRE_EQUAL(0, arQueue.front().index);
		arQueue.pop_front();
	}
	else {
		BOOST_REQUIRE_EQUAL(arQueue.size(), 0);
	}
}

BOOST_AUTO_TEST_SUITE(DatabaseTestSuite)

// tests for the various analog event conditions
BOOST_AUTO_TEST_CASE(AnalogEventZeroDeadband)
{
	PointInfo<Analog> pi;
	pi.deadband = 0.0;	
	BOOST_REQUIRE(pi.Load(Analog(0.0)));	
}

BOOST_AUTO_TEST_CASE(AnalogEventOnDeadband)
{
	PointInfo<Analog> pi;
	pi.deadband = 1.0;
	pi.lastEvent = pi.value = Analog(0.0);
	BOOST_REQUIRE_FALSE(pi.Load(Analog(1.0)));
}

BOOST_AUTO_TEST_CASE(AnalogEventNegative)
{
	PointInfo<Analog> pi;
	pi.deadband = 1.0;
	pi.value = pi.lastEvent = Analog(-34);	
	BOOST_REQUIRE(pi.Load(Analog(-36)));
}

BOOST_AUTO_TEST_CASE(AnalogInfiniteChange)
{
	PointInfo<Analog> pi;
	pi.deadband = 1.0;
	pi.value = pi.lastEvent = Analog(std::numeric_limits<double>::max());	
	BOOST_REQUIRE(pi.Load(Analog(-std::numeric_limits<double>::max())));
}

BOOST_AUTO_TEST_CASE(CounterMaxChange)
{
	PointInfo<Counter> pi;
	pi.deadband = std::numeric_limits<uint32_t>::max() - 1;
	pi.value = pi.lastEvent = Counter(0);	
	BOOST_REQUIRE(pi.Load(Counter(std::numeric_limits<uint32_t>::max())));
}

BOOST_AUTO_TEST_CASE(AnalogNoEventNegative)
{
	PointInfo<Analog> pi;
	pi.deadband = 2.0;
	pi.value = pi.lastEvent = Analog(-34);
	BOOST_REQUIRE_FALSE(pi.Load(Analog(-36)));
}

// Next 3 tests prove that "no change" doesn't get forwared to IEventBuffer
BOOST_AUTO_TEST_CASE(BinaryNoChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, 0, PC_CLASS_1);
	TestBufferForEvent(false, Binary(false, BQ_RESTART), t, t.buffer.mBinaryEvents);
}

BOOST_AUTO_TEST_CASE(AnalogNoChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::ANALOG, 1);
	t.db.SetClass(MeasurementType::ANALOG, 0, PC_CLASS_1);
	TestBufferForEvent(false, Analog(0, AQ_RESTART), t, t.buffer.mAnalogEvents);
}

BOOST_AUTO_TEST_CASE(CounterNoChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::COUNTER, 1);
	t.db.SetClass(MeasurementType::COUNTER, 0, PC_CLASS_1);
	TestBufferForEvent(false, Counter(0, CQ_RESTART), t, t.buffer.mCounterEvents);
}

// Next 3 tests prove that a change detection will forward to the buffer
BOOST_AUTO_TEST_CASE(BinaryChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, 0, PC_CLASS_1);
	TestBufferForEvent(true, Binary(false, BQ_ONLINE), t, t.buffer.mBinaryEvents);
}

BOOST_AUTO_TEST_CASE(AnalogChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::ANALOG, 1);
	t.db.SetClass(MeasurementType::ANALOG, 0, PC_CLASS_1);
	TestBufferForEvent(true, Analog(0, AQ_ONLINE), t, t.buffer.mAnalogEvents);
}

BOOST_AUTO_TEST_CASE(CounterChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::COUNTER, 1);
	t.db.SetClass(MeasurementType::COUNTER, 0, PC_CLASS_1);
	TestBufferForEvent(true, Counter(0, CQ_ONLINE), t, t.buffer.mCounterEvents);
}

//show that the last reported change gets recorded correctly and applied correctly for each type
BOOST_AUTO_TEST_CASE(AnalogLastReportedChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::ANALOG, 1);
	t.db.SetClass(MeasurementType::ANALOG, 0, PC_CLASS_1);
	t.db.SetDeadband(MeasurementType::ANALOG, 0, 5); //value must change by more than 5 before being reported

	TestBufferForEvent(false, Analog(-2, AQ_RESTART), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(false, Analog(5, AQ_RESTART), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(true, Analog(6, AQ_RESTART), t, t.buffer.mAnalogEvents); //change by 6, so 6 should get recorded
	TestBufferForEvent(false, Analog(1, AQ_RESTART), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(true, Analog(-1, AQ_RESTART), t, t.buffer.mAnalogEvents);
}

BOOST_AUTO_TEST_CASE(CounterLastReportedChange)
{
	DatabaseTestObject t;
	t.db.Configure(MeasurementType::COUNTER, 1);
	t.db.SetClass(MeasurementType::COUNTER, 0, PC_CLASS_1);
	t.db.SetDeadband(MeasurementType::COUNTER, 0, 5); //value must change by more than 5 before being reported

	TestBufferForEvent(false, Counter(1, CQ_RESTART), t, t.buffer.mCounterEvents);
	TestBufferForEvent(false, Counter(5, CQ_RESTART), t, t.buffer.mCounterEvents);
	TestBufferForEvent(true, Counter(6, CQ_RESTART), t, t.buffer.mCounterEvents); //change by 6, so 6 should get recorded
	TestBufferForEvent(false, Counter(1, CQ_RESTART), t, t.buffer.mCounterEvents);
	TestBufferForEvent(true, Counter(0, CQ_RESTART), t, t.buffer.mCounterEvents);
}

BOOST_AUTO_TEST_SUITE_END()
