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

#include <openpal/StaticArray.h>
#include <openpal/ListAdapter.h>
#include <openpal/StaticList.h>

using namespace openpal;

BOOST_AUTO_TEST_SUITE(ListAdapterTestSuite)

BOOST_AUTO_TEST_CASE(CorrectInitialState)
{
	StaticArray<int, int, 3> array;
	ListAdapter<int, int> list(array.ToIndexable());

	BOOST_REQUIRE_EQUAL(0, list.Size());
	BOOST_REQUIRE_EQUAL(3, list.Capacity());
}

BOOST_AUTO_TEST_CASE(CanAddUntilFull)
{
	StaticArray<uint32_t, uint16_t, 3> array;
	ListAdapter<uint32_t, uint16_t> list(array.ToIndexable());

	BOOST_REQUIRE(list.Add(2));
	BOOST_REQUIRE(list.Add(4));
	BOOST_REQUIRE(list.Add(6));

	BOOST_REQUIRE_EQUAL(3, list.Size());
	list.foreachIndex([](const uint32_t& value, uint32_t i) { BOOST_REQUIRE_EQUAL((i+1)*2, value); });

	BOOST_REQUIRE(!list.Add(8));
	BOOST_REQUIRE_EQUAL(3, list.Size());
}
BOOST_AUTO_TEST_CASE(StaticListHasSameBehavior)
{
	StaticList<uint32_t, uint8_t, 3> list;

	BOOST_REQUIRE_EQUAL(list.Size(), 0);
	BOOST_REQUIRE_EQUAL(list.Capacity(), 3);
	
}


BOOST_AUTO_TEST_SUITE_END()
