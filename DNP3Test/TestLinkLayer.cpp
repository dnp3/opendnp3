//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <APL/Exception.h>
#include <DNP3/DNPConstants.h>
#include <APLTestTools/BufferHelpers.h>

#include "LinkLayerTest.h"

using namespace apl;
using namespace apl::dnp;

BOOST_AUTO_TEST_SUITE(LinkLayerSuite)

// All operations should fail except for OnLowerLayerUp, just a representative
// number of them
BOOST_AUTO_TEST_CASE(ClosedState)
{
	LinkLayerTest t;
	BOOST_REQUIRE_THROW(t.upper.SendDown("00"), InvalidStateException);
	BOOST_REQUIRE_THROW(t.link.OnLowerLayerDown(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.link.Ack(false, false, 1, 2), InvalidStateException);
}

// Prove that the upper layer is notified when the lower layer comes online
BOOST_AUTO_TEST_CASE(ForwardsOnLowerLayerUp)
{
	LinkLayerTest t;
	BOOST_REQUIRE_FALSE(t.upper.IsLowerLayerUp());
	t.link.OnLowerLayerUp();
	BOOST_REQUIRE(t.upper.IsLowerLayerUp());
	BOOST_REQUIRE_THROW(t.link.OnLowerLayerUp(), InvalidStateException);
}

// Check that once the layer comes up, validation errors can occur
BOOST_AUTO_TEST_CASE(ValidatesMasterSlaveBit)
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	t.link.Ack(true, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_MASTER_BIT_MATCH);
}

// Only process frames from your designated remote address
BOOST_AUTO_TEST_CASE(ValidatesSourceAddress)
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	t.link.Ack(false, false, 1, 1023);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNKNOWN_SOURCE);
}

// This should actually never happen when using the LinkLayerRouter
// Only process frame addressed to you
BOOST_AUTO_TEST_CASE(ValidatesDestinationAddress)
{
	LinkLayerTest t;  t.link.OnLowerLayerUp();
	t.link.Ack(false, false, 2, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNKNOWN_DESTINATION);
}

// Show that the base state of idle logs SecToPri frames as errors
BOOST_AUTO_TEST_CASE(SecToPriNoContext)
{
	LinkLayerTest t; t.link.OnLowerLayerUp();

	BOOST_REQUIRE(t.IsLogErrorFree());
	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FRAME);


	BOOST_REQUIRE(t.IsLogErrorFree());
	t.link.Nack(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FRAME);


	BOOST_REQUIRE(t.IsLogErrorFree());
	t.link.LinkStatus(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FRAME);


	BOOST_REQUIRE(t.IsLogErrorFree());
	t.link.NotSupported(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FRAME);
}

// Show that the base state of idle forwards unconfirmed user data
BOOST_AUTO_TEST_CASE(UnconfirmedDataPassedUpFromIdleUnreset)
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	ByteStr bs(250, 0);
	t.link.UnconfirmedUserData(false, 1, 1024, bs, bs.Size());
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE(t.upper.BufferEquals(bs, bs.Size()));
}

// Show that the base state of idle forwards unconfirmed user data
BOOST_AUTO_TEST_CASE(ConfirmedDataIgnoredFromIdleUnreset)
{
	LinkLayerTest t; t.link.OnLowerLayerUp();
	ByteStr bs(250, 0);
	t.link.ConfirmedUserData(false, false, 1, 1024, bs, bs.Size());
	BOOST_REQUIRE(t.upper.IsBufferEmpty());
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FRAME);
}

// Secondary Reset Links
BOOST_AUTO_TEST_CASE(SecondaryResetLink)
{
	LinkLayerTest t(LinkLayerTest::DefaultConfig(), LEV_INTERPRET, true);
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
}

BOOST_AUTO_TEST_CASE(SecAckWrongFCB)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);

	ByteStr b(250, 0);
	t.link.ConfirmedUserData(false, false, 1, 1024, b, b.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
	BOOST_REQUIRE(t.upper.IsBufferEmpty()); //data should not be passed up!
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_WRONG_FCB_ON_RECEIVE_DATA);
}

// When we get another reset links when we're already reset,
// ACK it and reset the link state
BOOST_AUTO_TEST_CASE(SecondaryResetResetLinkStates)
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	t.link.ResetLinkStates(false, 1, 1024);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
}

BOOST_AUTO_TEST_CASE(SecondaryResetConfirmedUserData)
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.ResetLinkStates(false, 1, 1024);

	ByteStr bytes(250, 0);
	t.link.ConfirmedUserData(false, true, 1, 1024, bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	BOOST_REQUIRE(t.upper.BufferEquals(bytes, bytes.Size()));
	BOOST_REQUIRE(t.IsLogErrorFree());
	t.upper.ClearBuffer();

	t.link.ConfirmedUserData(false, true, 1, 1024, bytes, bytes.Size()); //send with wrong FCB
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3); //should still get an ACK
	BOOST_REQUIRE(t.upper.IsBufferEmpty()); //but no data
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_WRONG_FCB_ON_RECEIVE_DATA);
}

