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
#include "BufferHelpers.h"
#include "MeasurementComparisons.h"

#include <opendnp3/BitReader.h>
#include <opendnp3/LazyFixedSizeCollection.h>

#include <iostream>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(LazyBitCollectionTestSuite)

BOOST_AUTO_TEST_CASE(SingleValue)
{
	HexSequence hex("01");
	LazyFixedSizeCollection<Binary> collection(hex.ToReadOnly(), 1, GetBit);
	BOOST_REQUIRE_EQUAL(1, collection.size());
	std::vector<Binary> values;
	for(auto b: collection) values.push_back(b);
}

BOOST_AUTO_TEST_CASE(ComplexCount)
{
	HexSequence hex("FF 00 00");
	LazyFixedSizeCollection<Binary> collection(hex.ToReadOnly(), 17, GetBit);
	std::vector<Binary> values;
	for(auto b: collection) values.push_back(b);
	
	BOOST_REQUIRE_EQUAL(17, values.size());
	BOOST_REQUIRE(Binary(true) == values[7]);
	BOOST_REQUIRE(Binary(false) == values[8]);
}

BOOST_AUTO_TEST_CASE(HighestBitSet)
{
	HexSequence hex("80");
	LazyFixedSizeCollection<Binary> collection(hex.ToReadOnly(), 8, GetBit);	
	std::vector<Binary> values;
	for(auto b: collection) values.push_back(b);	
	BOOST_REQUIRE_EQUAL(8, values.size());
	BOOST_REQUIRE(Binary(true) == values[7]);
}

BOOST_AUTO_TEST_SUITE_END()