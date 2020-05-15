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
#include <opendnp3/app/ControlRelayOutputBlock.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "ControlRelayOutputBlock - " name

TEST_CASE(SUITE("Get raw code from components"))
{
    ControlRelayOutputBlock crob(OperationType::PULSE_ON, TripCloseCode::TRIP, true, 10, 100, 100,
                                 CommandStatus::SUCCESS);

    REQUIRE(crob.rawCode == 0xA1);
}

TEST_CASE(SUITE("From raw code preserves the QU bit"))
{
    ControlRelayOutputBlock crob(0x91, 10, 100, 100, CommandStatus::SUCCESS);

    REQUIRE(crob.rawCode == 0x91);
}

TEST_CASE(SUITE("From raw code preserves undefined Op Type"))
{
    ControlRelayOutputBlock crob(0xAA, 10, 100, 100, CommandStatus::SUCCESS);

    REQUIRE(crob.rawCode == 0xAA);
    REQUIRE(crob.opType == OperationType::Undefined);
}

TEST_CASE(SUITE("From raw code properly parses the enums"))
{
    ControlRelayOutputBlock crob(0xA2, 10, 100, 100, CommandStatus::SUCCESS);

    REQUIRE(crob.opType == OperationType::PULSE_OFF);
    REQUIRE(crob.tcc == TripCloseCode::TRIP);
    REQUIRE(crob.clear == true);
}

TEST_CASE(SUITE("Check QU flag"))
{
    {
        ControlRelayOutputBlock crob(0x51, 10, 100, 100, CommandStatus::SUCCESS);
        REQUIRE(crob.IsQUFlagSet() == true);
    }

    {
        ControlRelayOutputBlock crob(0xA1, 10, 100, 100, CommandStatus::SUCCESS);
        REQUIRE(crob.IsQUFlagSet() == false);
    }
}
