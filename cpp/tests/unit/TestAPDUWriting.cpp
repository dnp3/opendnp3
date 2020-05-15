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
#include "utils/APDUHelpers.h"

#include <ser4cpp/serialization/LittleEndian.h>
#include <ser4cpp/util/HexConversions.h>

#include <app/APDURequest.h>
#include <app/APDUResponse.h>
#include <catch.hpp>
#include <gen/objects/Group1.h>
#include <gen/objects/Group12.h>
#include <gen/objects/Group2.h>
#include <gen/objects/Group20.h>
#include <gen/objects/Group30.h>
#include <gen/objects/Group50.h>
#include <gen/objects/Group51.h>
#include <gen/objects/Group60.h>

#include <cassert>

using namespace opendnp3;
using namespace ser4cpp;

#define SUITE(name) "APDUWritingTestSuite - " name

TEST_CASE(SUITE("AllObjectsAndRollback"))
{
    APDURequest request(APDUHelpers::Request(FunctionCode::READ));
    auto writer = request.GetWriter();
    writer.WriteHeader(Group60Var1::ID(), QualifierCode::ALL_OBJECTS);
    writer.WriteHeader(Group60Var2::ID(), QualifierCode::ALL_OBJECTS);
    writer.Mark();
    writer.WriteHeader(Group60Var3::ID(), QualifierCode::ALL_OBJECTS);
    writer.WriteHeader(Group60Var4::ID(), QualifierCode::ALL_OBJECTS);

    REQUIRE("C0 01 3C 01 06 3C 02 06 3C 03 06 3C 04 06" == HexConversions::to_hex(request.ToRSeq()));

    writer.Rollback();

    REQUIRE("C0 01 3C 01 06 3C 02 06" == HexConversions::to_hex(request.ToRSeq()));
}

TEST_CASE(SUITE("AllObjectsReturnsFalseWhenFull"))
{
    APDURequest request(APDUHelpers::Request(FunctionCode::READ, 6));
    auto writer = request.GetWriter();

    REQUIRE(writer.WriteHeader(Group60Var1::ID(), QualifierCode::ALL_OBJECTS));
    REQUIRE(!writer.WriteHeader(Group60Var1::ID(), QualifierCode::ALL_OBJECTS));

    REQUIRE("C0 01 3C 01 06" == HexConversions::to_hex(request.ToRSeq()));
}

