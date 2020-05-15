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
#include <ser4cpp/container/Buffer.h>

#include <catch.hpp>
#include <link/ShiftableBuffer.h>

#include <cstring>

using namespace opendnp3;
using namespace ser4cpp;

#define SUITE(name) "ShiftableBufferSuite - " name

const static uint8_t SYNC[] = {0x05, 0x64};

TEST_CASE(SUITE("ConstructDestruct"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());
}

TEST_CASE(SUITE("InitialState"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    REQUIRE(b.NumBytesRead() == 0);
    REQUIRE(b.NumWriteBytes() == 100);
    REQUIRE(b.ReadBuffer() == b.WriteBuff());
}

TEST_CASE(SUITE("ReadingWriting"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    b.AdvanceWrite(40);
    REQUIRE(b.NumWriteBytes() == 60);
    REQUIRE(b.NumBytesRead() == 40);

    b.AdvanceWrite(60);
    REQUIRE(b.NumWriteBytes() == 0);
    REQUIRE(b.NumBytesRead() == 100);

    b.AdvanceRead(30);
    REQUIRE(b.NumWriteBytes() == 0);
    REQUIRE(b.NumBytesRead() == 70);

    b.AdvanceRead(70);
    REQUIRE(b.NumWriteBytes() == 0);
    REQUIRE(b.NumBytesRead() == 0);
}

TEST_CASE(SUITE("Shifting"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    // initialize buffer to all zeros
    for (size_t i = 0; i < b.NumWriteBytes(); ++i)
        b.WriteBuff()[i] = 0;
    b.WriteBuff()[97] = 1;
    b.WriteBuff()[98] = 2;
    b.WriteBuff()[99] = 3;

    b.AdvanceWrite(100);

    b.AdvanceRead(97);
    b.Shift();
}

TEST_CASE(SUITE("SyncNoPattern"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    for (size_t i = 0; i < b.NumWriteBytes(); ++i)
    {
        b.WriteBuff()[i] = 0;
    }

    b.AdvanceWrite(100);

    size_t skipBytes = 0;
    REQUIRE_FALSE(b.Sync(skipBytes));
    REQUIRE(b.NumBytesRead() == 1); // 1 byte left since need 2 bytes to sync
    REQUIRE(b.NumWriteBytes() == 0);
}

TEST_CASE(SUITE("SyncBeginning"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    for (size_t i = 0; i < b.NumWriteBytes(); ++i)
        b.WriteBuff()[i] = 0;

    memcpy(b.WriteBuff(), SYNC, 2);
    b.AdvanceWrite(100);

    size_t skipBytes = 0;
    REQUIRE(b.Sync(skipBytes));
    REQUIRE(b.NumBytesRead() == 100);
    REQUIRE(b.NumWriteBytes() == 0);
}

TEST_CASE(SUITE("SyncFullPattern"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    // initialize buffer to all zeros
    for (size_t i = 0; i < b.NumWriteBytes(); ++i)
        b.WriteBuff()[i] = 0;
    uint8_t pattern[] = {0x05, 0x64};
    memcpy(b.WriteBuff() + 50, pattern, 2); // copy the pattern into the buffer
    b.AdvanceWrite(100);

    size_t skipBytes = 0;
    REQUIRE(b.Sync(skipBytes));
    REQUIRE(b.NumBytesRead() == 50);
    REQUIRE(b.NumWriteBytes() == 0);
}

TEST_CASE(SUITE("SyncPartialPattern"))
{
    ser4cpp::Buffer buffer(100);
    ShiftableBuffer b(buffer.as_wslice(), buffer.length());

    // initialize buffer to all zeros
    for (size_t i = 0; i < b.NumWriteBytes(); ++i)
        b.WriteBuff()[i] = 0;

    b.WriteBuff()[97] = 0x05;
    b.AdvanceWrite(98);

    size_t skipBytes = 0;
    REQUIRE_FALSE(b.Sync(skipBytes));
    REQUIRE(b.NumBytesRead() == 1);
    REQUIRE(b.NumWriteBytes() == 2);
}
