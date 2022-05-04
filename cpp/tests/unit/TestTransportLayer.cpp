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
#include "utils/ProtocolUtil.h"
#include "utils/TransportTestObject.h"

#include <opendnp3/app/AppConstants.h>

#include "dnp3mocks/MockLogHandler.h"

#include <catch.hpp>
#include <transport/TransportConstants.h>

using namespace opendnp3;
using namespace ser4cpp;

#define SUITE(name) "TransportLayerTestSuite - " name

TEST_CASE(SUITE("RepeatSendsDoNotLogOrChangeStatistics"))
{
    MockLogHandler log;
    TransportTx tx(log.logger);
    HexSequence hs("12 34 56");
    tx.Configure(Message(Addresses(), hs.ToRSeq()));

    auto segment1 = tx.GetSegment();
    REQUIRE("C0 12 34 56" == HexConversions::to_hex(segment1));
    REQUIRE(tx.Statistics().numTransportTx == 1);

    auto segment2 = tx.GetSegment();
    REQUIRE("C0 12 34 56" == HexConversions::to_hex(segment2));
    REQUIRE(tx.Statistics().numTransportTx == 1);
}

// make sure an invalid state exception gets thrown
// for every event other than LowerLayerUp() since
// the layer starts in the online state
TEST_CASE(SUITE("StateOffline"))
{
    TransportTestObject test;

    REQUIRE_FALSE(test.upper.SendDown("00"));
    REQUIRE_FALSE(test.link.SendUp(""));
    REQUIRE_FALSE(test.transport.OnTxReady());
    REQUIRE_FALSE(test.transport.OnLowerLayerDown());
}

TEST_CASE(SUITE("StateReady"))
{
    TransportTestObject test(true); // makes an implicit call to 'test.link.ThisLayerUp()'

    // check that that the transport layer is correctly forwarding up/down
    REQUIRE(test.upper.IsOnline());
    test.transport.OnLowerLayerDown();
    REQUIRE_FALSE(test.upper.IsOnline());
    test.transport.OnLowerLayerUp();
    REQUIRE(test.upper.IsOnline());

    REQUIRE_FALSE(test.transport.OnLowerLayerUp());
    REQUIRE_FALSE(test.transport.OnTxReady());
}

TEST_CASE(SUITE("allows header-only final frame"))
{
    TransportTestObject test(true);

    test.link.SendUp("41 DE AD BE EF");
    test.link.SendUp("82");

    REQUIRE(test.upper.received.AsHex() == "DE AD BE EF");
}

TEST_CASE(SUITE("ReceiveNoFIR"))
{
    TransportTestObject test(true);
    // try sending a non-FIR w/ no prior packet
    test.link.SendUp("80 77"); // _/FIN
    REQUIRE(test.transport.GetStatistics().rx.numTransportIgnore == 1);
}

TEST_CASE(SUITE("ReceiveWrongSequence"))
{
    TransportTestObject test(true);
    // send a FIR, followed by a FIN w/ the wrong sequence
    test.link.SendUp(test.GetData("40")); // FIR/_/0
    test.link.SendUp(test.GetData("82")); // _/FIN/2
    REQUIRE(test.transport.GetStatistics().rx.numTransportIgnore == 1);
}

TEST_CASE(SUITE("PacketsCanBeOfVaryingSize"))
{
    TransportTestObject test(true);
    test.link.SendUp("40 0A 0B 0C"); // FIR/_/0
    test.link.SendUp("81 0D 0E 0F"); // _/FIN/1
    REQUIRE(test.upper.received.AsHex() == "0A 0B 0C 0D 0E 0F");
}

TEST_CASE(SUITE("ReceiveSinglePacket"))
{
    TransportTestObject test(true);
    // now try receiving 1 a single FIR/FIN with a magic value
    test.link.SendUp("C0 77");
    REQUIRE(test.upper.received.AsHex() == "77");
}

TEST_CASE(SUITE("ReceiveLargestPossibleAPDU"))
{
    TransportTestObject test(true);

    uint32_t num_packets = CalcMaxPackets(opendnp3::DEFAULT_MAX_APDU_SIZE, MAX_TPDU_PAYLOAD);
    uint32_t last_packet_length = CalcLastPacketSize(opendnp3::DEFAULT_MAX_APDU_SIZE, MAX_TPDU_PAYLOAD);

    std::vector<std::string> packets;
    const auto apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
    for (const auto& s : packets)
    {
        test.link.SendUp(s);
    }

    REQUIRE(test.upper.received.AsHex() == apdu); // check that the correct data was written
}

