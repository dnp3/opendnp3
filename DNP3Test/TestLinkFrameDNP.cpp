
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <DNP3/LinkFrame.h>
#include <DNP3/DNPCrc.h>
#include <APLTestTools/BufferHelpers.h>

#include "DNPHelpers.h"

using namespace apl;
using namespace apl::dnp;

void FormatUserData(apl::dnp::LinkFrame& f, bool aIsMaster, bool aIsConfirmed, int aDest, int aSrc, const std::string& arData,  bool aFcb = false)
{
	HexSequence hs(arData);
	BOOST_REQUIRE(hs.Size() < 250);

	if(aIsConfirmed) {
		f.FormatConfirmedUserData(aIsMaster, aFcb, aDest, aSrc, hs, hs.Size());
	}
	else {
		f.FormatUnconfirmedUserData(aIsMaster, aDest, aSrc, hs, hs.Size());
	}
}

BOOST_AUTO_TEST_SUITE(DNPLinkFrame)

BOOST_AUTO_TEST_CASE(LinkHeaderChangeFCB)
{
	LinkHeader hdr;
	hdr.Set(5, 1, 1024, true, true, true, FC_PRI_CONFIRMED_USER_DATA);

	BOOST_REQUIRE_EQUAL(hdr.ControlByte(true, true, true, FC_PRI_CONFIRMED_USER_DATA), hdr.GetControl());

	hdr.ChangeFCB(false);

	BOOST_REQUIRE_EQUAL(hdr.ControlByte(true, false, true, FC_PRI_CONFIRMED_USER_DATA), hdr.GetControl());
}

BOOST_AUTO_TEST_CASE(LinkHeaderToString)
{
	LinkHeader hdr;

	hdr.Set(5, 1, 1024, true, true, true, FC_PRI_CONFIRMED_USER_DATA);
	BOOST_REQUIRE_EQUAL("DL 1 to 1024 : FC_PRI_CONFIRMED_USER_DATA PayloadSize: 0 From Master Pri->Sec FCB=1 FCV=1", hdr.ToString());

	hdr.Set(5, 1, 1024, true, true, false, FC_SEC_ACK);
	BOOST_REQUIRE_EQUAL("DL 1 to 1024 : FC_SEC_ACK PayloadSize: 0 From Master Sec->Pri DFC=1", hdr.ToString());

	hdr.Set(5, 1, 1024, true, true, true, FC_SEC_ACK);
	BOOST_REQUIRE_EQUAL("DL 1 to 1024 : FC_SEC_ACK PayloadSize: 0 From Master Sec->Pri ERROR: FCB not Blank!! DFC=1", hdr.ToString());
}

BOOST_AUTO_TEST_CASE(CopyConstructor) //make sure the default copies the buffer properly
{
	LinkFrame a;
	for(size_t i = 0; i < a.GetSize(); i++)
		a.GetBuffer()[i] = static_cast<uint8_t>(i);

	LinkFrame b(a);

	//verify that the buffers are equal
	for(size_t i = 0; i < a.GetSize(); i++) {
		BOOST_REQUIRE_EQUAL(a.GetBuffer()[i], b.GetBuffer()[i]);
	}

	for(size_t i = 0; i < a.GetSize(); i++) {
		//verify that that buffers are different memory locations
		BOOST_REQUIRE_NOT_EQUAL(a.GetBuffer() + i, b.GetBuffer() + i);
	}
}

BOOST_AUTO_TEST_CASE(ResetLinks)
{
	LinkFrame f;
	BOOST_REQUIRE(!f.IsComplete());

	// ResetLinkStates - Master
	f.FormatResetLinkStates(true, 1, 1024);
	BOOST_REQUIRE(f.IsComplete());
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 05 C0 01 00 00 04 E9 21"));
}

BOOST_AUTO_TEST_CASE(ACK)
{
	LinkFrame f;

	//ACK - Slave (DFC = false)
	f.FormatAck(false, false, 1024, 1);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 05 00 00 04 01 00 19 A6")); //ACK - Slave

	// ACK - Slave (DFC = true)
	f.FormatAck(false, true, 1024, 1);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 05 10 00 04 01 00 8B 0C")); // ACK - Slave (with DFC set)

	// ACK - Master (DFC = false)
	f.FormatAck(true, false, 1, 1024);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 05 80 01 00 00 04 53 11"));

	// ACK - Master (DFC = true)
	f.FormatAck(true, true, 1, 1024);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 05 90 01 00 00 04 C1 BB"));
}

BOOST_AUTO_TEST_CASE(ConfirmedUserData)
{
	LinkFrame f;

	// Confirmed User Data - Master (FCB = true)
	FormatUserData(f, true, true, 1, 1024, "C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06", true);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 14 F3 01 00 00 04 0A 3B C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06 9A 12"));

	// Confirmed User Data - Slave (FCB = true)
	FormatUserData(f, false, true, 1024, 1, "C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00", true);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 53 73 00 04 01 00 03 FC C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 05 24 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 B4 77 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 A5 25 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 2F AC 00 1E 02 01 00 00 01 00 01 00 00 01 00 00 16 ED"));
}

BOOST_AUTO_TEST_CASE(ResetLinkStates)
{
	LinkFrame f;
	// Reset Link States - Slave
	f.FormatResetLinkStates(false, 1024, 1);
	BOOST_REQUIRE(IsFrameEqual(f, "05 64 05 40 00 04 01 00 A3 96"));
}

BOOST_AUTO_TEST_CASE(UnconfirmedUserData)
{
	LinkFrame f;
	// Unconfirmed User Data - Slave

	FormatUserData(f, false, false, 1024, 1, "C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00");
	//take the packet from the test above, change the control byte to 44 , repair the crc
	BOOST_REQUIRE(IsFrameEqual(f, RepairCRC("05 64 53 44 00 04 01 00 FF FF C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 FF FF 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 FF FF 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 FF FF 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 FF FF 00 1E 02 01 00 00 01 00 01 00 00 01 00 00 FF FF")));
}

BOOST_AUTO_TEST_CASE(LinkStatus)
{
	LinkFrame f;
	// LinkStatus - Master (DFC = true)
	f.FormatLinkStatus(true, true, 1, 1024);
	//take a length 10 frame, set the control to 9B and repair the CRC
	BOOST_REQUIRE(IsFrameEqual(f, RepairCRC("05 64 05 9B 01 00 00 04 E9 21")));
}

BOOST_AUTO_TEST_CASE(NotSupported)
{
	LinkFrame f;
	// Not Supported - Slave (DFC = true)
	f.FormatNotSupported(false, true, 1, 1024);
	BOOST_REQUIRE(IsFrameEqual(f, RepairCRC("05 64 05 1F 01 00 00 04 28 5A")));
}


BOOST_AUTO_TEST_SUITE_END() //end suite

