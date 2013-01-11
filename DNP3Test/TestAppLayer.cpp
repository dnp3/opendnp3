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

#include <APL/Exception.h>
#include <APLTestTools/TestHelpers.h>
#include <APLTestTools/BufferHelpers.h>

#include "AppLayerTest.h"

using namespace apl;
using namespace apl::dnp;

using namespace boost;

BOOST_AUTO_TEST_SUITE(AppLayerSuite)

// Test the accessible events to demonstrate do not work if the layer is offline
BOOST_AUTO_TEST_CASE(InitialState)
{
	AppLayerTest t;
	BOOST_REQUIRE_THROW(t.app.OnLowerLayerDown(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.app.OnSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.app.OnSendFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.lower.SendUp(""), InvalidStateException);
}

// Check that Up/Down are forwarded correctly
// and that the state gets reset correctly
BOOST_AUTO_TEST_CASE(LayerUpDown)
{
	AppLayerTest t(true); // master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.SendRequest(FC_READ, true, true, false, false);
	t.lower.ThisLayerDown(); ++t.state.NumLayerDown;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.SendRequest(FC_READ, true, true, false, false);
}


// Malformed data should be getting logged
BOOST_AUTO_TEST_CASE(ParsingErrorsCaptured)
{
	AppLayerTest t; t.lower.ThisLayerUp();
	t.lower.SendUp("");
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_INSUFFICIENT_DATA_FOR_FRAG);
}

// Test that the correct header validation occurs before passing up Unsol datas
BOOST_AUTO_TEST_CASE(UnsolErrors)
{
	AppLayerTest t(false); // slaves can't accept unsol responsess
	t.lower.ThisLayerUp();
	t.SendUp(FC_UNSOLICITED_RESPONSE, true, true, true, false, 0); // UNS = false
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_BAD_UNSOL_BIT);
	t.SendUp(FC_UNSOLICITED_RESPONSE, true, true, true, true, 0); // UNS = true
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), SERR_FUNC_NOT_SUPPORTED);
}

// Test that unsol data is correctly confirmed, passed up,
// and that flooding is logged+ignored
BOOST_AUTO_TEST_CASE(UnsolSuccess)
{
	AppLayerTest t(true); // master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.lower.DisableAutoSendCallback();
	t.SendUp(FC_UNSOLICITED_RESPONSE, true, true, true, true, 0); ++t.state.NumUnsol;
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE(t.CheckSentAPDU(FC_CONFIRM, true, true, false, true, 0));

	// if frame requiring confirmation is sent before confirm send success
	// it gets ignored and logged
	t.SendUp(FC_UNSOLICITED_RESPONSE, true, true, true, true, 0);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_UNSOL_FLOOD);

	t.app.OnSendSuccess(); //doesn't throw exception
}

// Test that the various send methods reject
// illegal function codes. Also check that the app layer
// is slave/master aware with respect to send
BOOST_AUTO_TEST_CASE(SendBadFuncCodeSlave)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp();

	// can't send a response until at least 1 request has been received
	// to set the sequence number
	BOOST_REQUIRE_THROW(
	    t.SendResponse(FC_RESPONSE, true, true, false, false),
	    apl::InvalidStateException);

	BOOST_REQUIRE_THROW(
	    t.SendUnsolicited(FC_RESPONSE, true, true, false, false),
	    apl::ArgumentException);

	BOOST_REQUIRE_THROW(
	    t.SendRequest(FC_WRITE, true, true, false, false), // master only
	    apl::Exception);
}

// Same test for the master
BOOST_AUTO_TEST_CASE(SendBadFuncCodeMaster)
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp();

	BOOST_REQUIRE_THROW(
	    t.SendResponse(FC_RESPONSE, true, true, false, false), // slave only
	    apl::Exception);

	BOOST_REQUIRE_THROW(
	    t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, false, false), // slave only
	    apl::Exception);

	BOOST_REQUIRE_THROW(
	    t.SendRequest(FC_RESPONSE, true, true, false, false), //bad code
	    apl::ArgumentException);

	BOOST_REQUIRE_THROW(
	    t.SendRequest(FC_WRITE, true, true, true, false), // bad CON bit
	    apl::ArgumentException);

	BOOST_REQUIRE_THROW(
	    t.SendRequest(FC_WRITE, true, true, false, true), // bad UNS bit
	    apl::ArgumentException);

	BOOST_REQUIRE_THROW(
	    t.SendRequest(FC_WRITE, false, true, false, false), // bad FIR
	    apl::ArgumentException);
}

