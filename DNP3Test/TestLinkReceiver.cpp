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


#include "LinkReceiverTest.h"
#include "DNPHelpers.h"

using namespace apl;
using namespace apl::dnp;



BOOST_AUTO_TEST_SUITE(AsyncLinkReceiver)

BOOST_AUTO_TEST_CASE(InitializationState)
{
	LinkReceiverTest t;
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
}

//////////////////////////////////////////
//  CRC Failures
//////////////////////////////////////////

BOOST_AUTO_TEST_CASE(HeaderCRCError)
{
	LinkReceiverTest t;
	t.WriteData("05 64 05 C0 01 00 00 04 E9 20");
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_CRC);
}

BOOST_AUTO_TEST_CASE(BodyCRCError)
{
	LinkReceiverTest t;
	t.WriteData("05 64 14 F3 01 00 00 04 0A 3B C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06 9A 11");
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_CRC);
}

//////////////////////////////////////////
//  Illegal packets of one form or another
//////////////////////////////////////////

// Change the length to something from 5 to something in [0,4] and recalc the crc
// A valid reset link states packet would be: 05 64 05 C0 01 00 00 04 E9 21
BOOST_AUTO_TEST_CASE(BadLengthError)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 01 C0 01 00 00 04 E9 21"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_INVALID_LENGTH);
}

//Test that the presence of user data disagrees with the function code
BOOST_AUTO_TEST_CASE(UnexpectedData)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 08 C0 01 00 00 04 E9 21"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_DATA);
}

// Test that the absence of user data disagrees with the function code
// This is the first 10 bytes of an unconfirmed user data packet w/ the length set to 5
BOOST_AUTO_TEST_CASE(AbsenceOfData)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 05 73 00 04 01 00 03 FC"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_NO_DATA);
}

// Test that the parser can handle an unknown PriToSec function code
// Reset Links w/ function code changed from 0 to 6
BOOST_AUTO_TEST_CASE(UnknownFunction)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 05 C6 01 00 00 04 E9 21"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(),  DLERR_UNKNOWN_FUNC);
}

// Test that the parser can handle an unexpected FCV bit
// Reset Links w/ FCV toggled on
BOOST_AUTO_TEST_CASE(UnexpectedFCV)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 05 D0 01 00 00 04 E9 21"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FCV);
}

// Test that the parser can handle an unexpected FCB bit for SecToPri
// ACK w/ FCB toggled on
BOOST_AUTO_TEST_CASE(UnexpectedFCB)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 05 20 00 04 01 00 19 A6"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FCB);
}

// Write two bad packets back-to-back tests that this produces
// back to back errors with a single write call
BOOST_AUTO_TEST_CASE(CombinedFailures)
{
	LinkReceiverTest t;
	t.WriteData(RepairCRC("05 64 05 20 00 04 01 00 19 A6")
	            + " " + RepairCRC("05 64 05 D0 01 00 00 04 E9 21"));
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 0);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FCB);
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_UNEXPECTED_FCV);
}

//////////////////////////////////////////
// Successful packets - Sec to Pri
//////////////////////////////////////////

BOOST_AUTO_TEST_CASE(ReadACK)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatAck(true, false, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLastWithDFC(FC_SEC_ACK, true, false, 1, 2));
}

BOOST_AUTO_TEST_CASE(ReadNACK)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatNack(false, true, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLastWithDFC(FC_SEC_NACK, false, true, 1, 2));
}

BOOST_AUTO_TEST_CASE(LinkStatus)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatLinkStatus(true, true, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLastWithDFC(FC_SEC_LINK_STATUS, true, true, 1, 2));
}

BOOST_AUTO_TEST_CASE(NotSupported)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatNotSupported(true, false, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLastWithDFC(FC_SEC_NOT_SUPPORTED, true, false, 1, 2));
}

//////////////////////////////////////////
// Successful packets - Pri To Sec
//////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestLinkStates)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatTestLinkStatus(false, true, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLastWithFCB(FC_PRI_TEST_LINK_STATES, false, true, 1, 2));
}

BOOST_AUTO_TEST_CASE(ResetLinkStates)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatResetLinkStates(false, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLast(FC_PRI_RESET_LINK_STATES, false, 1, 2));
}

BOOST_AUTO_TEST_CASE(RequestLinkStatus)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatRequestLinkStatus(true, 1, 2);
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLast(FC_PRI_REQUEST_LINK_STATUS, true, 1, 2));
}

BOOST_AUTO_TEST_CASE(UnconfirmedUserData)
{
	LinkReceiverTest t;
	LinkFrame f;
	ByteStr data(250, 0); //initializes a buffer with increasing value
	f.FormatUnconfirmedUserData(true, 1, 2, data, data.Size());
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLast(FC_PRI_UNCONFIRMED_USER_DATA, true, 1, 2));
	BOOST_REQUIRE(t.mSink.BufferEquals(data, data.Size()));
}

BOOST_AUTO_TEST_CASE(ConfirmedUserData)
{
	LinkReceiverTest t;
	LinkFrame f;
	ByteStr data(250, 0); //initializes a buffer with increasing value
	f.FormatConfirmedUserData(true, true, 1, 2, data, data.Size());
	t.WriteData(f);
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLastWithFCB(FC_PRI_CONFIRMED_USER_DATA, true, true, 1, 2));
	BOOST_REQUIRE(t.mSink.BufferEquals(data, data.Size()));
}

//////////////////////////////////////////
// multi packets
//////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestTwoPackets)
{
	LinkReceiverTest t;
	// back to back reset link
	t.WriteData("05 64 05 C0 01 00 00 04 E9 21 05 64 05 C0 01 00 00 04 E9 21");
	BOOST_REQUIRE(t.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 2);
	BOOST_REQUIRE(t.mSink.CheckLast(FC_PRI_RESET_LINK_STATES, true, 1, 1024));
}

//////////////////////////////////////////
// framing errors
//////////////////////////////////////////

// Test that the parser is able to resync without discarding
BOOST_AUTO_TEST_CASE(Resync0564)
{
	LinkReceiverTest t;
	t.WriteData("05 64 05 64 05 C0 01 00 00 04 E9 21");
	BOOST_REQUIRE_EQUAL(t.NextErrorCode(), DLERR_CRC);
	BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, 1);
	BOOST_REQUIRE(t.mSink.CheckLast(FC_PRI_RESET_LINK_STATES, true, 1, 1024));
}

//////////////////////////////////////////
// many packets
//////////////////////////////////////////

// Test that the parser is able to handle many packets successively without
// doing something stupid like overflowing it's buffer
BOOST_AUTO_TEST_CASE(ManyReceives)
{
	LinkReceiverTest t;
	LinkFrame f;
	f.FormatAck(true, false, 1, 2);

	for(size_t i = 1; i < 100; ++i) {
		t.WriteData(f);
		BOOST_REQUIRE(t.IsLogErrorFree());
		BOOST_REQUIRE_EQUAL(t.mSink.mNumFrames, i);
		BOOST_REQUIRE(t.mSink.CheckLastWithDFC(FC_SEC_ACK, true, false, 1, 2));
	}
}
BOOST_AUTO_TEST_SUITE_END()
