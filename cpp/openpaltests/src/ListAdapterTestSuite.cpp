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

#include <openpal/StaticArray.h>
#include <openpal/ListAdapter.h>
#include <openpal/StaticList.h>

using namespace openpal;

#define SUITE(name) "ListAdapter - " name

TEST_CASE(SUITE("CorrectInitialState"))
{
	StaticArray<int, int, 3> array;
	ListAdapter<int, int> list(array.ToIndexable());

	REQUIRE(0 == list.Size());
	REQUIRE(3 == list.Capacity());
}

TEST_CASE(SUITE("CanAddUntilFull"))
{
	StaticArray<uint32_t, uint16_t, 3> array;
	ListAdapter<uint32_t, uint16_t> list(array.ToIndexable());

	REQUIRE(list.Add(2));
	REQUIRE(list.Add(4));
	REQUIRE(list.Add(6));

	REQUIRE(3 == list.Size());
	list.foreachIndex([](const uint32_t& value, uint32_t i) { REQUIRE(((i+1)*2) == value); });

	REQUIRE(!list.Add(8));
	REQUIRE(3 == list.Size());
}
TEST_CASE(SUITE("StaticListHasSameBehavior"))
{
	StaticList<uint32_t, uint8_t, 3> list;

	REQUIRE(list.Size() == 0);
	REQUIRE(list.Capacity() == 3);
	
}


