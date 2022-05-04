/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#include "utils/BufferHelpers.h"
#include "utils/DNPHelpers.h"

#include <ser4cpp/container/Buffer.h>
#include <ser4cpp/util/HexConversions.h>

#include <catch.hpp>
#include <link/CRC.h>
#include <link/LinkFrame.h>
#include <link/LinkHeader.h>

using namespace opendnp3;
using namespace ser4cpp;

std::string FormatUserData(
    bool aIsMaster, bool aIsConfirmed, int aDest, int aSrc, const std::string& data, bool aFcb = false)
{
    HexSequence hs(data);
    REQUIRE(hs.Size() < 250);

    uint8_t buffer[292];
    wseq_t wrapper(buffer, 292);

    if (aIsConfirmed)
    {
        return HexConversions::to_hex(
            LinkFrame::FormatConfirmedUserData(wrapper, aIsMaster, aFcb, aDest, aSrc, hs, nullptr));
    }

    return HexConversions::to_hex(LinkFrame::FormatUnconfirmedUserData(wrapper, aIsMaster, aDest, aSrc, hs, nullptr));
}

#define SUITE(name) "DNPLinkFrame - " name

TEST_CASE(SUITE("LinkHeaderChangeFCB"))
{
    LinkHeader hdr(5, 1, 1024, true, true, true, LinkFunction::PRI_CONFIRMED_USER_DATA);

    REQUIRE(hdr.ControlByte(true, true, true, LinkFunction::PRI_CONFIRMED_USER_DATA) == hdr.GetControl());

    hdr.ChangeFCB(false);

    REQUIRE(hdr.ControlByte(true, false, true, LinkFunction::PRI_CONFIRMED_USER_DATA) == hdr.GetControl());
}

TEST_CASE(SUITE("ResetLinks"))
{
    Buffer buffer(292);

    // ResetLinkStates - Master
    auto write = buffer.as_wslice();
    auto wrapper = LinkFrame::FormatResetLinkStates(write, true, 1, 1024, nullptr);
    REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 C0 01 00 00 04 E9 21");
}

TEST_CASE(SUITE("RequestLinkStates"))
{
    Buffer buffer(292);

    // ResetLinkStates - Master
    auto write = buffer.as_wslice();
    auto wrapper = LinkFrame::FormatRequestLinkStatus(write, false, 1, 1024, nullptr);
    REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 49 01 00 00 04 D2 36");
}

TEST_CASE(SUITE("ACK"))
{
    Buffer buffer(292);

    {
        // ACK - Outstation (DFC = false)
        auto writeTo = buffer.as_wslice();
        auto wrapper = LinkFrame::FormatAck(writeTo, false, false, 1024, 1, nullptr);
        REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 00 00 04 01 00 19 A6"); // ACK - Outstation
    }

    {
        // ACK - Outstation (DFC = true)
        auto writeTo = buffer.as_wslice();
        auto wrapper = LinkFrame::FormatAck(writeTo, false, true, 1024, 1, nullptr);
        REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 10 00 04 01 00 8B 0C"); // ACK - Outstation (with DFC set)
    }

    {
        // ACK - Master (DFC = false)
        auto writeTo = buffer.as_wslice();
        auto wrapper = LinkFrame::FormatAck(writeTo, true, false, 1, 1024, nullptr);
        REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 80 01 00 00 04 53 11");
    }

    {
        // ACK - Master (DFC = true)
        auto writeTo = buffer.as_wslice();
        auto wrapper = LinkFrame::FormatAck(writeTo, true, true, 1, 1024, nullptr);
        REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 90 01 00 00 04 C1 BB");
    }
}

TEST_CASE(SUITE("ConfirmedUserData"))
{
    Buffer buffer(292);

    // Confirmed User Data - Master (FCB = true)
    auto hex = FormatUserData(true, true, 1, 1024, "C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06", true);
    REQUIRE(hex == "05 64 14 F3 01 00 00 04 0A 3B C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06 9A 12");

    // Confirmed User Data - Outstation (FCB = true)
    auto hex2 = FormatUserData(false, true, 1024, 1,
                               "C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 "
                               "02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 "
                               "01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00",
                               true);
    REQUIRE(hex2
            == "05 64 53 73 00 04 01 00 03 FC C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 05 24 01 00 01 00 01 02 "
               "01 28 01 00 02 00 01 02 01 28 B4 77 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 A5 25 02 28 01 00 "
               "01 00 01 00 00 01 01 01 00 00 03 00 2F AC 00 1E 02 01 00 00 01 00 01 00 00 01 00 00 16 ED");
}

TEST_CASE(SUITE("ResetLinkStates"))
{
    Buffer buffer(292);

    // Reset Link States - Outstation
    auto writeTo = buffer.as_wslice();
    auto wrapper = LinkFrame::FormatResetLinkStates(writeTo, false, 1024, 1, nullptr);
    REQUIRE(HexConversions::to_hex(wrapper) == "05 64 05 40 00 04 01 00 A3 96");
}

TEST_CASE(SUITE("UnconfirmedUserData"))
{
    auto hex = FormatUserData(false, false, 1024, 1,
                              "C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 "
                              "02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 "
                              "01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00");
    // take the packet from the test above, change the control byte to 44 , repair the crc
    REQUIRE(hex
            == RepairCRC(
                   "05 64 53 44 00 04 01 00 FF FF C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 FF FF 01 00 01 00 01 "
                   "02 01 28 01 00 02 00 01 02 01 28 FF FF 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 FF FF 02 28 "
                   "01 00 01 00 01 00 00 01 01 01 00 00 03 00 FF FF 00 1E 02 01 00 00 01 00 01 00 00 01 00 00 FF FF"));
}

TEST_CASE(SUITE("LinkStatus"))
{
    Buffer buffer(292);

    // LinkStatus - Master (DFC = true)
    auto writeTo = buffer.as_wslice();
    auto wrapper = LinkFrame::FormatLinkStatus(writeTo, true, true, 1, 1024, nullptr);
    // take a length 10 frame, set the control to 9B and repair the CRC
    REQUIRE(HexConversions::to_hex(wrapper) == RepairCRC("05 64 05 9B 01 00 00 04 E9 21"));
}

TEST_CASE(SUITE("NotSupported"))
{
    Buffer buffer(292);

    // Not Supported - Outstation (DFC = true)
    auto writeTo = buffer.as_wslice();
    auto wrapper = LinkFrame::FormatNotSupported(writeTo, false, true, 1, 1024, nullptr);
    REQUIRE(HexConversions::to_hex(wrapper) == RepairCRC("05 64 05 1F 01 00 00 04 28 5A"));
}
