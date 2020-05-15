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
#include <opendnp3/app/Flags.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "FlagsTestSuite - " name

TEST_CASE(SUITE("BinaryQuality"))
{
    Flags flags{};

    flags.Set(BinaryQuality::ONLINE);
    CHECK(flags.IsSet(BinaryQuality::ONLINE));

    flags.Set(BinaryQuality::RESTART);
    CHECK(flags.IsSet(BinaryQuality::RESTART));

    flags.Set(BinaryQuality::COMM_LOST);
    CHECK(flags.IsSet(BinaryQuality::COMM_LOST));

    flags.Set(BinaryQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(BinaryQuality::REMOTE_FORCED));

    flags.Set(BinaryQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(BinaryQuality::LOCAL_FORCED));

    flags.Set(BinaryQuality::CHATTER_FILTER);
    CHECK(flags.IsSet(BinaryQuality::CHATTER_FILTER));

    flags.Set(BinaryQuality::RESERVED);
    CHECK(flags.IsSet(BinaryQuality::RESERVED));

    flags.Set(BinaryQuality::STATE);
    CHECK(flags.IsSet(BinaryQuality::STATE));
}

TEST_CASE(SUITE("DoubleBitBinaryQuality"))
{
    Flags flags{};

    flags.Set(DoubleBitBinaryQuality::ONLINE);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::ONLINE));

    flags.Set(DoubleBitBinaryQuality::RESTART);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::RESTART));

    flags.Set(DoubleBitBinaryQuality::COMM_LOST);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::COMM_LOST));

    flags.Set(DoubleBitBinaryQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::REMOTE_FORCED));

    flags.Set(DoubleBitBinaryQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::LOCAL_FORCED));

    flags.Set(DoubleBitBinaryQuality::CHATTER_FILTER);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::CHATTER_FILTER));

    flags.Set(DoubleBitBinaryQuality::STATE1);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::STATE1));

    flags.Set(DoubleBitBinaryQuality::STATE2);
    CHECK(flags.IsSet(DoubleBitBinaryQuality::STATE2));

    CHECK(flags.value == 0xFF);
}

TEST_CASE(SUITE("AnalogQuality"))
{
    Flags flags{};

    flags.Set(AnalogQuality::ONLINE);
    CHECK(flags.IsSet(AnalogQuality::ONLINE));

    flags.Set(AnalogQuality::RESTART);
    CHECK(flags.IsSet(AnalogQuality::RESTART));

    flags.Set(AnalogQuality::COMM_LOST);
    CHECK(flags.IsSet(AnalogQuality::COMM_LOST));

    flags.Set(AnalogQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(AnalogQuality::REMOTE_FORCED));

    flags.Set(AnalogQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(AnalogQuality::LOCAL_FORCED));

    flags.Set(AnalogQuality::OVERRANGE);
    CHECK(flags.IsSet(AnalogQuality::OVERRANGE));

    flags.Set(AnalogQuality::REFERENCE_ERR);
    CHECK(flags.IsSet(AnalogQuality::REFERENCE_ERR));

    flags.Set(AnalogQuality::RESERVED);
    CHECK(flags.IsSet(AnalogQuality::RESERVED));

    CHECK(flags.value == 0xFF);
}

TEST_CASE(SUITE("CounterQuality"))
{
    Flags flags{};

    flags.Set(CounterQuality::ONLINE);
    CHECK(flags.IsSet(CounterQuality::ONLINE));

    flags.Set(CounterQuality::RESTART);
    CHECK(flags.IsSet(CounterQuality::RESTART));

    flags.Set(CounterQuality::COMM_LOST);
    CHECK(flags.IsSet(CounterQuality::COMM_LOST));

    flags.Set(CounterQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(CounterQuality::REMOTE_FORCED));

    flags.Set(CounterQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(CounterQuality::LOCAL_FORCED));

    flags.Set(CounterQuality::ROLLOVER);
    CHECK(flags.IsSet(CounterQuality::ROLLOVER));

    flags.Set(CounterQuality::DISCONTINUITY);
    CHECK(flags.IsSet(CounterQuality::DISCONTINUITY));

    flags.Set(CounterQuality::RESERVED);
    CHECK(flags.IsSet(CounterQuality::RESERVED));

    CHECK(flags.value == 0xFF);
}

TEST_CASE(SUITE("FrozenCounterQuality"))
{
    Flags flags{};

    flags.Set(FrozenCounterQuality::ONLINE);
    CHECK(flags.IsSet(FrozenCounterQuality::ONLINE));

    flags.Set(FrozenCounterQuality::RESTART);
    CHECK(flags.IsSet(FrozenCounterQuality::RESTART));

    flags.Set(FrozenCounterQuality::COMM_LOST);
    CHECK(flags.IsSet(FrozenCounterQuality::COMM_LOST));

    flags.Set(FrozenCounterQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(FrozenCounterQuality::REMOTE_FORCED));

    flags.Set(FrozenCounterQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(FrozenCounterQuality::LOCAL_FORCED));

    flags.Set(FrozenCounterQuality::ROLLOVER);
    CHECK(flags.IsSet(FrozenCounterQuality::ROLLOVER));

    flags.Set(FrozenCounterQuality::DISCONTINUITY);
    CHECK(flags.IsSet(FrozenCounterQuality::DISCONTINUITY));

    flags.Set(FrozenCounterQuality::RESERVED);
    CHECK(flags.IsSet(FrozenCounterQuality::RESERVED));

    CHECK(flags.value == 0xFF);
}

TEST_CASE(SUITE("BinaryOutputStatusQuality"))
{
    Flags flags{};

    flags.Set(BinaryOutputStatusQuality::ONLINE);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::ONLINE));

    flags.Set(BinaryOutputStatusQuality::RESTART);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::RESTART));

    flags.Set(BinaryOutputStatusQuality::COMM_LOST);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::COMM_LOST));

    flags.Set(BinaryOutputStatusQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::REMOTE_FORCED));

    flags.Set(BinaryOutputStatusQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::LOCAL_FORCED));

    flags.Set(BinaryOutputStatusQuality::RESERVED1);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::RESERVED1));

    flags.Set(BinaryOutputStatusQuality::RESERVED2);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::RESERVED2));

    flags.Set(BinaryOutputStatusQuality::STATE);
    CHECK(flags.IsSet(BinaryOutputStatusQuality::STATE));

    CHECK(flags.value == 0xFF);
}

TEST_CASE(SUITE("AnalogOutputStatusQuality"))
{
    Flags flags{};

    flags.Set(AnalogOutputStatusQuality::ONLINE);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::ONLINE));

    flags.Set(AnalogOutputStatusQuality::RESTART);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::RESTART));

    flags.Set(AnalogOutputStatusQuality::COMM_LOST);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::COMM_LOST));

    flags.Set(AnalogOutputStatusQuality::REMOTE_FORCED);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::REMOTE_FORCED));

    flags.Set(AnalogOutputStatusQuality::LOCAL_FORCED);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::LOCAL_FORCED));

    flags.Set(AnalogOutputStatusQuality::OVERRANGE);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::OVERRANGE));

    flags.Set(AnalogOutputStatusQuality::REFERENCE_ERR);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::REFERENCE_ERR));

    flags.Set(AnalogOutputStatusQuality::RESERVED);
    CHECK(flags.IsSet(AnalogOutputStatusQuality::RESERVED));

    CHECK(flags.value == 0xFF);
}
