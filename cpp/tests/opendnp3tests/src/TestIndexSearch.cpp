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

#include <openpal/container/DynamicArray.h>

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/outstation/IndexSearch.h>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "IndexSearch - " name

IndexSearch::Result TestResultLengthFour(uint16_t index)
{
	DynamicArray<Cell<Binary>, uint16_t> values(4);
	values[0].vIndex = 1;
	values[1].vIndex = 3;
	values[2].vIndex = 7;
	values[3].vIndex = 9;

	return IndexSearch::FindClosestRawIndex(values.ToView(), index);
}

TEST_CASE(SUITE("FindsExactMatchForLastValue"))
{	
	auto result = TestResultLengthFour(9);
	REQUIRE(result.match);
	REQUIRE(result.index == 3);
}

TEST_CASE(SUITE("FindsExactMatchForFirstValue"))
{
	auto result = TestResultLengthFour(1);
	REQUIRE(result.match);
	REQUIRE(result.index == 0);
}

TEST_CASE(SUITE("StopsOnFirstValueIfIndexLessThanFirst"))
{
	auto result = TestResultLengthFour(0);
	REQUIRE(!result.match);
	REQUIRE(result.index == 0);
}

TEST_CASE(SUITE("StopsOnLastValueIfIndexGreaterThanLast"))
{
	auto result = TestResultLengthFour(11);
	REQUIRE(!result.match);
	REQUIRE(result.index == 3);
}

TEST_CASE(SUITE("FindsNextSmallestIndex"))
{
	auto result = TestResultLengthFour(2);
	REQUIRE(!result.match);
	REQUIRE(result.index == 0);
}

TEST_CASE(SUITE("FindsNextBiggestIndex"))
{
	auto result = TestResultLengthFour(8);
	REQUIRE(!result.match);
	REQUIRE(result.index == 3);
}