BOOST_AUTO_TEST_CASE(SendObjectUnknownResponse)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	// doesn't matter what the sequence number is
	t.SendUp("C4 01 00 00 06"); ++t.state.NumUnknown;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.SendResponse(FC_RESPONSE, true, true, false, false); // no confirmation
	BOOST_REQUIRE(t.CheckSentAPDU(FC_RESPONSE, true, true, false, false, 4)); //check correct seq
}

// Test a simple send without confirm transaction
BOOST_AUTO_TEST_CASE(SendResponseWithoutConfirm)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	// doesn't matter what the sequence number is
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.SendResponse(FC_RESPONSE, true, true, false, false); // no confirmation
	BOOST_REQUIRE(t.CheckSentAPDU(FC_RESPONSE, true, true, false, false, 4)); //check correct seq
	++t.state.NumSolSendSuccess;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);

}

// Test a simple send without confirm transaction
BOOST_AUTO_TEST_CASE(SendResponseFailure)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.lower.EnableAutoSendCallback(false);
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.SendResponse(FC_RESPONSE, true, true, false, false); // no confirmation
	++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a send with confirm
BOOST_AUTO_TEST_CASE(SendResponseWithConfirm)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.SendResponse(FC_RESPONSE, true, false, true, false); // with confirmation, should start on timer
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	t.SendUp(FC_CONFIRM, true, true, false, false, 4); ++t.state.NumSolSendSuccess;
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a send with confirm
BOOST_AUTO_TEST_CASE(CancelResponseWhileSending)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.lower.DisableAutoSendCallback();
	t.SendResponse(FC_RESPONSE, true, false, true, false); // with confirmation
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);

	// before any send success or failure comes back, cancel the operation
	t.app.CancelResponse();
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);

	// now report that that the send has completed successfully, but this should cause the
	// the app layer to inform the user that the send has failed

	t.app.OnSendSuccess(); ++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a send with confirm
BOOST_AUTO_TEST_CASE(CancelResponseWhileAwaitingConfirm)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.SendResponse(FC_RESPONSE, true, false, true, false); // with confirmation
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);

	t.app.CancelResponse(); ++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0); //the timer should get canceled
}


BOOST_AUTO_TEST_CASE(NewRequestWhileAwaitingConfirm)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.SendResponse(FC_RESPONSE, true, false, true, false); // with confirmation
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);

	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.app.CancelResponse(); ++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0); //the timer should get canceled
	t.SendResponse(FC_RESPONSE, true, false, true, false); // with confirmation
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
}

// Test a send with confirm timeout
BOOST_AUTO_TEST_CASE(SendResponseWithConfirmTimeout)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.SendResponse(FC_RESPONSE, true, false, true, false); // with confirmation
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE(t.mts.DispatchOne()); ++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a non-fir send, this should
// increment the sequence number
BOOST_AUTO_TEST_CASE(SendResponseNonFIR)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(FC_READ, true, true, false, false, 4); ++t.state.NumRequest;
	t.SendResponse(FC_RESPONSE, false, false, true, false); //non-FIR, will increment seq number

	t.SendUp(FC_CONFIRM, true, true, false, false, 2); //wrong seq number
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_UNEXPECTED_CONFIRM);
	t.SendUp(FC_CONFIRM, true, true, false, false, 5); ++t.state.NumSolSendSuccess; // correct seq
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test the formatting exceptions for unsol send
BOOST_AUTO_TEST_CASE(SendUnsolBadFormatting)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp();

	BOOST_REQUIRE_THROW(
	    t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, false, true, true), //bad FIN
	    apl::ArgumentException
	);

	BOOST_REQUIRE_THROW(
	    t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, true, false), //bad UNS
	    apl::ArgumentException
	);
}