BOOST_AUTO_TEST_CASE(RequestStatusOfLink)
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.RequestLinkStatus(false, 1, 1024); //should be able to request this before the link is reset
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);
	LinkFrame f; f.FormatLinkStatus(true, false, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
	t.link.ResetLinkStates(false, 1, 1024);


	t.link.RequestLinkStatus(false, 1, 1024); //should be able to request this before the link is reset
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
}

BOOST_AUTO_TEST_CASE(TestLinkStates)
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();
	t.link.TestLinkStatus(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FRAME);

	t.link.ResetLinkStates(false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);

	t.link.TestLinkStatus(false, true, 1, 1024);
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
}

BOOST_AUTO_TEST_CASE(SendUnconfirmed)
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);

	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);
	LinkFrame f; f.FormatUnconfirmedUserData(true, 1024, 1, bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mSuccessCnt, 1);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);
}

BOOST_AUTO_TEST_CASE(CloseBehavior)
{
	LinkLayerTest t;
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE(t.upper.CountersEqual(1, 0));
	t.link.OnLowerLayerDown(); //take it down during the middle of a send
	BOOST_REQUIRE_FALSE(t.upper.IsLowerLayerUp());


	t.link.OnLowerLayerUp();
	BOOST_REQUIRE(t.upper.IsLowerLayerUp());
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);

}

BOOST_AUTO_TEST_CASE(ResetLinkTimerExpiration)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);
	LinkFrame f; f.FormatResetLinkStates(true, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
	BOOST_REQUIRE(t.upper.CountersEqual(0, 0));

	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE(t.mts.DispatchOne()); //trigger the timeout callback
	BOOST_REQUIRE(t.upper.CountersEqual(0, 1));
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_NO_RETRY);
}

BOOST_AUTO_TEST_CASE(ResetLinkTimerExpirationWithRetry)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);

	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_RETRY);
	BOOST_REQUIRE(t.upper.CountersEqual(0, 0)); //check that the send is still occuring
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	LinkFrame f; f.FormatResetLinkStates(true, 1024, 1);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f); // check that reset links got sent again

	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3);
	f.FormatConfirmedUserData(true, true, 1024, 1, bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mLastSend, f); // check that reset links got sent again

	BOOST_REQUIRE(t.mts.DispatchOne()); //timeout the ACK
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_NO_RETRY);
	BOOST_REQUIRE(t.upper.CountersEqual(0, 1));

	// Test retry reset
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 4);

	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_RETRY);
	BOOST_REQUIRE(t.upper.CountersEqual(0, 1)); //check that the send is still occuring
}
BOOST_AUTO_TEST_CASE(ResetLinkTimerExpirationWithRetryResetState)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size());
	t.link.Ack(false, false, 1, 1024);
	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE(t.upper.CountersEqual(1, 0));

	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3);

	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_RETRY);
	BOOST_REQUIRE(t.upper.CountersEqual(1, 0)); //check that the send is still occuring
	BOOST_REQUIRE_EQUAL(t.mNumSend, 4);

	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE(t.upper.CountersEqual(2, 0));

	// Test retry reset
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 5);	// Should now be waiting for an ACK with active timer

	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_RETRY);
	BOOST_REQUIRE(t.upper.CountersEqual(2, 0)); //check that the send is still occuring
}

BOOST_AUTO_TEST_CASE(ConfirmedDataRetry)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.NumRetry = 1;
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg); t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1); // Should now be waiting for an ACK with active timer

	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);

	BOOST_REQUIRE(t.mts.DispatchOne()); //timeout the ConfData, check that it retransmits
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_TIMEOUT_RETRY);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3);
	LinkFrame f; f.FormatConfirmedUserData(true, true, 1024, 1, bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);

	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3);
	BOOST_REQUIRE(t.upper.CountersEqual(1, 0));
}

BOOST_AUTO_TEST_CASE(SendDataTimerExpiration)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 1);
	t.link.Ack(false, false, 1, 1024); // ACK the reset links
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	LinkFrame f; f.FormatConfirmedUserData(true, true, 1024, 1, bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 2);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f); // check that data was sent
	BOOST_REQUIRE(t.mts.DispatchOne()); //trigger the timeout callback
	BOOST_REQUIRE(t.upper.CountersEqual(0, 1));
}

BOOST_AUTO_TEST_CASE(SendDataSuccess)
{
	LinkConfig cfg = LinkLayerTest::DefaultConfig();
	cfg.UseConfirms = true;

	LinkLayerTest t(cfg);
	t.link.OnLowerLayerUp();

	ByteStr bytes(250, 0);
	t.link.Send(bytes, bytes.Size()); // Should now be waiting for an ACK
	t.link.Ack(false, false, 1, 1024); //this

	t.link.Ack(false, false, 1, 1024);
	BOOST_REQUIRE(t.upper.CountersEqual(1, 0));

	t.link.Send(bytes, bytes.Size()); // now we should be directly sending w/o having to reset, and the FCB should flip
	LinkFrame f; f.FormatConfirmedUserData(true, false, 1024, 1, bytes, bytes.Size());
	BOOST_REQUIRE_EQUAL(t.mNumSend, 3);
	BOOST_REQUIRE_EQUAL(t.mLastSend, f);
}

BOOST_AUTO_TEST_SUITE_END()
