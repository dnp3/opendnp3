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
#include <opendnp3/link/Addresses.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "LinkAddressesSuite - " name

TEST_CASE(SUITE("LinkAddressesEqualityComparison"))
{
    Addresses lr1(1, 2);
    Addresses lr2(1, 3);
    Addresses lr3(1, 3);

    REQUIRE(lr1 != lr2);
    REQUIRE(lr3 == lr2);
}

TEST_CASE(SUITE("LinkAddressesIsBroadcast"))
{
    REQUIRE(Addresses(1, 0xFFFD).IsBroadcast());
    REQUIRE(Addresses(1, 0xFFFE).IsBroadcast());
    REQUIRE(Addresses(1, 0xFFFF).IsBroadcast());
    REQUIRE(!Addresses(1, 1024).IsBroadcast());
}
