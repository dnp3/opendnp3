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

#include <openpal/ToHex.h>
#include <openpal/StaticBuffer.h>

#include <opendnp3/DNPErrorCodes.h>
#include <opendnp3/LogLevels.h>

#include "Exception.h"
#include "BufferHelpers.h"
#include "AppLayerTest.h"

using namespace openpal;
using namespace opendnp3;

std::string RequestToHex(FunctionCode function, bool fir, bool fin, bool con, bool uns, uint8_t seq)
{
	StaticBuffer<2> buffer;
	APDURequest request(buffer.GetWriteBuffer());
	request.SetFunction(function);
	request.SetControl(AppControlField(fir, fin, con, uns, seq));
	return toHex(request.ToReadOnly());
}

std::string ResponseToHex(FunctionCode function, bool fir, bool fin, bool con, bool uns, uint8_t seq)
{
	StaticBuffer<4> buffer;
	APDUResponse response(buffer.GetWriteBuffer());
	response.SetFunction(function);
	response.SetControl(AppControlField(fir, fin, con, uns, seq));
	response.SetIIN(IINField::Empty);
	return toHex(response.ToReadOnly());
}


#define SUITE(name) "AppLayerSuite - " name

// Test the accessible events to demonstrate do not work if the layer is offline
TEST_CASE(SUITE("InitialState"))
{
	AppLayerTest t;
	t.app.OnLowerLayerDown();
	REQUIRE(t.log.PopOneEntry(opendnp3::flags::ERR));
	t.app.OnLowerLayerDown();
	REQUIRE(t.log.PopOneEntry(opendnp3::flags::ERR));
	t.app.OnSendResult(true);
	REQUIRE(t.log.PopOneEntry(opendnp3::flags::ERR));
	t.app.OnSendResult(false);
	REQUIRE(t.log.PopOneEntry(opendnp3::flags::ERR));
	t.lower.SendUp("");
	REQUIRE(t.log.PopOneEntry(opendnp3::flags::ERR));
}

// Check that Up/Down are forwarded correctly
// and that the state gets reset correctly
TEST_CASE(SUITE("LayerUpDown"))
{
	AppLayerTest t(true); // master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	REQUIRE(t.state ==  t.user.mState);
	t.SendRequest(FunctionCode::READ, true, true, false, false);
	t.lower.ThisLayerDown(); ++t.state.NumLayerDown;
	REQUIRE(t.state ==  t.user.mState);
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	REQUIRE(t.state ==  t.user.mState);
	t.SendRequest(FunctionCode::READ, true, true, false, false);
}


// Malformed data should be getting logged
TEST_CASE(SUITE("ParsingErrorsCaptured"))
{
	AppLayerTest t; t.lower.ThisLayerUp();
	t.lower.SendUp("");
	REQUIRE(t.log.NextErrorCode() ==  ALERR_INSUFFICIENT_DATA_FOR_FRAG);
}

// Test that the correct header validation occurs before passing up Unsol datas
TEST_CASE(SUITE("UnsolErrors"))
{
	AppLayerTest t(true); // master
	t.lower.ThisLayerUp();
	t.SendUp(AppControlField(true, true, true, false, 0), FunctionCode::UNSOLICITED_RESPONSE, IINField()); // UNS = false
	REQUIRE(t.log.NextErrorCode() ==  ALERR_BAD_UNSOL_BIT);
}

// Test that unsol data is correctly confirmed, passed up,
// and that flooding is logged+ignored
TEST_CASE(SUITE("UnsolSuccess"))
{
	AppLayerTest t(true); // master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.lower.DisableAutoSendCallback();
	t.SendUp(AppControlField(true, true, true, true, 0), FunctionCode::UNSOLICITED_RESPONSE, IINField()); ++t.state.NumUnsol;
	REQUIRE(t.log.IsLogErrorFree());
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(RequestToHex(FunctionCode::CONFIRM, true, true, false, true, 0) ==  t.lower.PopWriteAsHex());

	// if frame requiring confirmation is sent before confirm send success
	// it gets ignored and logged
	t.SendUp(AppControlField(true, true, true, true, 0), FunctionCode::UNSOLICITED_RESPONSE, IINField());
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.log.NextErrorCode() ==  ALERR_UNSOL_FLOOD);

	t.app.OnSendResult(true);
}

