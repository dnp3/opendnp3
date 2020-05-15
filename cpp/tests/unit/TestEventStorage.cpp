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
#include "dnp3mocks/MockEventWriteHandler.h"

#include <catch.hpp>
#include <outstation/event/EventStorage.h>

using namespace opendnp3;

#define SUITE(name) "EventStorageTestSuite - " name

TEST_CASE(SUITE("can construct"))
{
    EventStorage storage(EventBufferConfig::AllTypes(10));
}

TEST_CASE(SUITE("calls write multiple times for different variations"))
{
    EventStorage storage(EventBufferConfig::AllTypes(10));

    REQUIRE_FALSE(
        storage.Update(Event<BinarySpec>(Binary(true), 0, EventClass::EC1, EventBinaryVariation::Group2Var1)));
    REQUIRE_FALSE(
        storage.Update(Event<BinarySpec>(Binary(true), 0, EventClass::EC1, EventBinaryVariation::Group2Var2)));

    // select events by class
    REQUIRE(storage.SelectByClass(EventClass::EC1) == 2);

    REQUIRE(storage.NumSelected() == 2);

    // set up the expected order
    MockEventWriteHandler handler;
    handler.Expect(EventBinaryVariation::Group2Var1, 1);
    handler.Expect(EventBinaryVariation::Group2Var2, 1);

    REQUIRE(storage.Write(handler) == 2);

    REQUIRE(storage.NumSelected() == 0);
    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 0);

    handler.AssertEmpty();
}

TEST_CASE(SUITE("calls write one time for same variation"))
{
    EventStorage storage(EventBufferConfig::AllTypes(10));

    REQUIRE_FALSE(
        storage.Update(Event<BinarySpec>(Binary(true), 0, EventClass::EC1, EventBinaryVariation::Group2Var1)));
    REQUIRE_FALSE(
        storage.Update(Event<BinarySpec>(Binary(true), 0, EventClass::EC1, EventBinaryVariation::Group2Var1)));

    // select events by class
    REQUIRE(storage.SelectByClass(EventClass::EC1) == 2);
    REQUIRE(storage.NumSelected() == 2);
    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 2);

    // set up the expected order
    MockEventWriteHandler handler;
    handler.Expect(EventBinaryVariation::Group2Var1, 2);

    REQUIRE(storage.Write(handler) == 2);
    REQUIRE(storage.NumSelected() == 0);
    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 0);

    handler.AssertEmpty();
}

TEST_CASE(SUITE("calls write multiple times for different types"))
{
    EventStorage storage(EventBufferConfig::AllTypes(10));

    REQUIRE_FALSE(
        storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));
    REQUIRE_FALSE(
        storage.Update(Event<BinarySpec>(Binary(true), 0, EventClass::EC1, EventBinaryVariation::Group2Var1)));
    REQUIRE_FALSE(
        storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));

    // select events by class
    REQUIRE(storage.SelectByClass(EventClass::EC1) == 3);

    // set up the expected order
    MockEventWriteHandler handler;
    handler.Expect(EventAnalogVariation::Group32Var1, 1);
    handler.Expect(EventBinaryVariation::Group2Var1, 1);
    handler.Expect(EventAnalogVariation::Group32Var1, 1);

    REQUIRE(storage.Write(handler) == 3);

    handler.AssertEmpty();
}

TEST_CASE(SUITE("zero-size doesn't overflow"))
{
    EventStorage storage(EventBufferConfig::AllTypes(0));

    REQUIRE_FALSE(
        storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));

    REQUIRE_FALSE(storage.IsAnyTypeFull());
    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 0);
    REQUIRE(storage.NumSelected() == 0);
}

TEST_CASE(SUITE("overflows as expected"))
{
    EventStorage storage(EventBufferConfig::AllTypes(1));

    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 0);
    REQUIRE_FALSE(storage.IsAnyTypeFull());

    REQUIRE_FALSE(
        storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));

    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 1);
    REQUIRE(storage.IsAnyTypeFull());
    REQUIRE(storage.NumSelected() == 0);

    REQUIRE(storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));

    REQUIRE(storage.NumUnwritten(EventClass::EC1) == 1);
}

TEST_CASE(SUITE("selected events discarded on overflow"))
{
    EventStorage storage(EventBufferConfig::AllTypes(1));

    REQUIRE_FALSE(
        storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));

    REQUIRE(storage.SelectByClass(EventClass::EC1) == 1);

    REQUIRE(storage.Update(Event<AnalogSpec>(Analog(1.0), 0, EventClass::EC1, EventAnalogVariation::Group32Var1)));

    MockEventWriteHandler handler;
    REQUIRE(storage.Write(handler) == 0);
}
