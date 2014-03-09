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


#include "MeasurementComparisons.h"
#include "DatabaseTestObject.h"

#include <opendnp3/app/MeasurementHelpers.h>

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
		REQUIRE(arQueue.size() ==  1);
		REQUIRE((arNewVal == arQueue.front().value));
		REQUIRE(0 ==  arQueue.front().index);
		arQueue.pop_front();
	}
	else {
		REQUIRE(arQueue.size() ==  0);
	}
}

bool IsAnalogEvent(double val1, double val2, double deadband)
{
	Analog a1(val1);	
	Analog a2(val2);
	return a1.IsEvent(a2, deadband);
}

bool IsCounterEvent(uint32_t val1, uint32_t val2, uint32_t deadband)
{
	Counter c1(val1);
	Counter c2(val2);
	return c1.IsEvent(c2, deadband);
}

#define SUITE(name) "DatabaseTestSuite - " name

// tests for the various analog event conditions
TEST_CASE(SUITE("AnalogEventZeroDeadband"))
{	
	REQUIRE_FALSE(IsAnalogEvent(0, 0, 0));
}

TEST_CASE(SUITE("AnalogEventOnDeadband"))
{	
	REQUIRE_FALSE(IsAnalogEvent(0, 1, 1));	
}

TEST_CASE(SUITE("AnalogEventNegative"))
{
	REQUIRE(IsAnalogEvent(-34, -36, 1));	
}

TEST_CASE(SUITE("AnalogInfiniteChange"))
{
	auto maxAnalog = std::numeric_limits<double>::max();
	auto minAnalog = -maxAnalog;

	REQUIRE(IsAnalogEvent(minAnalog, maxAnalog, 1));	
}

TEST_CASE(SUITE("CounterMaxChange"))
{
	auto max = std::numeric_limits<uint32_t>::max();	
	REQUIRE(IsCounterEvent(0, max, max -1));	
}

TEST_CASE(SUITE("AnalogNoEventNegative"))
{
	REQUIRE_FALSE(IsAnalogEvent(-34, -36, 2));	
}

// Next 3 tests prove that "no change" doesn't get forwared to IEventBuffer
TEST_CASE(SUITE("BinaryNoChange"))
{	
	DatabaseTestObject t(DatabaseTemplate::BinaryOnly(1));
	t.db.staticData.binaries.metadata[0].clazz = CLASS_1;	
	TestBufferForEvent(false, Binary(false, BQ_RESTART), t, t.buffer.mBinaryEvents);
}

TEST_CASE(SUITE("AnalogNoChange"))
{
	DatabaseTestObject t(DatabaseTemplate::AnalogOnly(1));	
	t.db.staticData.analogs.metadata[0].clazz = CLASS_1;
	TestBufferForEvent(false, Analog(0, AQ_RESTART), t, t.buffer.mAnalogEvents);
}

TEST_CASE(SUITE("CounterNoChange"))
{
	DatabaseTestObject t(DatabaseTemplate::CounterOnly(1));	
	t.db.staticData.counters.metadata[0].clazz = CLASS_1;
	TestBufferForEvent(false, Counter(0, CQ_RESTART), t, t.buffer.mCounterEvents);
}

// Next 3 tests prove that a change detection will forward to the buffer
TEST_CASE(SUITE("BinaryChange"))
{
	DatabaseTestObject t(DatabaseTemplate::BinaryOnly(1));
	t.db.staticData.binaries.metadata[0].clazz = CLASS_1;
	TestBufferForEvent(true, Binary(false, BQ_ONLINE), t, t.buffer.mBinaryEvents);
}

TEST_CASE(SUITE("AnalogChange"))
{
	DatabaseTestObject t(DatabaseTemplate::AnalogOnly(1));	
	t.db.staticData.analogs.metadata[0].clazz = CLASS_1;
	TestBufferForEvent(true, Analog(0, AQ_ONLINE), t, t.buffer.mAnalogEvents);
}

TEST_CASE(SUITE("CounterChange"))
{
	DatabaseTestObject t(DatabaseTemplate::CounterOnly(1));	
	t.db.staticData.counters.metadata[0].clazz = CLASS_1;
	TestBufferForEvent(true, Counter(0, CQ_ONLINE), t, t.buffer.mCounterEvents);
}

//show that the last reported change gets recorded correctly and applied correctly for each type
TEST_CASE(SUITE("AnalogLastReportedChange"))
{
	DatabaseTestObject t(DatabaseTemplate::AnalogOnly(1));	
	t.db.staticData.analogs.metadata[0].clazz = CLASS_1;
	t.db.staticData.analogs.metadata[0].deadband = 5; //value must change by more than 5 before being reported	
	
	TestBufferForEvent(false, Analog(-2, AQ_RESTART), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(false, Analog(5, AQ_RESTART), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(true, Analog(6, AQ_RESTART), t, t.buffer.mAnalogEvents); //change by 6, so 6 should get recorded
	TestBufferForEvent(false, Analog(1, AQ_RESTART), t, t.buffer.mAnalogEvents);
	TestBufferForEvent(true, Analog(-1, AQ_RESTART), t, t.buffer.mAnalogEvents);
}

TEST_CASE(SUITE("CounterLastReportedChange"))
{
	DatabaseTestObject t(DatabaseTemplate::CounterOnly(1));	
	t.db.staticData.counters.metadata[0].clazz = CLASS_1;
	t.db.staticData.counters.metadata[0].deadband = 5;	

	TestBufferForEvent(false, Counter(1, CQ_RESTART), t, t.buffer.mCounterEvents);
	TestBufferForEvent(false, Counter(5, CQ_RESTART), t, t.buffer.mCounterEvents);
	TestBufferForEvent(true, Counter(6, CQ_RESTART), t, t.buffer.mCounterEvents); //change by 6, so 6 should get recorded
	TestBufferForEvent(false, Counter(1, CQ_RESTART), t, t.buffer.mCounterEvents);
	TestBufferForEvent(true, Counter(0, CQ_RESTART), t, t.buffer.mCounterEvents);
}


