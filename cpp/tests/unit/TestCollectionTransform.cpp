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
#include <app/parsing/Collections.h>
#include <catch.hpp>

#include <vector>

using namespace opendnp3;

#define SUITE(name) "CollectionTransformTestSuite - " name

TEST_CASE(SUITE("Map integers to booleans"))
{
    int values[4] = {1, 2, 3, 4};
    ArrayCollection<int> collectionInt(values, 4);
    auto greaterThanTwo = [](const int& x) -> bool { return x > 2; };
    auto collectionBool = Map<int, bool>(collectionInt, greaterThanTwo);

    std::vector<bool> items;
    auto fillVector = [&items](const bool& item) { items.push_back(item); };
    collectionBool.ForeachItem(fillVector);

    REQUIRE(items.size() == 4);
    REQUIRE_FALSE(items[0]);
    REQUIRE_FALSE(items[1]);
    REQUIRE(items[2]);
    REQUIRE(items[3]);
}
