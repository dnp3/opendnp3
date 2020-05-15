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
#include "utils/LinkHex.h"
#include "utils/LinkLayerTest.h"
#include "utils/MockTransportSegment.h"

#include <ser4cpp/util/HexConversions.h>

#include <catch.hpp>

#include <iostream>

using namespace opendnp3;
using namespace ser4cpp;

#define SUITE(name) "LinkLayerTestSuite - " name

// All operations should fail except for OnLowerLayerUp, just a representative
// number of them
TEST_CASE(SUITE("ClosedState"))
{
    LinkLayerTest t;
    MockTransportSegment segment(250, "00", Addresses());
    REQUIRE_FALSE(t.upper->SendDown(segment));
    REQUIRE_FALSE(t.link.OnLowerLayerDown());
    REQUIRE_FALSE(t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 2));
}

// Prove that the upper layer is notified when the lower layer comes online
TEST_CASE(SUITE("ForwardsOnLowerLayerUp"))
{
    LinkLayerTest t;

    REQUIRE_FALSE(t.upper->IsOnline());
    REQUIRE(t.link.OnLowerLayerUp());
    REQUIRE(t.upper->IsOnline());
    REQUIRE_FALSE(t.link.OnLowerLayerUp());
}

// Check that once the layer comes up, validation errors can occur
TEST_CASE(SUITE("ValidatesMasterOutstationBit"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::SEC_ACK, true, false, false, 1, 1024);
    REQUIRE(t.link.GetStatistics().numBadMasterBit == 1);
}

// Only process frames from your designated remote address
TEST_CASE(SUITE("ValidatesSourceAddress"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1023);
    REQUIRE(t.link.GetStatistics().numUnknownSource == 1);
}

// Only process frame addressed to you directly, or broadcast
TEST_CASE(SUITE("ValidatesDestinationAddress"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 2, 1024);
    REQUIRE(t.link.GetStatistics().numUnknownDestination == 1);
}

// Show that the base state of idle logs SecToPri frames as errors
TEST_CASE(SUITE("SecToPriNoContext"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.link.GetStatistics().numUnexpectedFrame == 1);
}

// Show that the base state of idle forwards unconfirmed user data
TEST_CASE(SUITE("UnconfirmedDataPassedUpFromIdleUnreset"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    ByteStr bs(250, 0);
    t.OnFrame(LinkFunction::PRI_UNCONFIRMED_USER_DATA, false, false, false, 1, 1024, bs.ToRSeq());
    REQUIRE(t.upper->receivedQueue.size() == 1);
    REQUIRE(t.upper->receivedQueue.front() == bs.ToHex());
}

// Show that the base state of idle forwards unconfirmed user data with broadcast address
TEST_CASE(SUITE("UnconfirmedBroadcastDataPassedUpFromIdleUnreset"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    ByteStr bs(250, 0);
    t.OnFrame(LinkFunction::PRI_UNCONFIRMED_USER_DATA, false, false, false, LinkBroadcastAddress::ShallConfirm, 1024,
              bs.ToRSeq());
    REQUIRE(t.upper->receivedQueue.size() == 1);
    REQUIRE(t.upper->receivedQueue.front() == bs.ToHex());
}

// Show that the base state of idle does not forward confirmed user data
TEST_CASE(SUITE("ConfirmedDataIgnoredFromIdleUnreset"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    ByteStr bs(250, 0);
    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, false, false, 1, 1024, bs.ToRSeq());
    REQUIRE(t.upper->receivedQueue.empty());
    REQUIRE(t.link.GetStatistics().numUnexpectedFrame == 1);
}

// Secondary Reset Links
TEST_CASE(SUITE("SecondaryResetLink"))
{
    LinkLayerTest t(LinkLayerTest::DefaultConfig());
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);

    REQUIRE(t.NumTotalWrites() == 1);
    REQUIRE(t.PopLastWriteAsHex() == LinkHex::Ack(true, false, 1024, 1));
}

// Secondary Reset Links with broadcast address
TEST_CASE(SUITE("BroadcastSecondaryResetLink"))
{
    LinkLayerTest t(LinkLayerTest::DefaultConfig());
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, LinkBroadcastAddress::ShallConfirm, 1024);

    REQUIRE(t.NumTotalWrites() == 0);
    REQUIRE(t.link.GetStatistics().numUnexpectedFrame == 1);
}