// Test an unsol send with confirm transaction
BOOST_AUTO_TEST_CASE(SendUnsolicitedWithConfirm)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, true, true);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	t.SendUp(FC_CONFIRM, true, true, false, false, 0); // solicited confirm, should do nothing
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_UNEXPECTED_CONFIRM);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.SendUp(FC_CONFIRM, true, true, false, true, 0); ++t.state.NumUnsolSendSuccess; // unsol confirm
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test an unsol send with confirm timeout
BOOST_AUTO_TEST_CASE(SendUnsolicitedWithConfirmTimeout)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, true, true);
	BOOST_REQUIRE(t.mts.DispatchOne()); ++t.state.NumUnsolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a single response transaction
BOOST_AUTO_TEST_CASE(SendRequestSingleResponse)
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendRequest(FC_READ, true, true, false, false);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1); //check that we're waiting for a response
	t.SendUp(FC_RESPONSE, true, true, false, false, 0); ++t.state.NumFinalRsp; // final response
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a response timeout
BOOST_AUTO_TEST_CASE(SendRequestResponseTimeout)
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendRequest(FC_READ, true, true, false, false);
	BOOST_REQUIRE(t.mts.DispatchOne()); ++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// Test a multi-fragmented response transaction
BOOST_AUTO_TEST_CASE(SendRequestMultiResponse)
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FC_READ, true, true, false, false);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1); //check that we're waiting for a response

	t.SendUp(FC_RESPONSE, false, true, false, false, 0); // check that bad FIR is rejected
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_BAD_FIR_FIN);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);

	t.SendUp(FC_RESPONSE, true, false, false, false, 0); ++t.state.NumPartialRsp; // partial response
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);

	t.SendUp(FC_RESPONSE, false, false, false, false, 0); // check that a bad sequence number is rejected
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_BAD_SEQUENCE);

	t.SendUp(FC_RESPONSE, false, false, false, false, 1); ++t.state.NumPartialRsp;
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);

	t.SendUp(FC_RESPONSE, true, true, false, false, 2); // check that a bad FIR bit is rejected
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), ALERR_BAD_FIR_FIN);
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);

	t.SendUp(FC_RESPONSE, false, true, false, false, 2); ++t.state.NumFinalRsp;
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

// The SendRequest transaction needs to gracefully confirm and handle
// incoming unsolicited data before receiving it's response
BOOST_AUTO_TEST_CASE(MasterUnsolictedDuringRequest)
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FC_READ, true, true, false, false); // write a request
	BOOST_REQUIRE(t.CheckSentAPDU(FC_READ, true, true, false, false, 0)); //verify and clear the buffer

	// this should queue a confirm and pass the data up immediately
	t.SendUp(FC_UNSOLICITED_RESPONSE, true, true, true, true, 5); ++t.state.NumUnsol;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE(t.CheckSentAPDU(FC_CONFIRM, true, true, false, true, 5)); //verify and clear the buffer

	t.SendUp(FC_RESPONSE, true, true, true, false, 0); ++t.state.NumFinalRsp;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	BOOST_REQUIRE(t.CheckSentAPDU(FC_CONFIRM, true, true, false, false, 0)); // check that the response gets confirmed
}

/** The SendUnsolicited transaction needs to gracefully pass up
	requests while doing an unsolicited transaction. It's up to the
	slave itself to buffer the last such request and process it
	after the unsol transaction is complete. */
BOOST_AUTO_TEST_CASE(SlaveRequestDuringUnsolicited)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, true, true);

	// this should queue a confirm and pass the data up immediately
	t.SendUp(FC_READ, true, true, false, false, 0); ++t.state.NumRequest;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
	t.SendUp(FC_CONFIRM, true, true, false, true, 0); ++t.state.NumUnsolSendSuccess; //confirm the unsolicited
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

