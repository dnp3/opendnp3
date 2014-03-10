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

#include "Exception.h"

#include "BufferHelpers.h"
#include "LinkLayerTest.h"

#include <iostream>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "LinkLayerTestSuite - " name

// All operations should fail except for OnLowerLayerUp, just a representative
// number of them
TEST_CASE(SUITE("ClosedState"))
{
	LinkLayerTest t;
	t.upper.SendDown("00");
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.link.OnLowerLayerDown();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
	t.link.Ack(false, false, 1, 2);
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
}

// Prove that the upper layer is notified when the lower layer comes online
TEST_CASE(SUITE("ForwardsOnLowerLayerUp"))
{
	LinkLayerTest t;
	REQUIRE_FALSE(t.upper.IsLowerLayerUp());
	t.link.OnLowerLayerUp();
	REQUIRE(t.upper.IsLowerLayerUp());
	t.link.OnLowerLayerUp();
	REQUIRE(t.log.PopOneEntry(LogLevel::Error));
}

// Check that once the layer comes up, validation errors can occur
TEST_CASE(SUITE("ValidatesMasterSlaveBit"))
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
	REQUIRE(t.upper.BufferEquals(bs, bs.Size()));
}

// Show that the base state of idle forwards unconfirmed user data
TEST_CASE(SUITE("ConfirmedDataIgnoredFromIdleUnreset"))
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	ByteStr bs(250, 0);
	t.link.ConfirmedUserData(false, false, 1, 1024, bs.ToReadOnly());
	REQUIRE(t.upper.IsBufferEmpty());
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);
}

// Secondary Reset Links
TEST_CASE(SUITE("SecondaryResetLink"))
{
	LinkLayerTest t(LinkLayerTest::DefaultConfig(), LogLevel::Interpret, true);
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	REQUIRE(t.mNumSend ==  1);
	REQUIRE(t.mLastSend ==  f);
}

TEST_CASE(SUITE("SecAckWrongFCB"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	REQUIRE(t.mNumSend ==  1);

	ByteStr b(250, 0);
	t.link.ConfirmedUserData(false, false, 1, 1024, b.ToReadOnly());
	REQUIRE(t.mNumSend ==  2);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	REQUIRE(t.mLastSend ==  f);
	REQUIRE(t.upper.IsBufferEmpty()); //data should not be passed up!
	REQUIRE(t.log.NextErrorCode() ==  DLERR_WRONG_FCB_ON_RECEIVE_DATA);
}

// When we get another reset links when we're already reset,
// ACK it and reset the link state
TEST_CASE(SUITE("SecondaryResetResetLinkStates"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	t.link.ResetLinkStates(false, 1, 1024);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	REQUIRE(t.mNumSend ==  2);
	REQUIRE(t.mLastSend ==  f);
}

TEST_CASE(SUITE("SecondaryResetConfirmedUserData"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	ByteStr bytes(250, 0);
	t.link.ConfirmedUserData(false, true, 1, 1024, bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  2);
	REQUIRE(t.upper.BufferEquals(bytes, bytes.Size()));
	REQUIRE(t.log.IsLogErrorFree());
	t.upper.ClearBuffer();

	t.link.ConfirmedUserData(false, true, 1, 1024, bytes.ToReadOnly()); //send with wrong FCB
	REQUIRE(t.mNumSend ==  3); //should still get an ACK
	REQUIRE(t.upper.IsBufferEmpty()); //but no data
	REQUIRE(t.log.NextErrorCode() ==  DLERR_WRONG_FCB_ON_RECEIVE_DATA);
}

TEST_CASE(SUITE("RequestStatusOfLink"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.RequestLinkStatus(false, 1, 1024); //should be able to request this before the link is reset
	REQUIRE(t.mNumSend ==  1);
	LinkFrame f; f.FormatLinkStatus(true, false, 1024, 1);
	REQUIRE(t.mLastSend ==  f);
	t.link.ResetLinkStates(false, 1, 1024);


	t.link.RequestLinkStatus(false, 1, 1024); //should be able to request this before the link is reset
	REQUIRE(t.mNumSend ==  3);
	REQUIRE(t.mLastSend ==  f);
}

TEST_CASE(SUITE("TestLinkStates"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.TestLinkStatus(false, false, 1, 1024);
	REQUIRE(t.log.NextErrorCode() ==  DLERR_UNEXPECTED_FRAME);

	t.link.ResetLinkStates(false, 1, 1024);
	REQUIRE(t.mNumSend ==  1);

	t.link.TestLinkStatus(false, true, 1, 1024);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	REQUIRE(t.mNumSend ==  2);
	REQUIRE(t.mLastSend ==  f);
}

TEST_CASE(SUITE("SendUnconfirmed"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);

	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  1);
	LinkFrame f; f.FormatUnconfirmedUserData(true, 1024, 1, bytes, bytes.Size());
	REQUIRE(t.mLastSend ==  f);
	REQUIRE(t.upper.GetState().mSuccessCnt ==  1);
	REQUIRE(t.mNumSend ==  1);
}

TEST_CASE(SUITE("CloseBehavior"))
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.upper.CountersEqual(1, 0));
	t.link.OnLowerLayerDown(); //take it down during the middle of a send
	REQUIRE_FALSE(t.upper.IsLowerLayerUp());


	t.link.OnLowerLayerUp();
	REQUIRE(t.upper.IsLowerLayerUp());
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  2);

}

TEST_CASE(SUITE("ResetLinkTimerExpiration"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  1);
	LinkFrame f; f.FormatResetLinkStates(true, 1024, 1);
	REQUIRE(t.mLastSend ==  f);
	REQUIRE(t.upper.CountersEqual(0, 0));

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne()); //trigger the timeout callback
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
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  1);

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.upper.CountersEqual(0, 0)); //check that the send is still occuring
	REQUIRE(t.mNumSend ==  2);
	LinkFrame f; f.FormatResetLinkStates(true, 1024, 1);
	REQUIRE(t.mLastSend ==  f); // check that reset links got sent again

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.mNumSend ==  3);
	f.FormatConfirmedUserData(true, true, 1024, 1, bytes, bytes.Size());
	REQUIRE(t.mLastSend ==  f); // check that reset links got sent again

	REQUIRE(t.mts.DispatchOne()); //timeout the ACK
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_NO_RETRY);
	REQUIRE(t.upper.CountersEqual(0, 1));

	// Test retry reset
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  4);

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
	t.link.Send(bytes.ToReadOnly());
	t.link.Ack(false, false, 1, 1024);
	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.upper.CountersEqual(1, 0));

	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  3);

	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.upper.CountersEqual(1, 0)); //check that the send is still occuring
	REQUIRE(t.mNumSend ==  4);

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.upper.CountersEqual(2, 0));

	// Test retry reset
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  5);	// Should now be waiting for an ACK with active timer

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
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  1); // Should now be waiting for an ACK with active timer

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.mNumSend ==  2);

	REQUIRE(t.mts.DispatchOne()); //timeout the ConfData, check that it retransmits
	REQUIRE(t.log.NextErrorCode() ==  DLERR_TIMEOUT_RETRY);
	REQUIRE(t.mNumSend ==  3);
	LinkFrame f; f.FormatConfirmedUserData(true, true, 1024, 1, bytes, bytes.Size());
	REQUIRE(t.mLastSend ==  f);

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.mNumSend ==  3);
	REQUIRE(t.upper.CountersEqual(1, 0));
}

