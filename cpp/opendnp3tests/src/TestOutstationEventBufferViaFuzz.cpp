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

#include "MockEventWriter.h"
#include "OutstationEventBufferFuzzer.h"

using namespace std;
using namespace opendnp3;

void AssertCountInvariant(const ClassCount& selected, const ClassCount& unselected, const ClassCount& total)
{
	auto sum = selected.Total() + unselected.Total();
	REQUIRE(sum ==  total.Total());
}

#define SUITE(name) "OutstationEventBufferFuzzTestSuite - " name

/*
TEST_CASE(SUITE("FuzzTests"))
{
	const size_t iterations = 100*1000;

	DynamicallyAllocatedEventBuffer underlying(200, 100, 50, 50);
	OutstationEventBuffer buffer(underlying.GetFacade());

	EventBufferFuzzer fuzzer;
	MockEventWriter writer;
	
	for(size_t i = 0; i < iterations; ++i)
	{
		auto numSelected = fuzzer.PerformAction(buffer, writer); // do something
		
		//assert invariants

		REQUIRE(numSelected ==  writer.TotalEvents());

		auto selected = buffer.SelectedEvents();
		auto unselected = buffer.UnselectedEvents();
		auto total = buffer.TotalEvents();

		REQUIRE(selected.class1.Total() + unselected.class1.Total() ==  total.class1.Total());
		REQUIRE(selected.class2.Total() + unselected.class2.Total() ==  total.class2.Total());
		REQUIRE(selected.class3.Total() + unselected.class3.Total() ==  total.class3.Total());
		
		writer.Clear(); //prepare writer for next iteration
	}	
}
*/