TEST_CASE(SUITE("ReceiveBufferOverflow"))
{
    TransportTestObject test(true, 4); // maximum ASDU size of 4

    REQUIRE(test.link.SendUp("40 11 22 33")); // FIN = 0, FIR = 1, SEQ = 0

    {
        auto stats = test.transport.GetStatistics().rx;
        REQUIRE(stats.numTransportRx == 1);
        REQUIRE(stats.numTransportBufferOverflow == 0);
    }

    REQUIRE(test.link.SendUp("81 44 55")); // FIN = 0, FIR = 1, SEQ = 1

    {
        auto stats = test.transport.GetStatistics().rx;
        REQUIRE(stats.numTransportRx == 2);
        REQUIRE(stats.numTransportBufferOverflow == 1);
    }
}

TEST_CASE(SUITE("ReceiveNewFir"))
{
    TransportTestObject test(true);

    test.link.SendUp(test.GetData("40")); // FIR/_/0
    REQUIRE(test.upper.received.IsEmpty());

    test.link.SendUp("C0 AB CD"); // FIR/FIN/0
    REQUIRE(test.upper.received.AsHex() == "AB CD");
    REQUIRE(test.transport.GetStatistics().rx.numTransportDiscard == 1);
}

TEST_CASE(SUITE("StateSending"))
{
    TransportTestObject test(true);

    // this puts the layer into the Sending state
    REQUIRE(test.upper.SendDown("11"));
    REQUIRE("C0 11" == test.link.PopWriteAsHex()); // FIR/FIN SEQ=0

    // Check that while we're sending, all other send requests are rejected
    REQUIRE_FALSE(test.upper.SendDown("00"));

    REQUIRE_FALSE(test.transport.OnLowerLayerUp());

    // while we are sending, we should still be able to receive data as normal
    test.link.SendUp("C0 77");
    REQUIRE(test.upper.received.AsHex() == "77");

    // this should put us back in the Ready state since it was a single tpdu send
    test.transport.OnTxReady();
    REQUIRE(test.upper.GetCounters().numTxReady == 1);

    REQUIRE_FALSE(test.transport.OnTxReady());
}

TEST_CASE(SUITE("SendSuccess"))
{
    TransportTestObject test(true);

    // this puts the layer into the Sending state
    test.upper.SendDown("11");
    REQUIRE("C0 11" == test.link.PopWriteAsHex()); // FIR/FIN SEQ=0
    test.transport.OnTxReady();

    // this puts the layer into the Sending state
    test.upper.SendDown("11");
    REQUIRE("C1 11" == test.link.PopWriteAsHex()); // FIR/FIN SEQ=1
    test.transport.OnTxReady();
    REQUIRE(test.upper.GetCounters().numTxReady == 2);
}

// if we're in the middle of a send and the layer goes down
TEST_CASE(SUITE("ClosedWhileSending"))
{
    TransportTestObject test(true);
    test.upper.SendDown("11"); // get the layer into the sending state

    test.transport.OnLowerLayerDown();
    REQUIRE_FALSE(test.upper.IsOnline());
}

TEST_CASE(SUITE("SendFullAPDU"))
{
    TransportTestObject test(true);

    uint32_t numPackets = CalcMaxPackets(opendnp3::DEFAULT_MAX_APDU_SIZE, MAX_TPDU_PAYLOAD);
    uint32_t lastPacketLength = CalcLastPacketSize(opendnp3::DEFAULT_MAX_APDU_SIZE, MAX_TPDU_PAYLOAD);

    std::vector<std::string> packets;
    std::string apdu = test.GeneratePacketSequence(packets, numPackets, lastPacketLength);

    test.upper.SendDown(apdu);

    // 9 total packets
    REQUIRE(numPackets == test.link.sends.size());
    REQUIRE(packets.size() == test.link.sends.size());

    for (const auto& packet : packets)
    {
        REQUIRE(packet == test.link.PopWriteAsHex());
    }

    test.transport.OnTxReady();
}
