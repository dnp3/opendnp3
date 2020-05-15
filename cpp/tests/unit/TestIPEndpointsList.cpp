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

#include "channel/IPEndpointsList.h"

#include <catch.hpp>

using namespace opendnp3;

TEST_CASE("TestIPEndpointsList")
{
    const std::string ADDRESS = "127.0.0.1";
    const uint16_t PORT = 20000;

    const std::string OTHER_ADDRESS = "192.168.0.1";
    const uint16_t OTHER_PORT = 20001;

    // assert() has different behaviours on different platforms.
    /*SECTION("throws if list is empty")
    {
        REQUIRE_THROWS(IPEndpointsList({}););
    }*/

    SECTION("single endpoint")
    {
        IPEndpointsList list({IPEndpoint(ADDRESS, PORT)});

        list.GetCurrentEndpoint();
        REQUIRE(list.GetCurrentEndpoint().address == ADDRESS);
        REQUIRE(list.GetCurrentEndpoint().port == PORT);

        list.Next();

        list.GetCurrentEndpoint();
        REQUIRE(list.GetCurrentEndpoint().address == ADDRESS);
        REQUIRE(list.GetCurrentEndpoint().port == PORT);
    }

    SECTION("two endpoints")
    {
        IPEndpointsList list({IPEndpoint(ADDRESS, PORT), IPEndpoint(OTHER_ADDRESS, OTHER_PORT)});

        list.GetCurrentEndpoint();
        REQUIRE(list.GetCurrentEndpoint().address == ADDRESS);
        REQUIRE(list.GetCurrentEndpoint().port == PORT);

        list.Next();

        list.GetCurrentEndpoint();
        REQUIRE(list.GetCurrentEndpoint().address == OTHER_ADDRESS);
        REQUIRE(list.GetCurrentEndpoint().port == OTHER_PORT);

        list.Next();

        list.GetCurrentEndpoint();
        REQUIRE(list.GetCurrentEndpoint().address == ADDRESS);
        REQUIRE(list.GetCurrentEndpoint().port == PORT);

        list.Next();
        list.Reset();

        REQUIRE(list.GetCurrentEndpoint().address == ADDRESS);
        REQUIRE(list.GetCurrentEndpoint().port == PORT);
    }
}
