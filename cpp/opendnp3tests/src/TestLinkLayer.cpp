/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include <catch.hpp>

#include <opendnp3/DNPErrorCodes.h>
#include <opendnp3/link/LinkFrame.h>

#include <openpal/ToHex.h>
#include <openpal/StaticBuffer.h>

#include "Exception.h"

#include "BufferHelpers.h"
#include "LinkLayerTest.h"
#include "HexConversions.h"

#include <iostream>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "LinkLayerTestSuite - " name

// All operations should fail except for OnLowerLayerUp, just a representative
// number of them
TEST_CASE(SUITE("ClosedState"))
{
	LinkLayerTest t;
	BufferSegment segment(250, "00");
	t.upper.SendDown(segment);
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.link.OnLowerLayerDown();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.link.Ack(false, false, 1, 2);
	REQUIRE(t.log.PopOneEntry(flags::ERR));
}

// Prove that the upper layer is notified when the lower layer comes online
TEST_CASE(SUITE("ForwardsOnLowerLayerUp"))
{
	LinkLayerTest t;

	REQUIRE_FALSE(t.upper.IsOnline());
	t.link.OnLowerLayerUp();
	REQUIRE(t.upper.IsOnline());
	t.link.OnLowerLayerUp();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
}

// Check that once the layer comes up, validation errors can occur
TEST_CASE(SUITE("ValidatesMasterOutstationBit"))
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	t.link.Ack(true, false, 1, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_MASTER_BIT_MATCH);
}

// Only process frames from your designated remote address
TEST_CASE(SUITE("ValidatesSourceAddress"))
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	t.link.Ack(false, false, 1, 1023);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNKNOWN_SOURCE);
}

// This should actually never happen when using the LinkLayerRouter
// Only process frame addressed to you
TEST_CASE(SUITE("ValidatesDestinationAddress"))
{
	LinkLayerTest t;  t.link.OnLowerLayerUp();
	t.link.Ack(false, false, 2, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNKNOWN_DESTINATION);
}

// Show that the base state of idle logs SecToPri frames as errors
TEST_CASE(SUITE("SecToPriNoContext"))
{
	LinkLayerTest t; t.link.OnLowerLayerUp();

	REQUIRE(t.log.IsLogErrorFree());
	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);


	REQUIRE(t.log.IsLogErrorFree());
	t.link.Nack(false, false, 1, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);


	REQUIRE(t.log.IsLogErrorFree());
	t.link.LinkStatus(false, false, 1, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);


	REQUIRE(t.log.IsLogErrorFree());
	t.link.NotSupported(false, false, 1, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);
}

// Show that the base state of idle forwards unconfirmed user data
TEST_CASE(SUITE("UnconfirmedDataPassedUpFromIdleUnreset"))
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	ByteStr bs(250, 0);
	t.link.UnconfirmedUserData(false, 1, 1024, bs.ToReadOnly());
	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.upper.receivedQueue.front() == bs.ToHex());
}

// Show that the base state of idle forwards unconfirmed user data
TEST_CASE(SUITE("ConfirmedDataIgnoredFromIdleUnreset"))
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	ByteStr bs(250, 0);
	t.link.ConfirmedUserData(false, false, 1, 1024, bs.ToReadOnly());
	REQUIRE(t.upper.receivedQueue.empty());
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);
}

// Secondary Reset Links
TEST_CASE(SUITE("SecondaryResetLink"))
{
	LinkLayerTest t(LinkLayerTest::DefaultConfig());
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatAck(writeTo, true, false, 1024, 1, nullptr);

	REQUIRE(t.numWrites ==  1);
	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
}

TEST_CASE(SUITE("SecAckWrongFCB"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.numWrites == 1);
	t.link.OnTransmitResult(false, true);

	ByteStr b(250, 0);
	t.link.ConfirmedUserData(false, false, 1, 1024, b.ToReadOnly());
	t.link.OnTransmitResult(false, true);
	REQUIRE(t.numWrites ==  2);

	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatAck(writeTo, true, false, 1024, 1, nullptr);

	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
	REQUIRE(t.upper.receivedQueue.empty()); //data should not be passed up!
	REQUIRE(t.log.NextErrorCode() ==  DLERR_WRONG_FCB_ON_RECEIVE_DATA);
}

// When we get another reset links when we're already reset,
// ACK it and reset the link state
TEST_CASE(SUITE("SecondaryResetResetLinkStates"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.numWrites == 1);
	t.link.OnTransmitResult(false, true);

	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.numWrites == 2);
	t.link.OnTransmitResult(false, true);

	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatAck(writeTo, true, false, 1024, 1, nullptr);

	REQUIRE(t.numWrites ==  2);
	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
}

