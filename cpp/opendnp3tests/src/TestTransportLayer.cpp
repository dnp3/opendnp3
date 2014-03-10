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


#include "TransportTestObject.h"
#include "Exception.h"

#include <opendnp3/transport/TransportConstants.h>


#include <opendnp3/ProtocolUtil.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "TransportLayerTestSuite - " name

// make sure an invalid state exception gets thrown
// for every event other than LowerLayerUp() since
// the layer starts in the online state
TEST_CASE(SUITE("StateOffline"))
{
	TransportTestObject test;

	test.upper.SendDown("00");
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.lower.SendUp("");
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.lower.SendSuccess();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.lower.ThisLayerDown();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
}

TEST_CASE(SUITE("StateReady"))
{
	TransportTestObject test(true); //makes an implicit call to 'test.lower.ThisLayerUp()'

	//check that that the transport layer is correctly forwarding up/down
	REQUIRE(test.upper.IsLowerLayerUp());
	test.lower.ThisLayerDown();
	REQUIRE_FALSE(test.upper.IsLowerLayerUp());
	test.lower.ThisLayerUp();
	REQUIRE(test.upper.IsLowerLayerUp());

	// check that these actions all log errors
	test.lower.ThisLayerUp();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.lower.SendSuccess();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
}

TEST_CASE(SUITE("ReceiveBadArguments"))
{
	TransportTestObject test(true);

	//check that the wrong aruments throw argument exceptions, and it's doesn't go to the sending state
	test.lower.SendUp("");
	REQUIRE(TLERR_NO_PAYLOAD ==  test.log.NextErrorCode());
	test.lower.SendUp("FF");
	REQUIRE(TLERR_NO_PAYLOAD ==  test.log.NextErrorCode());

	test.lower.SendUp(test.GetData("C0", 0, 250)); // length 251

	REQUIRE(TLERR_TOO_MUCH_DATA ==  test.log.NextErrorCode());
}

TEST_CASE(SUITE("ReceiveNoPayload"))
{
	TransportTestObject test(true);
	//try sending a FIR/FIN packet with no payload (1 byte)
	test.lower.SendUp("C0"); // FIR/FIN
	REQUIRE(test.log.NextErrorCode() ==  TLERR_NO_PAYLOAD);
}

TEST_CASE(SUITE("ReceiveNoFIR"))
{
	TransportTestObject test(true);
	//try sending a non-FIR w/ no prior packet
	test.lower.SendUp("80 77"); // _/FIN
	REQUIRE(test.log.NextErrorCode() ==  TLERR_MESSAGE_WITHOUT_FIR);
}

TEST_CASE(SUITE("ReceiveWrongSequence"))
{
	TransportTestObject test(true);
	//send a FIR, followed by a FIN w/ the wrong sequence
	test.lower.SendUp(test.GetData("40")); // FIR/_/0
	test.lower.SendUp(test.GetData("82")); // _/FIN/2
	REQUIRE(test.log.NextErrorCode() ==  TLERR_BAD_SEQUENCE);
}

TEST_CASE(SUITE("PacketsCanBeOfVaryingSize"))
{
	TransportTestObject test(true);
	test.lower.SendUp("40 0A 0B 0C"); // FIR/_/0
	REQUIRE(test.log.IsLogErrorFree());
	test.lower.SendUp("81 0D 0E 0F"); // _/FIN/1
	REQUIRE(test.log.IsLogErrorFree());
	REQUIRE("0A 0B 0C 0D 0E 0F" ==  test.upper.GetBufferAsHexString());
}

TEST_CASE(SUITE("ReceiveSinglePacket"))
{
	TransportTestObject test(true);
	//now try receiving 1 a single FIR/FIN with a magic value
	test.lower.SendUp("C0 77");
	REQUIRE("77" ==  test.upper.GetBufferAsHexString());
}

TEST_CASE(SUITE("ReceiveLargestPossibleAPDU"))
{
	TransportTestObject test(true);

	uint32_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	uint32_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	vector<string> packets;
	string apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
	for(string s : packets)
	{
		test.lower.SendUp(s);
	}

	REQUIRE(test.log.IsLogErrorFree());
	REQUIRE(test.upper.BufferEqualsHex(apdu)); //check that the correct data was written
}