TEST_CASE(SUITE("RangeWriteIteratorStartStop"))
{
    APDUResponse response(APDUHelpers::Response());
    auto writer = response.GetWriter();

    {
        auto iterator
            = writer.IterateOverRange<UInt8, Counter>(QualifierCode::UINT8_START_STOP, Group20Var6::Inst(), 2);
        REQUIRE(iterator.Write(Counter(9)));
        REQUIRE(iterator.Write(Counter(7)));
    }

    REQUIRE("C0 81 00 00 14 06 00 02 03 09 00 07 00" == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("RangeIterator UInt8 boundary condition"))
{
    APDUResponse response(APDUHelpers::Response());
    auto writer = response.GetWriter();

    {
        auto iterator = writer.IterateOverRange<UInt8, Binary>(QualifierCode::UINT8_START_STOP, Group1Var2::Inst(), 0);

        for (int i = 0; i < 256; ++i)
        {
            REQUIRE(iterator.Write(Binary(true)));
        }

        REQUIRE_FALSE(iterator.Write(Binary(true)));
    }

    std::string beginsWith("C0 81 00 00 01 02 00 00 FF");
    auto truncated = HexConversions::to_hex(response.ToRSeq()).substr(0, beginsWith.size());

    REQUIRE(beginsWith == truncated);
}

TEST_CASE(SUITE("CountIterator UInt8 boundary condition"))
{
    APDUResponse response(APDUHelpers::Response());
    auto writer = response.GetWriter();

    {
        auto iterator = writer.IterateOverCount<UInt8, Binary>(QualifierCode::UINT8_CNT, Group1Var2::Inst());

        for (int i = 0; i < 255; ++i)
        {
            REQUIRE(iterator.Write(Binary(true)));
        }

        REQUIRE_FALSE(iterator.Write(Binary(true)));
    }

    std::string beginsWith("C0 81 00 00 01 02 07 FF");
    auto truncated = HexConversions::to_hex(response.ToRSeq()).substr(0, beginsWith.size());

    REQUIRE(beginsWith == truncated);
}

TEST_CASE(SUITE("EmptyHeadersWhenNotEnoughSpaceForSingleValue"))
{
    APDUResponse response(APDUHelpers::Response(8));
    auto writer = response.GetWriter();

    auto iterator = writer.IterateOverRange<UInt8, Counter>(QualifierCode::UINT8_START_STOP, Group20Var6::Inst(), 2);

    REQUIRE(!iterator.IsValid());

    REQUIRE("C0 81 00 00" == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("CountWriteIteratorAllowsCountOfZero"))
{
    APDUResponse response(APDUHelpers::Response());
    auto writer = response.GetWriter();

    writer.IterateOverCount<UInt16, Analog>(QualifierCode::UINT16_CNT, Group30Var1::Inst());

    REQUIRE("C0 81 00 00 1E 01 08 00 00" == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("CountWriteIteratorFillsUpCorrectly"))
{
    APDUResponse response(APDUHelpers::Response(15));
    auto writer = response.GetWriter();

    {
        auto iter = writer.IterateOverCount<UInt8, Analog>(QualifierCode::UINT8_CNT, Group30Var2::Inst());

        REQUIRE(iter.Write(Analog(9, Flags(0xFF))));
        REQUIRE(iter.Write(Analog(7, Flags(0xFF))));
        REQUIRE(!iter.Write(Analog(7, Flags(0xFF)))); // we're full
    }

    REQUIRE("C0 81 00 00 1E 02 07 02 FF 09 00 FF 07 00" == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("PrefixWriteIteratorWithSingleCROB"))
{
    APDUResponse response(APDUHelpers::Response());
    auto writer = response.GetWriter();

    {
        auto iter = writer.IterateOverCountWithPrefix<UInt8, ControlRelayOutputBlock>(
            QualifierCode::UINT8_CNT_UINT8_INDEX, Group12Var1::Inst());
        REQUIRE(iter.IsValid());
        ControlRelayOutputBlock crob(OperationType::LATCH_ON, TripCloseCode::NUL, false, 0x1F, 0x10, 0xAA,
                                     CommandStatus::LOCAL);
        REQUIRE(iter.Write(crob, 0x21));
    }

    REQUIRE("C0 81 00 00 0C 01 17 01 21 03 1F 10 00 00 00 AA 00 00 00 07" == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("PrefixWriteIteratorCTO"))
{
    APDUResponse response(APDUHelpers::Response());
    auto writer = response.GetWriter();

    Group51Var1 cto;
    cto.time = DNPTime(0xAA);

    {
        auto iter = writer.IterateOverCountWithPrefixAndCTO<UInt16, Binary>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                            Group2Var3::Inst(), cto);
        REQUIRE(iter.IsValid());

        REQUIRE(iter.Write(Binary(true, Flags(0x01), DNPTime(0x0B, TimestampQuality::SYNCHRONIZED)), 6));
        REQUIRE(iter.Write(Binary(true, Flags(0x01), DNPTime(0x0C, TimestampQuality::SYNCHRONIZED)), 7));
    }

    REQUIRE("C0 81 00 00 33 01 07 01 AA 00 00 00 00 00 02 03 28 02 00 06 00 81 0B 00 07 00 81 0C 00"
            == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("PrefixWriteIteratorCTOSpaceForOnly1Value"))
{
    APDUResponse response(APDUHelpers::Response(26));
    auto writer = response.GetWriter();

    Group51Var1 cto;
    cto.time = DNPTime(0xAA);

    {
        auto iter = writer.IterateOverCountWithPrefixAndCTO<UInt16, Binary>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                            Group2Var3::Inst(), cto);
        REQUIRE(iter.IsValid());
        REQUIRE(iter.Write(Binary(true, Flags(0x01), DNPTime(0x0B, TimestampQuality::SYNCHRONIZED)), 6));
        REQUIRE(!iter.Write(Binary(true, Flags(0x01), DNPTime(0x0C, TimestampQuality::SYNCHRONIZED)), 7));
    }

    REQUIRE("C0 81 00 00 33 01 07 01 AA 00 00 00 00 00 02 03 28 01 00 06 00 81 0B 00"
            == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("PrefixWriteIteratorNotEnoughSpaceForAValue"))
{
    APDUResponse response(APDUHelpers::Response(23));
    auto writer = response.GetWriter();

    Group51Var1 cto;
    cto.time = DNPTime(0xAA);

    {
        auto iter = writer.IterateOverCountWithPrefixAndCTO<UInt16, Binary>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                            Group2Var3::Inst(), cto);
        REQUIRE(!iter.IsValid());
    }

    REQUIRE("C0 81 00 00" == HexConversions::to_hex(response.ToRSeq()));
}

TEST_CASE(SUITE("SingleValueWithIndexCROBLatchOn"))
{
    APDURequest request(APDUHelpers::Request(FunctionCode::SELECT));
    auto writer = request.GetWriter();

    ControlRelayOutputBlock crob(OperationType::LATCH_ON, TripCloseCode::NUL, false, 0x1F, 0x10, 0xAA,
                                 CommandStatus::LOCAL);

    REQUIRE(writer.WriteSingleIndexedValue<UInt16>(QualifierCode::UINT16_CNT, Group12Var1::Inst(), crob, 0x21));

    REQUIRE("C0 03 0C 01 08 01 00 21 00 03 1F 10 00 00 00 AA 00 00 00 07" == HexConversions::to_hex(request.ToRSeq()));
}

TEST_CASE(SUITE("SingleValueWithIndexCROBLatchOff"))
{
    APDURequest request(APDUHelpers::Request(FunctionCode::SELECT));
    auto writer = request.GetWriter();

    ControlRelayOutputBlock crob(OperationType::LATCH_OFF, TripCloseCode::NUL, false, 0x1F, 0x10, 0xAA,
                                 CommandStatus::LOCAL);

    REQUIRE(writer.WriteSingleIndexedValue<UInt16>(QualifierCode::UINT16_CNT, Group12Var1::Inst(), crob, 0x21));

    REQUIRE("C0 03 0C 01 08 01 00 21 00 04 1F 10 00 00 00 AA 00 00 00 07" == HexConversions::to_hex(request.ToRSeq()));
}

TEST_CASE(SUITE("WriteSingleValue"))
{
    APDURequest request(APDUHelpers::Request(FunctionCode::WRITE));
    auto writer = request.GetWriter();

    Group50Var1 obj;
    obj.time = DNPTime(0x1234);
    REQUIRE(writer.WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, obj));

    REQUIRE("C0 02 32 01 07 01 34 12 00 00 00 00" == HexConversions::to_hex(request.ToRSeq()));
}

TEST_CASE(SUITE("WriteIINRestart"))
{
    APDURequest request(APDUHelpers::Request(FunctionCode::WRITE));
    auto writer = request.GetWriter();

    {
        auto iter
            = writer.IterateOverSingleBitfield<UInt8>(GroupVariationID(80, 1), QualifierCode::UINT8_START_STOP, 7);
        iter.Write(true);
        iter.Write(true);
    }

    REQUIRE("C0 02 50 01 00 07 08 03" == HexConversions::to_hex(request.ToRSeq()));
}