// When we get another reset links when we're already reset,
// ACK it and reset the link state
TEST_CASE(SUITE("SecondaryResetResetLinkStates"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 2);
    t.link.OnTxReady();

    REQUIRE(t.NumTotalWrites() == 2);
    REQUIRE(t.PopLastWriteAsHex() == LinkHex::Ack(true, false, 1024, 1));
}

TEST_CASE(SUITE("SecondaryResetConfirmedUserData"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    ByteStr bytes(250, 0);
    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, true, false, 1, 1024, bytes.ToRSeq());
    REQUIRE(t.NumTotalWrites() == 2);
    t.link.OnTxReady();

    REQUIRE(t.upper->receivedQueue.front() == bytes.ToHex());
    t.upper->receivedQueue.clear();

    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, true, false, 1, 1024,
              bytes.ToRSeq());               // send with wrong FCB
    REQUIRE(t.NumTotalWrites() == 3);        // should still get an ACK
    REQUIRE(t.upper->receivedQueue.empty()); // but no data
}

TEST_CASE(SUITE("RequestStatusOfLink"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_REQUEST_LINK_STATUS, false, false, false, 1,
              1024); // should be able to request this before the link is reset
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    REQUIRE(t.PopLastWriteAsHex() == LinkHex::LinkStatus(true, false, 1024, 1));

    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 2);
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::PRI_REQUEST_LINK_STATUS, false, false, false, 1,
              1024); // should be able to request this before the link is reset
    REQUIRE(t.NumTotalWrites() == 3);
    REQUIRE(t.PopLastWriteAsHex() == LinkHex::LinkStatus(true, false, 1024, 1));
}

TEST_CASE(SUITE("BroadcastRequestStatusOfLink"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_REQUEST_LINK_STATUS, false, false, false, LinkBroadcastAddress::ShallConfirm, 1024);
    REQUIRE(t.NumTotalWrites() == 0);
    REQUIRE(t.link.GetStatistics().numUnexpectedFrame == 1);
}

TEST_CASE(SUITE("TestLinkStates"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_TEST_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 0);

    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::PRI_TEST_LINK_STATES, false, true, false, 1, 1024);

    REQUIRE(t.NumTotalWrites() == 2);
    REQUIRE(t.PopLastWriteAsHex() == LinkHex::Ack(true, false, 1024, 1));
}

TEST_CASE(SUITE("BroadcastTestLinkStates"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();
    t.OnFrame(LinkFunction::PRI_TEST_LINK_STATES, false, false, false, LinkBroadcastAddress::ShallConfirm, 1024);
    REQUIRE(t.NumTotalWrites() == 0);
    REQUIRE(t.link.GetStatistics().numUnexpectedFrame == 1);
}

TEST_CASE(SUITE("SendUnconfirmed"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();

    MockTransportSegment segment(250, HexConversions::increment_hex(0, 250), Addresses());
    t.link.Send(segment);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.upper->GetCounters().numTxReady == 1);
    REQUIRE(t.NumTotalWrites() == 1);
}

TEST_CASE(SUITE("CloseBehavior"))
{
    LinkLayerTest t;
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), Addresses());
    t.link.Send(segments);
    t.link.OnTxReady();

    REQUIRE(t.exe->run_many() > 0);

    t.link.OnLowerLayerDown(); // take it down during the middle of a send
    REQUIRE_FALSE(t.upper->IsOnline());

    t.link.OnLowerLayerUp();
    REQUIRE(t.upper->IsOnline());
    segments.Reset();
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 2);
}
