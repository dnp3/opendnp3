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

#include <opendnp3/outstation/OutstationEventBuffer.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>

#include "MeasurementComparisons.h"

#include "MockEventWriter.h"

#include <iostream>

using namespace std;
using namespace opendnp3;

#define SUITE(name) "OutstationEventBufferTestSuite - " name

/* TODO
TEST_CASE(SUITE("InitialState"))
{
	DynamicallyAllocatedEventBuffer underlying(3,3,3,3);
	OutstationEventBuffer buffer(underlying.GetFacade());

	MockEventWriter writer;

	SelectionCriteria criteria;	
	REQUIRE(0, buffer.SelectEvents(criteria ==  writer));
	REQUIRE(0, buffer.SelectEvents(criteria ==  writer));
	REQUIRE(0 ==  writer.TotalEvents());
}

TEST_CASE(SUITE("StaticAllocation"))
{
	//StaticallyAllocatedEventBuffer<100, 100, 100> underlying;

	openpal::StaticArray<uint16_t, uint16_t, 100> underlying;

	std::cout << sizeof(underlying) << std::endl;

	
	//OutstationEventBuffer buffer(underlying.GetFacade());

	//MockEventWriter writer;

	//SelectionCriteria criteria;	
	//REQUIRE(0, buffer.SelectEvents(criteria ==  writer));
	//REQUIRE(0, buffer.SelectEvents(criteria ==  writer));
	//REQUIRE(0 ==  writer.TotalEvents());
	
}

TEST_CASE(SUITE("SingleValueIsWrittenAndCleared"))
{
	DynamicallyAllocatedEventBuffer underlying(3,3,3,3);
	OutstationEventBuffer buffer(underlying.GetFacade());

	buffer.Update(Event<Binary>(Binary(true), 3, EventClass::EC1));

	MockEventWriter writer;

	SelectionCriteria criteria;
	criteria.class1 = EventTypeMasks::BINARY;
	
	REQUIRE(1, buffer.SelectEvents(criteria ==  writer));
	REQUIRE(1 ==  writer.TotalEvents());
	REQUIRE(0, buffer.SelectEvents(criteria ==  writer)); //second select does nothing
	REQUIRE(1 ==  writer.TotalEvents());

	REQUIRE(1 ==  buffer.SelectedEvents().class1.numBinary);
	REQUIRE(0 ==  buffer.SelectedEvents().class2.numBinary);
	REQUIRE(0 ==  buffer.SelectedEvents().class3.numBinary);
	REQUIRE(0 ==  buffer.SelectedEvents().class1.numAnalog);

	REQUIRE(0 ==  buffer.UnselectedEvents().class1.numBinary);

	buffer.Reset();

	REQUIRE(1, buffer.SelectEvents(criteria ==  writer)); //event goes right back into buffer
	REQUIRE(2 ==  writer.TotalEvents());
	REQUIRE((writer.binaries[0] == writer.binaries[1])); // same event

	buffer.Clear();

	REQUIRE(buffer.TotalEvents().IsEmpty());
	REQUIRE(buffer.SelectedEvents().IsEmpty());
	REQUIRE(buffer.UnselectedEvents().IsEmpty());


}

TEST_CASE(SUITE("MixedTypesAndClassesOfEvents"))
{
	DynamicallyAllocatedEventBuffer underlying(3,3,3, 3);
	OutstationEventBuffer buffer(underlying.GetFacade());
	
	buffer.Update(Event<Binary>(Binary(true), 3, EventClass::EC1));
	buffer.Update(Event<Binary>(Binary(true), 5, EventClass::EC3));
	buffer.Update(Event<Analog>(Analog(16), 1, EventClass::EC2));
	buffer.Update(Event<Analog>(Analog(71), 7, EventClass::EC3));
	buffer.Update(Event<Counter>(Counter(23), 3, EventClass::EC2));
	buffer.Update(Event<Counter>(Counter(42), 4, EventClass::EC2));
	buffer.Update(Event<Counter>(Counter(81), 9, EventClass::EC3));
		
	//construct a select query that designates much, but not all of the data in the buffer
	SelectionCriteria criteria;
	criteria.class1 = EventTypeMasks::BINARY;
	criteria.class2 = EventTypeMasks::COUNTER;
	criteria.class3 = EventTypeMasks::ANALOG | EventTypeMasks::COUNTER;
	
	REQUIRE(5 ==  buffer.NumUnselectedMatching(criteria));

	MockEventWriter writer;
	uint32_t selected = buffer.SelectEvents(criteria, writer);
	
	REQUIRE(5 ==  selected);
	REQUIRE(5 ==  buffer.SelectedEvents().Total());
	REQUIRE(5 ==  writer.TotalEvents());

	auto unselected = buffer.UnselectedEvents();
	REQUIRE(0 ==  unselected.class1.Total());
	REQUIRE(2 ==  unselected.class2.Total());
	REQUIRE(0 ==  unselected.class3.Total());
	REQUIRE(2 ==  unselected.Total());

	REQUIRE(buffer.UnselectedEvents().IsEmpty());
}
*/