TEST_CASE(SUITE("ReceiveBufferOverflow"))
{
	TransportTestObject test(true);

	uint32_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	uint32_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	//send 1 more packet than possible
	vector<string> packets;
	string apdu = test.GeneratePacketSequence(packets, num_packets + 1, last_packet_length);
	for(string s : packets)
	{
		test.lower.SendUp(s);
	}

	REQUIRE(test.upper.IsBufferEmpty());
	REQUIRE(test.log.NextErrorCode() ==  TLERR_BUFFER_FULL);
}

TEST_CASE(SUITE("ReceiveNewFir"))
{
	TransportTestObject test(true);

	test.lower.SendUp(test.GetData("40"));	// FIR/_/0
	REQUIRE(test.upper.IsBufferEmpty());

	test.lower.SendUp("C0 AB CD");	// FIR/FIN/0
	REQUIRE("AB CD" ==  test.upper.GetBufferAsHexString());
	REQUIRE(test.log.NextErrorCode() ==  TLERR_NEW_FIR); //make sure it logs the dropped frames
}

TEST_CASE(SUITE("SendArguments"))
{
	TransportTestObject test(true);
	test.upper.SendDown("");
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
}

TEST_CASE(SUITE("StateSending"))
{
	TransportTestObject test(true);

	test.lower.DisableAutoSendCallback();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	REQUIRE("C0 11" ==  test.lower.PopWriteAsHex()); //FIR/FIN SEQ=0

	// Check that while we're sending, all other send requests are rejected
	test.upper.SendDown("00");
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.lower.ThisLayerUp();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));

	//while we are sending, we should still be able to receive data as normal
	test.lower.SendUp("C0 77");
	test.upper.BufferEqualsHex("77");

	//this should put us back in the Ready state since it was a single tpdu send
	test.lower.SendSuccess();
	REQUIRE(test.upper.GetState().mSuccessCnt ==  1);

	test.lower.SendSuccess();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
}

TEST_CASE(SUITE("SendFailure"))
{
	TransportTestObject test(true);

	test.lower.DisableAutoSendCallback();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	REQUIRE("C0 11" ==  test.lower.PopWriteAsHex()); //FIR/FIN SEQ=0

	//this should put us back in the Ready state
	test.lower.SendFailure();
	REQUIRE(test.upper.GetState().mSuccessCnt ==  0);
	REQUIRE(test.upper.GetState().mFailureCnt ==  1);

	test.upper.SendDown("11");
	REQUIRE("C0 11" ==  test.lower.PopWriteAsHex()); // should resend with the same sequence number FIR/FIN SEQ=0
	test.lower.SendSuccess();
	REQUIRE(test.upper.GetState().mSuccessCnt ==  1);
	REQUIRE(test.upper.GetState().mFailureCnt ==  1);
}

TEST_CASE(SUITE("SendSuccess"))
{
	TransportTestObject test(true);

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	REQUIRE("C0 11" ==  test.lower.PopWriteAsHex()); //FIR/FIN SEQ=0

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	REQUIRE("C1 11" ==  test.lower.PopWriteAsHex()); //FIR/FIN SEQ=1
	REQUIRE(test.upper.GetState().mSuccessCnt ==  2);
}

//if we're in the middle of a send and the layer goes down
TEST_CASE(SUITE("ClosedWhileSending"))
{
	TransportTestObject test(true);
	test.upper.SendDown("11"); //get the layer into the sending state

	test.lower.ThisLayerDown(); // go to the TS_ClosedAfterSend state
	REQUIRE_FALSE(test.upper.IsLowerLayerUp());
}

TEST_CASE(SUITE("SendFullAPDU"))
{
	TransportTestObject test(true);

	uint32_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	uint32_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	vector<string> packets;
	std::string apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
	test.lower.DisableAutoSendCallback();
	test.upper.SendDown(apdu);

	//verify that each packet is received correctly
	for(string tpdu : packets)
	{
		REQUIRE(1 ==  test.lower.NumWrites());
		REQUIRE(tpdu ==  test.lower.PopWriteAsHex());
		REQUIRE(0 ==  test.lower.NumWrites());
		test.lower.SendSuccess();
	}
}



