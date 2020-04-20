/*
 * Copyright 2013-2019 Automatak, LLC
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

TEST_CASE(SUITE("SecAckWrongFCB"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    ByteStr b(250, 0);
    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, false, false, 1, 1024, b.ToRSeq());
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 2);

    REQUIRE(t.PopLastWriteAsHex() == LinkHex::Ack(true, false, 1024, 1));
    REQUIRE(t.upper->receivedQueue.empty()); // data should not be passed up!
}

TEST_CASE(SUITE("BroadcastConfirmedDataWithoutResetDoesntForward"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    ByteStr b(250, 0);
    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, false, false, LinkBroadcastAddress::ShallConfirm, 1024,
              b.ToRSeq());
    t.link.OnTxReady();
    REQUIRE(t.upper->receivedQueue.empty());
}

TEST_CASE(SUITE("BroadcastConfirmedDataFlipNFCBAndDoesntRespond"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    t.OnFrame(LinkFunction::PRI_RESET_LINK_STATES, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    ByteStr b(250, 0);
    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, true, false, LinkBroadcastAddress::ShallConfirm, 1024,
              b.ToRSeq());
    t.link.OnTxReady();
    REQUIRE(t.upper->receivedQueue.size() == 1);
    REQUIRE(t.upper->receivedQueue.front() == b.ToHex());
    REQUIRE(t.link.GetStatistics().numUnexpectedFrame == 0);
    REQUIRE(t.NumTotalWrites() == 1);

    // Wrong FCB shouldn't be forwarded
    t.OnFrame(LinkFunction::PRI_CONFIRMED_USER_DATA, false, true, false, LinkBroadcastAddress::ShallConfirm, 1024,
              b.ToRSeq());
    t.link.OnTxReady();
    REQUIRE(t.upper->receivedQueue.size() == 1);
    REQUIRE(t.NumTotalWrites() == 1);
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

TEST_CASE(SUITE("ResetLinkTimerExpiration"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady(); // reset link

    REQUIRE(t.PopLastWriteAsHex() == LinkHex::ResetLinkStates(true, 1024, 1));
    REQUIRE(t.upper->GetCounters().numTxReady == 0);

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.upper->GetCounters().numTxReady == 1);
}

TEST_CASE(SUITE("ResetLinkTimerExpirationWithRetry"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.NumRetry = 1;
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();
    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0); // timeout the wait for Ack

    REQUIRE(t.upper->GetCounters().numTxReady == 0);
    REQUIRE(t.NumTotalWrites() == 2);

    REQUIRE(t.PopLastWriteAsHex() == LinkHex::ResetLinkStates(true, 1024, 1)); // check that reset links got sent again
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024); // this time Ack it
    REQUIRE(t.NumTotalWrites() == 3);
    REQUIRE(t.PopLastWriteAsHex()
            == LinkHex::ConfirmedUserData(true, true, 1024, 1,
                                          HexConversions::increment_hex(0x00, 250))); // check that the data got sent

    t.link.OnTxReady();

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0); // timeout the ACK
    REQUIRE(t.upper->GetCounters().numTxReady == 1);

    // Test retry reset
    segments.Reset();
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 4);
    t.link.OnTxReady();

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.upper->GetCounters().numTxReady == 1);
}

TEST_CASE(SUITE("ResetLinkTimerExpirationWithRetryResetState"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.NumRetry = 1;
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();
    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 2);
    t.link.OnTxReady();
    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);

    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.upper->GetCounters().numTxReady == 1);

    segments.Reset();
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 3);
    t.link.OnTxReady();

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_one()); // timeout
    REQUIRE(t.upper->GetCounters().numTxReady == 1);
    REQUIRE(t.NumTotalWrites() == 4);
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.upper->GetCounters().numTxReady == 2);

    // Test retry reset
    segments.Reset();
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 5); // Should now be waiting for an ACK with active timer
    t.link.OnTxReady();

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_one());
    REQUIRE(t.upper->GetCounters().numTxReady == 2);
}

TEST_CASE(SUITE("ConfirmedDataRetry"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.NumRetry = 1;
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 1); // Should now be waiting for an ACK with active timer

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 2);
    t.link.OnTxReady();

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0); // timeout the ConfData, check that it retransmits
    REQUIRE(t.NumTotalWrites() == 3);

    REQUIRE(t.PopLastWriteAsHex()
            == LinkHex::ConfirmedUserData(true, true, 1024, 1, HexConversions::increment_hex(0x00, 250)));
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.NumTotalWrites() == 3);
    REQUIRE(t.upper->GetCounters().numTxReady == 1);
}

TEST_CASE(SUITE("ConfirmedDataFailureResetsLink"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.NumRetry = 1;
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 1); // Should now be waiting for an ACK with active timer

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.NumTotalWrites() == 2);

    REQUIRE(t.PopLastWriteAsHex()
            == LinkHex::ConfirmedUserData(true, true, 1024, 1, HexConversions::increment_hex(0x00, 250)));
    t.link.OnTxReady();

    // Timeout original transmission
    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.NumTotalWrites() == 3);

    REQUIRE(t.PopLastWriteAsHex()
            == LinkHex::ConfirmedUserData(true, true, 1024, 1, HexConversions::increment_hex(0x00, 250)));
    t.link.OnTxReady();

    // Timeout retransmission, no more retransmission
    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.NumTotalWrites() == 3);

    // When sending something else, then reset link state
    t.link.Send(segments);
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 4); // Should now be waiting for an ACK with active timer
    REQUIRE(t.PopLastWriteAsHex() == LinkHex::ResetLinkStates(true, 1024, 1));
}

TEST_CASE(SUITE("ResetLinkRetries"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.NumRetry = 3;
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);

    for (int i = 1; i < 5; ++i)
    {
        REQUIRE(t.NumTotalWrites() == i); // sends link retry
        REQUIRE(t.PopLastWriteAsHex() == LinkHex::ResetLinkStates(true, 1024, 1));
        t.link.OnTxReady();
        t.exe->advance_time(cfg.Timeout.value);
        REQUIRE(t.exe->run_many() > 0); // timeout
    }

    REQUIRE(t.NumTotalWrites() == 4);
}

TEST_CASE(SUITE("ConfirmedDataNackDFCClear"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.NumRetry = 1;
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 1); // Should now be waiting for an ACK with active timer

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 2); // num transmitting confirmed data

    t.OnFrame(LinkFunction::SEC_NACK, false, false, false, 1, 1024); // test that we try to reset the link again
    t.link.OnTxReady();
    REQUIRE(t.NumTotalWrites() == 3);

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024); // ACK the link reset
    REQUIRE(t.NumTotalWrites() == 4);
}

TEST_CASE(SUITE("SendDataTimerExpiration"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    REQUIRE(t.NumTotalWrites() == 1);
    t.link.OnTxReady();

    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024); // ACK the reset links
    REQUIRE(t.NumTotalWrites() == 2);

    REQUIRE(t.NumTotalWrites() == 2);
    REQUIRE(t.PopLastWriteAsHex()
            == LinkHex::ConfirmedUserData(true, true, 1024, 1,
                                          HexConversions::increment_hex(0x00, 250))); // check that data was sent
    t.link.OnTxReady();

    t.exe->advance_time(cfg.Timeout.value);
    REQUIRE(t.exe->run_many() > 0); // trigger the timeout callback
    REQUIRE(t.upper->GetCounters().numTxReady == 1);
}

TEST_CASE(SUITE("SendDataSuccess"))
{
    LinkConfig cfg = LinkLayerTest::DefaultConfig();
    cfg.UseConfirms = true;

    LinkLayerTest t(cfg);
    t.link.OnLowerLayerUp();

    MockTransportSegment segments(250, HexConversions::increment_hex(0, 250), cfg.GetAddresses());
    t.link.Send(segments);
    t.link.OnTxReady();
    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    t.link.OnTxReady();
    t.OnFrame(LinkFunction::SEC_ACK, false, false, false, 1, 1024);
    REQUIRE(t.exe->run_many() > 0);
    REQUIRE(t.upper->GetCounters().numTxReady == 1);

    segments.Reset();
    t.link.Send(segments); // now we should be directly sending w/o having to reset, and the FCB should flip

    REQUIRE(t.NumTotalWrites() == 3);
    REQUIRE(t.PopLastWriteAsHex()
            == LinkHex::ConfirmedUserData(true, false, 1024, 1, HexConversions::increment_hex(0x00, 250)));
}
