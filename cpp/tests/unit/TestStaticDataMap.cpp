/*
 * Copyright 2013-2019 Automatak, LLC
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

#include <outstation/StaticDataMap.h>
#include <opendnp3/app/MeasurementTypeSpecs.h>

using namespace std;
using namespace opendnp3;

#define SUITE(name) "StaticDataMap - " name

TEST_CASE(SUITE("update returns 'point_not_defined' for values that don't exist"))
{
    StaticDataMap<BinarySpec> map;
	REQUIRE(map.update(Binary(true), 0) == UpdateResult::point_not_defined);	
}

TEST_CASE(SUITE("can only add points that aren't yet defined"))
{
    StaticDataMap<BinarySpec> map;
    REQUIRE(map.add(Binary(), 0, BinaryConfig()));
    REQUIRE_FALSE(map.add(Binary(), 0, BinaryConfig()));
}

TEST_CASE(SUITE("can detect events on existing point"))
{
    StaticDataMap<BinarySpec> map;
    REQUIRE(map.add(Binary(), 0, BinaryConfig()));
    
	REQUIRE(map.update(Binary(true), 0) == UpdateResult::event);
    REQUIRE(map.update(Binary(true), 0) == UpdateResult::no_change);
}
