/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <catch.hpp>
#include <outstation/event/List.h>

using namespace opendnp3;

#define SUITE(name) "ListAdapter - " name

TEST_CASE(SUITE("CorrectInitialState"))
{
    List<int> list(3);

    REQUIRE(list.is_empty());
    REQUIRE(!list.IsFullAndCapacityNotZero());
    REQUIRE(0 == list.length());
}

TEST_CASE(SUITE("AddsUntilFull"))
{
    List<int> list(3);

    REQUIRE(list.Add(1));
    REQUIRE(list.Add(2));
    REQUIRE(list.Add(3));

    REQUIRE(list.IsFullAndCapacityNotZero());

    // adding to a full list returns a nullptr
    REQUIRE_FALSE(list.Add(4));
}

TEST_CASE(SUITE("IsFullAndCapacityNotZero for list of capacity 0 return false"))
{
    List<int> list(0);

    REQUIRE_FALSE(list.Add(1));

    REQUIRE_FALSE(list.IsFullAndCapacityNotZero());
}

TEST_CASE(SUITE("CanRemoveHead"))
{
    List<int> list(3);

    auto one = list.Add(1);
    list.Add(2);
    list.Add(3);

    list.Remove(one);

    REQUIRE(2 == list.length());

    auto four = list.Add(4);

    REQUIRE(four == one); // these pointers should be the same
}

TEST_CASE(SUITE("CanRemoveTail"))
{
    List<int> list(3);

    list.Add(1);
    list.Add(2);
    auto three = list.Add(3);

    list.Remove(three);

    REQUIRE(2 == list.length());

    auto four = list.Add(4);

    REQUIRE(four == three); // these pointers should be the same
}

TEST_CASE(SUITE("CanRemoveMiddle"))
{
    List<int> list(3);

    list.Add(1);
    auto two = list.Add(2);
    list.Add(3);

    list.Remove(two);

    REQUIRE(2 == list.length());

    auto four = list.Add(4);

    REQUIRE(four == two); // these pointers should be the same
}

TEST_CASE(SUITE("RemoveAllComplexQuery"))
{
    List<int> list(10);

    list.Add(2);
    list.Add(3);
    list.Add(4);
    list.Add(7);
    list.Add(20);
    list.Add(8);

    auto isEven = [](int num) { return (num % 2) == 0; };
    auto count = list.RemoveAll(isEven);

    REQUIRE(count == 4);
    REQUIRE(list.length() == 2);

    std::vector<int> remaining;
    auto pushToVector = [&](int num) { remaining.push_back(num); };
    list.Foreach(pushToVector);

    REQUIRE(remaining.size() == 2);
    REQUIRE(remaining[0] == 3);
    REQUIRE(remaining[1] == 7);
}

TEST_CASE(SUITE("CanIterateOverValues"))
{
    List<int> list(3);

    list.Add(1);
    list.Add(2);
    list.Add(3);

    auto iter = list.Iterate();

    for (int i = 1; i < 4; ++i)
    {
        REQUIRE(iter.HasNext());
        REQUIRE(i == iter.Next()->value);
    }

    REQUIRE(!iter.HasNext());
}