TEST_CASE(SUITE("ResetLinkRetries"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 3;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg); t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes.ToReadOnly());
	for(int i = 1; i < 5; ++i)
	{
		REQUIRE(t.mNumSend ==  i); // sends link retry
		LinkFrame f;
		f.FormatResetLinkStates(true, 1024, 1);
		REQUIRE(f ==  t.mLastSend);
		REQUIRE(t.mts.DispatchOne()); //timeout
	}
	REQUIRE(t.mNumSend ==  4);
}

TEST_CASE(SUITE("ConfirmedDataNackDFCClear"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg); t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  1); // Should now be waiting for an ACK with active timer

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.mNumSend ==  2);  // num transmitting confirmed data

	t.link.Nack(false, false, 1, 1024);  // test that we try to reset the link again
	REQUIRE(t.mNumSend ==  3);
	REQUIRE((t.mLastSend.GetFunc() == LinkFunction::PRI_RESET_LINK_STATES));
	t.link.Ack(false, false, 1, 1024); // ACK the link reset
	REQUIRE(t.mNumSend ==  4);
	REQUIRE((t.mLastSend.GetFunc() == LinkFunction::PRI_CONFIRMED_USER_DATA));
}

TEST_CASE(SUITE("SendDataTimerExpiration"))
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes.ToReadOnly());
	REQUIRE(t.mNumSend ==  1);
	t.link.Ack(false, false, 1, 1024); // ACK the reset links
	REQUIRE(t.mNumSend ==  2);
	LinkFrame f; f.FormatConfirmedUserData(true, true, 1024, 1, bytes, bytes.Size());
	REQUIRE(t.mNumSend ==  2);
	REQUIRE(t.mLastSend ==  f); // check that data was sent
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
	t.link.Send(bytes.ToReadOnly()); // Should now be waiting for an ACK
	t.link.Ack(false, false, 1, 1024); //this

	t.link.Ack(false, false, 1, 1024);
	REQUIRE(t.upper.CountersEqual(1, 0));

	t.link.Send(bytes.ToReadOnly()); // now we should be directly sending w/o having to reset, and the FCB should flip
	LinkFrame f; f.FormatConfirmedUserData(true, false, 1024, 1, bytes, bytes.Size());
	REQUIRE(t.mNumSend ==  3);
	REQUIRE(t.mLastSend ==  f);
}