TEST_CASE(SUITE("SecondaryResetConfirmedUserData"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.numWrites == 1);
	t.link.OnTransmitResult(false, true);

	ByteStr bytes(250, 0);
	t.link.ConfirmedUserData(false, true, 1, 1024, bytes.ToReadOnly());
	REQUIRE(t.numWrites ==  2);
	t.link.OnTransmitResult(false, true);

	REQUIRE(t.upper.receivedQueue.front() == bytes.ToHex());
	REQUIRE(t.log.IsLogErrorFree());
	t.upper.receivedQueue.clear();

	t.link.ConfirmedUserData(false, true, 1, 1024, bytes.ToReadOnly()); //send with wrong FCB
	REQUIRE(t.numWrites ==  3); //should still get an ACK
	REQUIRE(t.upper.receivedQueue.empty()); //but no data
	REQUIRE(t.log.NextErrorCode() ==  DLERR_WRONG_FCB_ON_RECEIVE_DATA);
}

TEST_CASE(SUITE("RequestStatusOfLink"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.RequestLinkStatus(false, 1, 1024); //should be able to request this before the link is reset
	REQUIRE(t.numWrites ==  1);
	t.link.OnTransmitResult(false, true);

	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatLinkStatus(writeTo, true, false, 1024, 1, nullptr);

	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));

	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.numWrites == 2);
	t.link.OnTransmitResult(false, true);

	t.link.RequestLinkStatus(false, 1, 1024); //should be able to request this before the link is reset
	REQUIRE(t.numWrites ==  3);
	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
}

TEST_CASE(SUITE("TestLinkStates"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.TestLinkStatus(false, false, 1, 1024);
	REQUIRE(t.numWrites == 0);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);

	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.numWrites ==  1);
	t.link.OnTransmitResult(false, true);

	t.link.TestLinkStatus(false, true, 1, 1024);
	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatAck(writeTo, true, false, 1024, 1, nullptr);
	REQUIRE(t.numWrites ==  2);
	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
}

TEST_CASE(SUITE("SendUnconfirmed"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segment(250, bytes.ToHex());
	t.link.Send(segment);
	REQUIRE(t.numWrites ==  1);
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.lastWrite.Size() ==  292);

	REQUIRE(t.upper.GetState().successCnt ==  1);
	REQUIRE(t.numWrites ==  1);
}


TEST_CASE(SUITE("CloseBehavior"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.upper.CountersEqual(1, 0));
	t.link.OnLowerLayerDown(); //take it down during the middle of a send
	REQUIRE_FALSE(t.upper.IsOnline());


	t.link.OnLowerLayerUp();
	REQUIRE(t.upper.IsOnline());
	segments.Reset();
	t.link.Send(segments);
	REQUIRE(t.numWrites ==  2);

}

TEST_CASE(SUITE("ResetLinkTimerExpiration"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	REQUIRE(t.numWrites ==  1);
	t.link.OnTransmitResult(true, true); // reset link

	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto result = LinkFrame::FormatResetLinkStates(writeTo, true, 1024, 1, nullptr);
	REQUIRE(toHex(t.lastWrite) ==  toHex(result));
	REQUIRE(t.upper.CountersEqual(0, 0));

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne()); // trigger the timeout callback
	REQUIRE(t.upper.CountersEqual(0, 1));
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_NO_RETRY);
}

TEST_CASE(SUITE("ResetLinkTimerExpirationWithRetry"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	REQUIRE(t.numWrites == 1);
	t.link.OnTransmitResult(true, true);
	REQUIRE(t.mts.DispatchOne()); // timeout the wait for Ack

	REQUIRE(t.log.NextErrorCode() == DLERR_TIMEOUT_RETRY);
	REQUIRE(t.upper.CountersEqual(0, 0)); //check that the send is still occuring
	REQUIRE(t.numWrites == 2);
	StaticBuffer<292> buffer;
	{
		auto writeTo = buffer.GetWriteBuffer();
		auto result = LinkFrame::FormatResetLinkStates(writeTo, true, 1024, 1, nullptr);
		REQUIRE(toHex(t.lastWrite) == toHex(result)); // check that reset links got sent again
	}
	t.link.OnTransmitResult(true, true);

	t.link.Ack(false, false, 1, 1024); // this time Ack it
	REQUIRE(t.numWrites == 3);
	{
		auto writeTo = buffer.GetWriteBuffer();
		auto result = LinkFrame::FormatConfirmedUserData(writeTo, true, true, 1024, 1, bytes, bytes.Size(), nullptr);
		REQUIRE(toHex(t.lastWrite) == toHex(result)); // check that the data got sent
	}
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.mts.DispatchOne()); //timeout the ACK
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_NO_RETRY);
	REQUIRE(t.upper.CountersEqual(0, 1));

	// Test retry reset
	segments.Reset();
	t.link.Send(segments);
	REQUIRE(t.numWrites ==  4);
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.upper.CountersEqual(0, 1)); //check that the send is still occuring
}

