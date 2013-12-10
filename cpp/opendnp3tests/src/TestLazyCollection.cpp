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

#include <opendnp3/LazyIterable.h>

#include <opendnp3/DataTypes.h>
#include <opendnp3/BitReader.h>
#include <opendnp3/objects/Group30.h>

#include <iostream>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(LazyCollectionTestSuite)

BOOST_AUTO_TEST_CASE(ReadSimpleTypes)
{
	HexSequence hex("AB 01 01 CD 02 00");

	auto read = [](ReadOnlyBuffer& b, size_t position) { return Group30Var2::Read(b); };
		
	LazyIterable<Group30Var2> collection(hex.ToReadOnly(), 2, read);
	
	auto test = [&]() {
		std::vector<Group30Var2> vec;
		collection.Foreach([&](const Group30Var2& gv) { vec.push_back(gv); });
								
		BOOST_REQUIRE_EQUAL(2, vec.size());
		BOOST_REQUIRE_EQUAL(257, vec[0].value);
		BOOST_REQUIRE_EQUAL(0xAB, vec[0].flags);
		BOOST_REQUIRE_EQUAL(2, vec[1].value);
		BOOST_REQUIRE_EQUAL(0xCD, vec[1].flags);
	};

	// calling the function 2x proves that the buffer can be read again.
	test();
	test();
}

BOOST_AUTO_TEST_CASE(SingleBitValue)
{
	HexSequence hex("01");
	LazyIterable<Binary> collection(hex.ToReadOnly(), 1, GetBit);
	BOOST_REQUIRE_EQUAL(1, collection.Size());
	std::vector<Binary> values;	
	collection.Foreach([&](const Binary& v) { values.push_back(v); });
}

BOOST_AUTO_TEST_CASE(ComplexBitCount)
{
	HexSequence hex("FF 00 00");
	LazyIterable<Binary> collection(hex.ToReadOnly(), 17, GetBit);
	std::vector<Binary> values;
	collection.Foreach([&](const Binary& v) { values.push_back(v); });
	
	BOOST_REQUIRE_EQUAL(17, values.size());
	BOOST_REQUIRE(Binary(true) == values[7]);
	BOOST_REQUIRE(Binary(false) == values[8]);
}

BOOST_AUTO_TEST_CASE(HighestBitSet)
{
	HexSequence hex("80");
	LazyIterable<Binary> collection(hex.ToReadOnly(), 8, GetBit);	
	std::vector<Binary> values;
	collection.Foreach([&](const Binary& v) { values.push_back(v); });	
	BOOST_REQUIRE_EQUAL(8, values.size());
	BOOST_REQUIRE(Binary(true) == values[7]);
}

BOOST_AUTO_TEST_SUITE_END()