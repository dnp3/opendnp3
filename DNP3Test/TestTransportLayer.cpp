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

#include "TransportTestObject.h"
#include <DNP3/TransportConstants.h>

#include <APL/Util.h>
#include <APL/ProtocolUtil.h>
#include <APL/Exception.h>



using namespace std;
using namespace apl;
using namespace apl::dnp;



BOOST_AUTO_TEST_SUITE(AsyncTransportSuite)

// make sure an invalid state exception gets thrown
// for every event other than LowerLayerUp() since
// the layer starts in the online state
BOOST_AUTO_TEST_CASE(TestStateOffline)
{
	TransportTestObject test;

	BOOST_REQUIRE_THROW(test.upper.SendDown("00"), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.SendUp(""), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.SendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.ThisLayerDown(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(TestStateReady)
{
	TransportTestObject test(true); //makes an implicit call to 'test.lower.ThisLayerUp()'

	//check that that the transport layer is correctly forwarding up/down
	BOOST_REQUIRE(test.upper.IsLowerLayerUp());
	test.lower.ThisLayerDown();
	BOOST_REQUIRE_FALSE(test.upper.IsLowerLayerUp());
	test.lower.ThisLayerUp();
	BOOST_REQUIRE(test.upper.IsLowerLayerUp());

	//check that these actions all throw InvalidStateException
	BOOST_REQUIRE_THROW(test.lower.ThisLayerUp(), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.SendSuccess(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(TestReceiveBadArguments)
{
	TransportTestObject test(true);
	//check that the wrong aruments throw argument exceptions, and it's doesn't go to the sending state
	BOOST_REQUIRE_THROW(test.lower.SendUp(""), ArgumentException);   // length 0
	BOOST_REQUIRE_THROW(test.lower.SendUp(test.GetData("C0", 0, 250)), ArgumentException); // length 251
}

BOOST_AUTO_TEST_CASE(TestReceiveNoPayload)
{
	TransportTestObject test(true);
	//try sending a FIR/FIN packet with no payload (1 byte)
	test.lower.SendUp("C0"); // FIR/FIN
	BOOST_REQUIRE_EQUAL(test.NextErrorCode(), TLERR_NO_PAYLOAD);
}

BOOST_AUTO_TEST_CASE(TestReceiveNoFIR)
{
	TransportTestObject test(true);
	//try sending a non-FIR w/ no prior packet
	test.lower.SendUp("80 77"); // _/FIN
	BOOST_REQUIRE_EQUAL(test.NextErrorCode(), TLERR_MESSAGE_WITHOUT_FIR);
}

BOOST_AUTO_TEST_CASE(TestReceiveWrongSequence)
{
	TransportTestObject test(true);
	//send a FIR, followed by a FIN w/ the wrong sequence
	test.lower.SendUp(test.GetData("40")); // FIR/_/0
	test.lower.SendUp(test.GetData("82")); // _/FIN/2
	BOOST_REQUIRE_EQUAL(test.NextErrorCode(), TLERR_BAD_SEQUENCE);
}

BOOST_AUTO_TEST_CASE(TestReceiveNonFinLessThanMaxTpduSize)
{
	TransportTestObject test(true);
	//send a FIR, followed by a FIN w/ the wrong sequence
	test.lower.SendUp("40 0A 0B 0C"); // FIR/_/0
	BOOST_REQUIRE_EQUAL(test.NextErrorCode(), TLERR_BAD_LENGTH);
}

BOOST_AUTO_TEST_CASE(TestReceiveSinglePacket)
{
	TransportTestObject test(true);
	//now try receiving 1 a single FIR/FIN with a magic value
	test.lower.SendUp("C0 77");
	test.upper.BufferEqualsHex("77");
}

BOOST_AUTO_TEST_CASE(TestReceiveLargestPossibleAPDU)
{
	TransportTestObject test(true);

	size_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	size_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	vector<string> packets;
	string apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
	for(string s: packets) {
		test.lower.SendUp(s);
	}

	BOOST_REQUIRE(test.IsLogErrorFree());
	BOOST_REQUIRE(test.upper.BufferEqualsHex(apdu)); //check that the correct data was written
}

BOOST_AUTO_TEST_CASE(TestReceiveBufferOverflow)
{
	TransportTestObject test(true);

	size_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	size_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	//send 1 more packet than possible
	vector<string> packets;
	string apdu = test.GeneratePacketSequence(packets, num_packets + 1, last_packet_length);
	for(string s: packets) {
		test.lower.SendUp(s);
	}

	BOOST_REQUIRE(test.upper.IsBufferEmpty());
	BOOST_REQUIRE_EQUAL(test.NextErrorCode(), TLERR_BUFFER_FULL);
}

BOOST_AUTO_TEST_CASE(TestReceiveNewFir)
{
	TransportTestObject test(true);

	test.lower.SendUp(test.GetData("40"));	// FIR/_/0
	BOOST_REQUIRE(test.upper.IsBufferEmpty());

	test.lower.SendUp(test.GetData("C0"));	// FIR/FIN/0
	test.upper.BufferEqualsHex(test.GetData(""));
	BOOST_REQUIRE_EQUAL(test.NextErrorCode(), TLERR_NEW_FIR);
}

BOOST_AUTO_TEST_CASE(TestSendArguments)
{
	TransportTestObject test(true);
	BOOST_REQUIRE_THROW(test.upper.SendDown(""), ArgumentException); // 0 Length
	BOOST_REQUIRE_THROW(test.upper.SendDown(test.GetData("", 0, DEFAULT_FRAG_SIZE + 1)), ArgumentException); // Max Size + 1
}

BOOST_AUTO_TEST_CASE(TestStateSending)
{
	TransportTestObject test(true);

	test.lower.DisableAutoSendCallback();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE(test.lower.BufferEqualsHex("C0 11")); //FIR/FIN SEQ=0

	// Check that while we're sending, all other send requests are rejected
	BOOST_REQUIRE_THROW(test.upper.SendDown("00"), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.ThisLayerUp(), InvalidStateException);

	//while we are sending, we should still be able to receive data as normal
	test.lower.SendUp("C0 77");
	test.upper.BufferEqualsHex("77");

	//this should put us back in the Ready state since it was a single tpdu send
	test.lower.SendSuccess();
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 1);

	BOOST_REQUIRE_THROW(test.lower.SendSuccess(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(TestSendFailure)
{
	TransportTestObject test(true);

	test.lower.DisableAutoSendCallback();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE(test.lower.BufferEqualsHex("C0 11")); //FIR/FIN SEQ=0

	//this should put us back in the Ready state
	test.lower.SendFailure();
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 0);
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mFailureCnt, 1);

	test.lower.ClearBuffer();
	test.upper.SendDown("11");
	BOOST_REQUIRE(test.lower.BufferEqualsHex("C0 11")); // should resend with the same sequence number FIR/FIN SEQ=0
	test.lower.SendSuccess();
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 1);
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mFailureCnt, 1);
}

BOOST_AUTO_TEST_CASE(TestSendSuccess)
{
	TransportTestObject test(true);

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE(test.lower.BufferEqualsHex("C0 11")); //FIR/FIN SEQ=0
	test.lower.ClearBuffer();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE(test.lower.BufferEqualsHex("C1 11")); //FIR/FIN SEQ=1
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 2);
}

//if we're in the middle of a send and the layer goes down
BOOST_AUTO_TEST_CASE(TestClosedWhileSending)
{
	TransportTestObject test(true);
	test.upper.SendDown("11"); //get the layer into the sending state

	test.lower.ThisLayerDown(); // go to the TS_ClosedAfterSend state
	BOOST_REQUIRE_FALSE(test.upper.IsLowerLayerUp());
}

BOOST_AUTO_TEST_CASE(TestSendFullAPDU)
{
	TransportTestObject test(true);

	size_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	size_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	vector<string> packets;
	std::string apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
	test.lower.DisableAutoSendCallback();
	test.upper.SendDown(apdu);

	for(string tpdu: packets) { //verify that each packet is received correctly
		BOOST_REQUIRE(test.lower.BufferEqualsHex(tpdu));
		test.lower.ClearBuffer(); //clear the buffer, otherwise the packets accumulate
		test.lower.SendSuccess();
	}
}


BOOST_AUTO_TEST_SUITE_END()