TEST_CASE(SUITE("ResetLinkTimerExpirationWithRetryResetState"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	REQUIRE(t.numWrites == 1);
	t.link.OnTransmitResult(true, true);
	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.numWrites == 2);
	t.link.OnTransmitResult(true, true);
	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.upper.CountersEqual(1, 0));

	segments.Reset();
	t.link.Send(segments);
	REQUIRE(t.numWrites ==  3);
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne()); // timeout
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.upper.CountersEqual(1, 0)); //check that the send is still occuring
	REQUIRE(t.numWrites ==  4);
	t.link.OnTransmitResult(true, true);

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.upper.CountersEqual(2, 0));

	// Test retry reset
	segments.Reset();
	t.link.Send(segments);
	REQUIRE(t.numWrites ==  5);	// Should now be waiting for an ACK with active timer
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.upper.CountersEqual(2, 0)); //check that the send is still occuring
}


TEST_CASE(SUITE("ConfirmedDataRetry"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg); t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	t.link.OnTransmitResult(true, true);
	REQUIRE(t.numWrites ==  1); // Should now be waiting for an ACK with active timer

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.numWrites ==  2);
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.mts.DispatchOne()); //timeout the ConfData, check that it retransmits
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.numWrites ==  3);

	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatConfirmedUserData(writeTo, true, true, 1024, 1, bytes, bytes.Size(), nullptr);

	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
	t.link.OnTransmitResult(true, true);

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.numWrites ==  3);
	REQUIRE(t.upper.CountersEqual(1, 0));
}

TEST_CASE(SUITE("ResetLinkRetries"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 3;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg); t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);

	for(int i = 1; i < 5; ++i)
	{
		REQUIRE(t.numWrites ==  i); // sends link retry
		StaticBuffer<292> buffer;
		auto writeTo = buffer.GetWriteBuffer();
		auto frame = LinkFrame::FormatResetLinkStates(writeTo, true, 1024, 1, nullptr);
		REQUIRE(toHex(t.lastWrite) == toHex(frame));
		t.link.OnTransmitResult(true, true);
		REQUIRE(t.mts.DispatchOne()); //timeout
	}

	REQUIRE(t.numWrites ==  4);
}

TEST_CASE(SUITE("ConfirmedDataNackDFCClear"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg); t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	t.link.OnTransmitResult(true, true);
	REQUIRE(t.numWrites ==  1); // Should now be waiting for an ACK with active timer

	t.link.Ack(false, false, 1, 1024);
	t.link.OnTransmitResult(true, true);
	REQUIRE(t.numWrites ==  2);  // num transmitting confirmed data

	t.link.Nack(false, false, 1, 1024);  // test that we try to reset the link again
	t.link.OnTransmitResult(true, true);
	REQUIRE(t.numWrites ==  3);

	t.link.Ack(false, false, 1, 1024); // ACK the link reset
	REQUIRE(t.numWrites ==  4);
}

TEST_CASE(SUITE("SendDataTimerExpiration"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	REQUIRE(t.numWrites ==  1);
	t.link.OnTransmitResult(true, true);

	t.link.Ack(false, false, 1, 1024); // ACK the reset links
	REQUIRE(t.numWrites ==  2);
	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatConfirmedUserData(writeTo, true, true, 1024, 1, bytes, bytes.Size(), nullptr);
	REQUIRE(t.numWrites ==  2);
	REQUIRE(toHex(t.lastWrite) ==  toHex(frame)); // check that data was sent
	t.link.OnTransmitResult(true, true);

	REQUIRE(t.mts.DispatchOne()); //trigger the timeout callback
	REQUIRE(t.upper.CountersEqual(0, 1));
}


TEST_CASE(SUITE("SendDataSuccess"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	BufferSegment segments(250, bytes.ToHex());
	t.link.Send(segments);
	t.link.OnTransmitResult(true, true);
	t.link.Ack(false, false, 1, 1024);
	t.link.OnTransmitResult(true, true);
	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.upper.CountersEqual(1, 0));

	segments.Reset();
	t.link.Send(segments); // now we should be directly sending w/o having to reset, and the FCB should flip
	StaticBuffer<292> buffer;
	auto writeTo = buffer.GetWriteBuffer();
	auto frame = LinkFrame::FormatConfirmedUserData(writeTo, true, false, 1024, 1, bytes, bytes.Size(), nullptr);
	REQUIRE(t.numWrites ==  3);
	REQUIRE(toHex(t.lastWrite) ==  toHex(frame));
}