/** Test that no retries are performed by default */
BOOST_AUTO_TEST_CASE(TestNoRetries)
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, true, true);
	BOOST_REQUIRE_EQUAL(t.lower.NumWrites(), 1);
	BOOST_REQUIRE(t.mts.DispatchOne()); ++t.state.NumUnsolFailure;// timeout the unsol confirm
	BOOST_REQUIRE_EQUAL(t.lower.NumWrites(), 1);
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

/** Test that retries are performed for unsol */
BOOST_AUTO_TEST_CASE(TestUnsolRetries)
{
	const size_t RETRIES = 3;

	AppLayerTest t(false, RETRIES);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FC_UNSOLICITED_RESPONSE, true, true, true, true);

	for(size_t i = 0; i < (RETRIES + 1); ++i) {
		BOOST_REQUIRE_EQUAL(t.lower.NumWrites(), i + 1);
		BOOST_REQUIRE(t.mts.DispatchOne());				// timeout the confirm
	}

	++t.state.NumUnsolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

/** Test that retries are performed for a solicited request */
BOOST_AUTO_TEST_CASE(TestOperateRetries)
{
	const size_t RETRIES = 3;

	AppLayerTest t(true, RETRIES);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FC_OPERATE, true, true, false, false);

	for(size_t i = 0; i < (RETRIES + 1); ++i) {
		BOOST_REQUIRE_EQUAL(t.lower.NumWrites(), i + 1);
		BOOST_REQUIRE(t.mts.DispatchOne());	 // timeout the response
	}

	++t.state.NumSolFailure;
	BOOST_REQUIRE_EQUAL(t.state, t.user.mState);
}

/** Test that retries are performed for a solicited request */
BOOST_AUTO_TEST_CASE(TestRetrieReentrancy)
{
	const size_t RETRIES = 1;

	AppLayerTest t(true, RETRIES);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FC_OPERATE, true, true, false, false);
	BOOST_REQUIRE_EQUAL(t.lower.NumWrites(), 1);
	BOOST_REQUIRE(t.mts.DispatchOne()); // timeout the response
	BOOST_REQUIRE_EQUAL(t.lower.NumWrites(), 2);
}

BOOST_AUTO_TEST_CASE(LargeFrame)
{
	AppLayerTest t(true, 1);
	t.lower.ThisLayerUp();
	HexSequence hex("80 81 06 00 01 01 01 00 00 FF 03 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0A 02 01 00 00 FF 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 1E 05 01 00 00 13 01 01 63 81 D8 45 01 01 F7 5B C3 01 08 71 90 3E 01 00 00 70 42 01 00 00 80 3F 01 00 00 80 3F 01 B5 81 D8 45 01 00 00 80 3F 01 00 00 80 3F 01 B4 81 D8 45 01 00 00 80 3F 01 00 00 80 3F 01 9A 81 D8 45 01 37 B2 6F 43 01 72 6C 35 46 01 D8 CA 7A 44 01 31 B2 6F 43 01 70 E7 23 46 01 CB 7F 62 44 01 37 B2 6F 43 01 27 22 13 46 01 11 64 4B 44 01 37 B2 6F 43 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 C8 42 01 D0 2A 5F 42 01 7E 89 8C C2 01 D0 72 5D 40 01 CF 93 10 40 01 76 75 E5 42 01 43 B2 15 C3 01 E9 5E 65 42 01 4D BC 8A C2 01 99 1D 91 C2 01 B9 E7 FA 3D 01 49 00 7A 44 01 84 4E 86 C2 01 3D 8F E6 3D 01 48 00 7A 44 01 A6 C3 A0 C2 01 CD D7 F4 3D 01 2A 00 7A 44 01 31 B2 6F 43 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 C8 42 01 37 B2 6F 43 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 C8 42 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00");

	t.SendRequest(FC_READ, true, true, false, false); //get the app layer into a mode to receive a request
	t.app.OnReceive(hex.Buffer(), hex.Size());
}

BOOST_AUTO_TEST_SUITE_END() //end suite

