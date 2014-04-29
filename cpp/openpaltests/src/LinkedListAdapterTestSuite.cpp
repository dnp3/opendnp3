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

#include <openpal/DynamicArray.h>
#include <openpal/LinkedListAdapter.h>
#include <openpal/StaticLinkedList.h>

using namespace openpal;

#define SUITE(name) "LinkedListAdapter - " name

TEST_CASE(SUITE("CorrectInitialState"))
{
	StaticLinkedList<int, uint16_t, 3> list;

	REQUIRE(list.IsEmpty());
	REQUIRE(!list.IsFull());
	REQUIRE(0 == list.Size());
}

TEST_CASE(SUITE("AddsUntilFull"))
{
	StaticLinkedList<int, uint16_t, 3> list;

	REQUIRE(list.Add(1));
	REQUIRE(list.Add(2));
	REQUIRE(list.Add(3));

	REQUIRE(list.IsFull());

	// adding to a full list returns a nullptr
	REQUIRE_FALSE(list.Add(4));
}

TEST_CASE(SUITE("CanRemoveHead"))
{
	DynamicArray<ListNode<int>, uint16_t> arr(3);
	LinkedListAdapter<int, uint16_t> list(arr.ToIndexable());

	auto one = list.Add(1);
	auto two = list.Add(2);
	auto three = list.Add(3);

	list.Remove(one);

	REQUIRE(2 == list.Size());

	auto four = list.Add(4);

	REQUIRE(four == one); // these pointers should be the same
}

TEST_CASE(SUITE("CanRemoveTail"))
{
	DynamicArray<ListNode<int>, uint16_t> arr(3);
	LinkedListAdapter<int, uint16_t> list(arr.ToIndexable());

	auto one = list.Add(1);
	auto two = list.Add(2);
	auto three = list.Add(3);

	list.Remove(three);

	REQUIRE(2 == list.Size());

	auto four = list.Add(4);

	REQUIRE(four == three); // these pointers should be the same
}

TEST_CASE(SUITE("CanRemoveMiddle"))
{
	DynamicArray<ListNode<int>, uint16_t> arr(3);
	LinkedListAdapter<int, uint16_t> list(arr.ToIndexable());

	auto one = list.Add(1);
	auto two = list.Add(2);
	auto three = list.Add(3);

	list.Remove(two);

	REQUIRE(2 == list.Size());

	auto four = list.Add(4);

	REQUIRE(four == two); // these pointers should be the same
}

TEST_CASE(SUITE("CanIterateOverValues"))
{
	DynamicArray<ListNode<int>, uint16_t> arr(3);
	LinkedListAdapter<int, uint16_t> list(arr.ToIndexable());

	list.Add(1);
	list.Add(2);
	list.Add(3);

	auto iter = list.Iterate();

	for(int i = 1; i < 4; ++i)
	{
		REQUIRE(iter.HasNext());
		REQUIRE(i == iter.Next()->value);
	}

	REQUIRE(!iter.HasNext());
}

TEST_CASE(SUITE("StaticLinkedList"))
{
	StaticLinkedList<int, uint16_t, 3> list;

	REQUIRE(list.Add(1));
	REQUIRE(list.Add(2));
	REQUIRE(list.Add(3));
	REQUIRE_FALSE(list.Add(4));

	REQUIRE(list.Remove(2));
}

TEST_CASE(SUITE("Insert at front of list"))
{
	StaticLinkedList<int, uint16_t, 10> list;

	REQUIRE(list.Add(7));
	
	auto lessThan = [](int lhs, int rhs) { return lhs < rhs; };
	REQUIRE(list.Insert(4, lessThan));

	std::vector<int> items;

	list.Foreach([&](int x) { items.push_back(x); });
	REQUIRE(items.size() == 2);
	REQUIRE(items[0] == 4);
	REQUIRE(items[1] == 7);
}

TEST_CASE(SUITE("Insert in center of list"))
{
	StaticLinkedList<int, uint16_t, 10> list;

	REQUIRE(list.Add(2));
	REQUIRE(list.Add(7));

	auto lessThan = [](int lhs, int rhs) { return lhs < rhs; };
	REQUIRE(list.Insert(4, lessThan));

	std::vector<int> items;

	list.Foreach([&](int x) { items.push_back(x); });
	REQUIRE(items.size() == 3);

	REQUIRE(items[0] == 2);
	REQUIRE(items[1] == 4);
	REQUIRE(items[2] == 7);
}

TEST_CASE(SUITE("Insert at end of list"))
{
	StaticLinkedList<int, uint16_t, 10> list;

	REQUIRE(list.Add(2));
	REQUIRE(list.Add(4));

	auto lessThan = [](int lhs, int rhs) { return lhs < rhs; };
	REQUIRE(list.Insert(7, lessThan));

	std::vector<int> items;

	list.Foreach([&](int x) { items.push_back(x); });
	REQUIRE(items.size() == 3);

	REQUIRE(items[0] == 2);
	REQUIRE(items[1] == 4);
	REQUIRE(items[2] == 7);
}

