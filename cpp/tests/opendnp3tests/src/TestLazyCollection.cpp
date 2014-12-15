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

#include "BufferHelpers.h"
#include "MeasurementComparisons.h"

#include <opendnp3/app/IterableTransforms.h>

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/app/BitReader.h>
#include <opendnp3/objects/Group30.h>

#include <iostream>

using namespace std;
using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "LazyCollectionTestSuite - " name

TEST_CASE(SUITE("ReadSimpleTypes"))
{
	HexSequence hex("AB 01 01 CD 02 00");

	auto collection = CreateLazyIterable<Group30Var2>(hex.ToReadOnly(), 2,
	                  [](ReadBufferView & b, uint32_t)
	{
		return Group30Var2::Read(b);
	}
	                                                 );

	auto test = [&]()
	{
		std::vector<Group30Var2> vec;
		collection.foreach([&](const Group30Var2 & gv)
		{
			vec.push_back(gv);
		});

		REQUIRE(2 ==  vec.size());
		REQUIRE(257 ==  vec[0].value);
		REQUIRE(0xAB ==  vec[0].flags);
		REQUIRE(2 ==  vec[1].value);
		REQUIRE(0xCD ==  vec[1].flags);
	};

	// calling the function 2x proves that the buffer can be read again.
	test();
	test();
}

TEST_CASE(SUITE("SingleBitValue"))
{
	HexSequence hex("01");
	auto collection = CreateLazyIterable<Binary>(hex.ToReadOnly(), 1,
	                  [](ReadBufferView & buff, uint32_t pos)
	{
		return Binary(GetBit(buff, pos));
	}
	                                            );
	REQUIRE(1 ==  collection.Count());
	std::vector<Binary> values;
	collection.foreach([&](const Binary & v)
	{
		values.push_back(v);
	});
	REQUIRE(true ==  values[0].value);
}

TEST_CASE(SUITE("ComplexBitCount"))
{
	HexSequence hex("FF 00 00");
	auto collection = CreateLazyIterable<bool>(hex.ToReadOnly(), 17,
	                  [](ReadBufferView & buff, uint32_t pos)
	{
		return GetBit(buff, pos);
	}
	                                          );
	std::vector<bool> values;
	collection.foreach([&](const bool & v)
	{
		values.push_back(v);
	});

	REQUIRE(17 ==  values.size());
	REQUIRE(true ==  values[7]);
	REQUIRE(false ==  values[8]);
}

TEST_CASE(SUITE("HighestBitSet"))
{
	HexSequence hex("80");

	auto collection = CreateLazyIterable<bool>(hex.ToReadOnly(), 8,
	                  [](ReadBufferView & buffer, uint32_t pos)
	{
		return GetBit(buffer, pos);
	}
	                                          );

	auto collection2 = MapIterableBuffer<bool, Binary>(&collection,
	                   [](const bool & bit)
	{
		return Binary(bit);
	}
	                                                  );

	std::vector<Binary> values;
	collection2.foreach([&](const Binary & v)
	{
		values.push_back(v);
	});
	REQUIRE(8 ==  values.size());
	REQUIRE((Binary(false) == values[6]));
	REQUIRE((Binary(true) == values[7]));
}