// Test that the various send methods reject
// illegal function codes. Also check that the app layer
// is slave/master aware with respect to send
TEST_CASE(SUITE("SendBadFuncCodeSlave"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp();

	// can't send a response until at least 1 request has been received
	// to set the sequence number. Report this as a response failure
	t.SendResponse(FunctionCode::RESPONSE, true, true, false, false);
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(1 ==  t.user.mState.NumSolFailure);
	REQUIRE(t.log.PopOneEntry(opendnp3::flags::ERR));
}

TEST_CASE(SUITE("SendExtraObjectData"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	// doesn't matter what the sequence number is
	t.SendUp("C4 01 00 00 06"); ++t.state.NumRequest;
	REQUIRE(t.state ==  t.user.mState);
	t.SendResponse(FunctionCode::RESPONSE, true, true, false, false); // no confirmation

	REQUIRE(ResponseToHex(FunctionCode::RESPONSE, true, true, false, false, 4) ==  t.lower.PopWriteAsHex()); //check correct seq
}

// Test a simple send without confirm transaction
TEST_CASE(SUITE("SendResponseWithoutConfirm"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	// doesn't matter what the sequence number is
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	REQUIRE(t.state ==  t.user.mState);
	t.SendResponse(FunctionCode::RESPONSE, true, true, false, false); // no confirmation
	REQUIRE(ResponseToHex(FunctionCode::RESPONSE, true, true, false, false, 4) ==  t.lower.PopWriteAsHex()); //check correct seq
	++t.state.NumSolSendSuccess;
	REQUIRE(t.state ==  t.user.mState);

}

// Test a simple send without confirm transaction
TEST_CASE(SUITE("SendResponseFailure"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.lower.EnableAutoSendCallback(false);
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.SendResponse(FunctionCode::RESPONSE, true, true, false, false); // no confirmation
	++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

// Test a send with confirm
TEST_CASE(SUITE("SendResponseWithConfirm"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.SendResponse(FunctionCode::RESPONSE, true, false, true, false); // with confirmation, should start on timer
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  1);
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::CONFIRM); ++t.state.NumSolSendSuccess;
	REQUIRE(t.mts.NumActive() ==  0);
	REQUIRE(t.state ==  t.user.mState);
}

// Test a send with confirm
TEST_CASE(SUITE("CancelResponseWhileSending"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.lower.DisableAutoSendCallback();
	t.SendResponse(FunctionCode::RESPONSE, true, false, true, false); // with confirmation
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  0);

	// before any send success or failure comes back, cancel the operation
	t.app.CancelResponse();
	REQUIRE(t.state ==  t.user.mState);

	// now report that that the send has completed successfully, but this should cause the
	// the app layer to inform the user that the send has failed

	t.app.OnSendResult(true); ++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

// Test a send with confirm
TEST_CASE(SUITE("CancelResponseWhileAwaitingConfirm"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.SendResponse(FunctionCode::RESPONSE, true, false, true, false); // with confirmation
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  1);

	t.app.CancelResponse(); ++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  0); //the timer should get canceled
}


TEST_CASE(SUITE("NewRequestWhileAwaitingConfirm"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.SendResponse(FunctionCode::RESPONSE, true, false, true, false); // with confirmation
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  1);

	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.app.CancelResponse(); ++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  0); //the timer should get canceled
	t.SendResponse(FunctionCode::RESPONSE, true, false, true, false); // with confirmation
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.NumActive() ==  1);
}

// Test a send with confirm timeout
TEST_CASE(SUITE("SendResponseWithConfirmTimeout"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.SendResponse(FunctionCode::RESPONSE, true, false, true, false); // with confirmation
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.mts.DispatchOne()); ++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

// Test a non-fir send, this should
// increment the sequence number
TEST_CASE(SUITE("SendResponseNonFIR"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUp(AppControlField(true, true, false, false, 4), FunctionCode::READ); ++t.state.NumRequest;
	t.SendResponse(FunctionCode::RESPONSE, false, false, true, false); //non-FIR, will increment seq number

	t.SendUp(AppControlField(true, true, false, false, 2), FunctionCode::CONFIRM); //wrong seq number
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.log.NextErrorCode() ==  ALERR_UNEXPECTED_CONFIRM);
	t.SendUp(AppControlField(true, true, false, false, 5), FunctionCode::CONFIRM); ++t.state.NumSolSendSuccess; // correct seq
	REQUIRE(t.state ==  t.user.mState);
}

// Test an unsol send with confirm transaction
TEST_CASE(SUITE("SendUnsolicitedWithConfirm"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FunctionCode::UNSOLICITED_RESPONSE, true, true, true, true);
	REQUIRE(t.mts.NumActive() ==  1);
	t.SendUp(AppControlField(true, true, false, false, 0), FunctionCode::CONFIRM); // solicited confirm, should do nothing
	REQUIRE(t.log.NextErrorCode() ==  ALERR_UNEXPECTED_CONFIRM);
	REQUIRE(t.state ==  t.user.mState);
	t.SendUp(AppControlField(true, true, false, true, 0), FunctionCode::CONFIRM); ++t.state.NumUnsolSendSuccess; // unsol confirm
	REQUIRE(t.state ==  t.user.mState);
}

// Test an unsol send with confirm timeout
TEST_CASE(SUITE("SendUnsolicitedWithConfirmTimeout"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendUnsolicited(FunctionCode::UNSOLICITED_RESPONSE, true, true, true, true);
	REQUIRE(t.mts.DispatchOne()); ++t.state.NumUnsolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

// Test a single response transaction
TEST_CASE(SUITE("SendRequestSingleResponse"))
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendRequest(FunctionCode::READ, true, true, false, false);
	REQUIRE(t.mts.NumActive() ==  1); //check that we're waiting for a response
	t.SendUp(AppControlField(true, true, false, false, 0), FunctionCode::RESPONSE, IINField()); ++t.state.NumFinalRsp; // final response
	REQUIRE(t.mts.NumActive() ==  0);
	REQUIRE(t.state ==  t.user.mState);
}

// Test a response timeout
TEST_CASE(SUITE("SendRequestResponseTimeout"))
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;
	t.SendRequest(FunctionCode::READ, true, true, false, false);
	REQUIRE(t.mts.DispatchOne()); ++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

// Test a multi-fragmented response transaction
TEST_CASE(SUITE("SendRequestMultiResponse"))
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FunctionCode::READ, true, true, false, false);
	REQUIRE(t.mts.NumActive() ==  1); //check that we're waiting for a response

	t.SendUp(AppControlField(false, true, false, false, 0), FunctionCode::RESPONSE, IINField()); // check that bad FIR is rejected
	REQUIRE(t.log.NextErrorCode() ==  ALERR_BAD_FIR_FIN);
	REQUIRE(t.mts.NumActive() ==  1);
	REQUIRE(t.state ==  t.user.mState);

	t.SendUp(AppControlField(true, false, false, false, 0), FunctionCode::RESPONSE, IINField()); ++t.state.NumPartialRsp; // partial response
	REQUIRE(t.mts.NumActive() ==  1);
	REQUIRE(t.state ==  t.user.mState);

	t.SendUp(AppControlField(false, false, false, false, 0), FunctionCode::RESPONSE, IINField()); // check that a bad sequence number is rejected
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(t.log.NextErrorCode() ==  ALERR_BAD_SEQUENCE);

	t.SendUp(AppControlField(false, false, false, false, 1), FunctionCode::RESPONSE, IINField()); ++t.state.NumPartialRsp;
	REQUIRE(t.mts.NumActive() ==  1);
	REQUIRE(t.state ==  t.user.mState);

	t.SendUp(AppControlField(true, true, false, false, 2), FunctionCode::RESPONSE, IINField()); // check that a bad FIR bit is rejected
	REQUIRE(t.log.NextErrorCode() ==  ALERR_BAD_FIR_FIN);
	REQUIRE(t.mts.NumActive() ==  1);
	REQUIRE(t.state ==  t.user.mState);

	t.SendUp(AppControlField(false, true, false, false, 2), FunctionCode::RESPONSE, IINField()); ++t.state.NumFinalRsp;
	REQUIRE(t.mts.NumActive() ==  0);
	REQUIRE(t.state ==  t.user.mState);
}

// The SendRequest transaction needs to gracefully confirm and handle
// incoming unsolicited data before receiving it's response
TEST_CASE(SUITE("MasterUnsolictedDuringRequest"))
{
	AppLayerTest t(true);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FunctionCode::READ, true, true, false, false); // write a request
	REQUIRE(RequestToHex(FunctionCode::READ, true, true, false, false, 0) ==  t.lower.PopWriteAsHex());

	// this should queue a confirm and pass the data up immediately
	t.SendUp(AppControlField(true, true, true, true, 5), FunctionCode::UNSOLICITED_RESPONSE, IINField()); ++t.state.NumUnsol;
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(RequestToHex(FunctionCode::CONFIRM, true, true, false, true, 5) ==  t.lower.PopWriteAsHex()); //verify and clear the buffer

	t.SendUp(AppControlField(true, true, true, false, 0), FunctionCode::RESPONSE, IINField()); ++t.state.NumFinalRsp;
	REQUIRE(t.state ==  t.user.mState);
	REQUIRE(RequestToHex(FunctionCode::CONFIRM, true, true, false, false, 0) ==  t.lower.PopWriteAsHex()); // check that the response gets confirmed
}

/** The SendUnsolicited transaction needs to gracefully pass up
	requests while doing an unsolicited transaction. It's up to the
	slave itself to buffer the last such request and process it
	after the unsol transaction is complete. */
TEST_CASE(SUITE("SlaveRequestDuringUnsolicited"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FunctionCode::UNSOLICITED_RESPONSE, true, true, true, true);

	// this should queue a confirm and pass the data up immediately
	t.SendUp(AppControlField(true, true, false, false, 0), FunctionCode::READ); ++t.state.NumRequest;
	REQUIRE(t.state ==  t.user.mState);
	t.SendUp(AppControlField(true, true, false, true, 0), FunctionCode::CONFIRM); ++t.state.NumUnsolSendSuccess; //confirm the unsolicited
	REQUIRE(t.state ==  t.user.mState);
}

/** Test that no retries are performed by default */
TEST_CASE(SUITE("TestNoRetries"))
{
	AppLayerTest t(false);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FunctionCode::UNSOLICITED_RESPONSE, true, true, true, true);
	REQUIRE(t.lower.NumWrites() ==  1);
	REQUIRE(t.mts.DispatchOne()); ++t.state.NumUnsolFailure;// timeout the unsol confirm
	REQUIRE(t.lower.NumWrites() ==  1);
	REQUIRE(t.state ==  t.user.mState);
}

/** Test that retries are performed for unsol */
TEST_CASE(SUITE("TestUnsolRetries"))
{
	const size_t RETRIES = 3;

	AppLayerTest t(false, RETRIES);	// slave
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendUnsolicited(FunctionCode::UNSOLICITED_RESPONSE, true, true, true, true);

	for(size_t i = 0; i < (RETRIES + 1); ++i)
	{
		REQUIRE(t.lower.NumWrites() ==  i + 1);
		REQUIRE(t.mts.DispatchOne());				// timeout the confirm
	}

	++t.state.NumUnsolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

/** Test that retries are performed for a solicited request */
TEST_CASE(SUITE("TestOperateRetries"))
{
	const size_t RETRIES = 3;

	AppLayerTest t(true, RETRIES);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FunctionCode::OPERATE, true, true, false, false);

	for(size_t i = 0; i < (RETRIES + 1); ++i)
	{
		REQUIRE(t.lower.NumWrites() ==  i + 1);
		REQUIRE(t.mts.DispatchOne());	 // timeout the response
	}

	++t.state.NumSolFailure;
	REQUIRE(t.state ==  t.user.mState);
}

/** Test that retries are performed for a solicited request */
TEST_CASE(SUITE("TestRetrieReentrancy"))
{
	const size_t RETRIES = 1;

	AppLayerTest t(true, RETRIES);	// master
	t.lower.ThisLayerUp(); ++t.state.NumLayerUp;

	t.SendRequest(FunctionCode::OPERATE, true, true, false, false);
	REQUIRE(t.lower.NumWrites() ==  1);
	REQUIRE(t.mts.DispatchOne()); // timeout the response
	REQUIRE(t.lower.NumWrites() ==  2);
}

TEST_CASE(SUITE("LargeFrame"))
{
	AppLayerTest t(true, 1);
	t.lower.ThisLayerUp();
	HexSequence hex("80 81 06 00 01 01 01 00 00 FF 03 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0A 02 01 00 00 FF 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 1E 05 01 00 00 13 01 01 63 81 D8 45 01 01 F7 5B C3 01 08 71 90 3E 01 00 00 70 42 01 00 00 80 3F 01 00 00 80 3F 01 B5 81 D8 45 01 00 00 80 3F 01 00 00 80 3F 01 B4 81 D8 45 01 00 00 80 3F 01 00 00 80 3F 01 9A 81 D8 45 01 37 B2 6F 43 01 72 6C 35 46 01 D8 CA 7A 44 01 31 B2 6F 43 01 70 E7 23 46 01 CB 7F 62 44 01 37 B2 6F 43 01 27 22 13 46 01 11 64 4B 44 01 37 B2 6F 43 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 C8 42 01 D0 2A 5F 42 01 7E 89 8C C2 01 D0 72 5D 40 01 CF 93 10 40 01 76 75 E5 42 01 43 B2 15 C3 01 E9 5E 65 42 01 4D BC 8A C2 01 99 1D 91 C2 01 B9 E7 FA 3D 01 49 00 7A 44 01 84 4E 86 C2 01 3D 8F E6 3D 01 48 00 7A 44 01 A6 C3 A0 C2 01 CD D7 F4 3D 01 2A 00 7A 44 01 31 B2 6F 43 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 C8 42 01 37 B2 6F 43 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 C8 42 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00 01 00 00 00 00");

	t.SendRequest(FunctionCode::READ, true, true, false, false); //get the app layer into a mode to receive a request
	t.app.OnReceive(hex.ToReadOnly());
}



