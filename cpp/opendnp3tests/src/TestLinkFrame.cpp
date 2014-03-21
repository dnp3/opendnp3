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

#include <opendnp3/link/LinkFrame.h>
#include <opendnp3/link/DNPCrc.h>
#include <opendnp3/link/LinkHeader.h>

#include <openpal/ToHex.h>
#include <openpal/StaticBuffer.h>

#include "BufferHelpers.h"
#include "DNPHelpers.h"

using namespace opendnp3;
using namespace openpal;

std::string FormatUserData(bool aIsMaster, bool aIsConfirmed, int aDest, int aSrc, const std::string& data,  bool aFcb = false)
{
	HexSequence hs(data);
	REQUIRE(hs.Size() < 250);

	uint8_t buffer[292];
	WriteBuffer wrapper(buffer, 292);

	if(aIsConfirmed)
	{
		return toHex(LinkFrame::FormatConfirmedUserData(wrapper, aIsMaster, aFcb, aDest, aSrc, hs, hs.Size()));
	}
	else
	{
		return toHex(LinkFrame::FormatUnconfirmedUserData(wrapper, aIsMaster, aDest, aSrc, hs, hs.Size()));
	}
}

#define SUITE(name) "DNPLinkFrame - " name

TEST_CASE(SUITE("LinkHeaderChangeFCB"))
{
	LinkHeader hdr(5, 1, 1024, true, true, true, LinkFunction::PRI_CONFIRMED_USER_DATA);
	

	REQUIRE(hdr.ControlByte(true, true, true, LinkFunction::PRI_CONFIRMED_USER_DATA) ==  hdr.GetControl());

	hdr.ChangeFCB(false);

	REQUIRE(hdr.ControlByte(true, false, true, LinkFunction::PRI_CONFIRMED_USER_DATA) ==  hdr.GetControl());
}

TEST_CASE(SUITE("LinkHeaderToString"))
{
	LinkHeader hdr;

	hdr.Set(5, 1, 1024, true, true, true, LinkFunction::PRI_CONFIRMED_USER_DATA);
	REQUIRE("DL 1 to 1024 : PRI_CONFIRMED_USER_DATA PayloadSize: 0 From Master Pri->Sec FCB=1 FCV=1" ==  hdr.ToString());

	hdr.Set(5, 1, 1024, true, true, false, LinkFunction::SEC_ACK);
	REQUIRE("DL 1 to 1024 : SEC_ACK PayloadSize: 0 From Master Sec->Pri DFC=1" ==  hdr.ToString());

	hdr.Set(5, 1, 1024, true, true, true, LinkFunction::SEC_ACK);
	REQUIRE("DL 1 to 1024 : SEC_ACK PayloadSize: 0 From Master Sec->Pri ERROR: FCB not Blank!! DFC=1" ==  hdr.ToString());
}

TEST_CASE(SUITE("ResetLinks"))
{
	StaticBuffer<292> buffer;

	// ResetLinkStates - Master
	auto wrapper = LinkFrame::FormatResetLinkStates(buffer.GetWriteBuffer(), true, 1, 1024);	
	REQUIRE(toHex(wrapper) == "05 64 05 C0 01 00 00 04 E9 21");
}

TEST_CASE(SUITE("ACK"))
{
	StaticBuffer<292> buffer;

	//ACK - Slave (DFC = false)
	{
		auto wrapper = LinkFrame::FormatAck(buffer.GetWriteBuffer(), false, false, 1024, 1);
		REQUIRE(toHex(wrapper) == "05 64 05 00 00 04 01 00 19 A6"); //ACK - Slave
	}

	// ACK - Slave (DFC = true)
	{
		auto wrapper = LinkFrame::FormatAck(buffer.GetWriteBuffer(), false, true, 1024, 1);
		REQUIRE(toHex(wrapper) == "05 64 05 10 00 04 01 00 8B 0C"); // ACK - Slave (with DFC set)
	}

	{	// ACK - Master (DFC = false)
		auto wrapper = LinkFrame::FormatAck(buffer.GetWriteBuffer(), true, false, 1, 1024);
		REQUIRE(toHex(wrapper) == "05 64 05 80 01 00 00 04 53 11");
	}

	{
		// ACK - Master (DFC = true)
		auto wrapper = LinkFrame::FormatAck(buffer.GetWriteBuffer(), true, true, 1, 1024);
		REQUIRE(toHex(wrapper) == "05 64 05 90 01 00 00 04 C1 BB");
	}
}

TEST_CASE(SUITE("ConfirmedUserData"))
{
	StaticBuffer<292> buffer;

	// Confirmed User Data - Master (FCB = true)
	auto hex = FormatUserData(true, true, 1, 1024, "C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06", true);
	REQUIRE(hex == "05 64 14 F3 01 00 00 04 0A 3B C0 C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06 9A 12");

	// Confirmed User Data - Slave (FCB = true)
	auto hex2 = FormatUserData(false, true, 1024, 1, "C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00", true);
	REQUIRE(hex2 == "05 64 53 73 00 04 01 00 03 FC C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 05 24 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 B4 77 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 A5 25 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 2F AC 00 1E 02 01 00 00 01 00 01 00 00 01 00 00 16 ED");
}

TEST_CASE(SUITE("ResetLinkStates"))
{
	StaticBuffer<292> buffer;

	// Reset Link States - Slave
	auto wrapper = LinkFrame::FormatResetLinkStates(buffer.GetWriteBuffer(), false, 1024, 1);
	REQUIRE(toHex(wrapper) == "05 64 05 40 00 04 01 00 A3 96");
}

TEST_CASE(SUITE("UnconfirmedUserData"))
{
	auto hex = FormatUserData(false, false, 1024, 1, "C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00");
	//take the packet from the test above, change the control byte to 44 , repair the crc
	REQUIRE(hex == RepairCRC("05 64 53 44 00 04 01 00 FF FF C1 E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 FF FF 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 FF FF 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 FF FF 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 FF FF 00 1E 02 01 00 00 01 00 01 00 00 01 00 00 FF FF"));
}

TEST_CASE(SUITE("LinkStatus"))
{
	StaticBuffer<292> buffer;
	
	// LinkStatus - Master (DFC = true)
	auto wrapper = LinkFrame::FormatLinkStatus(buffer.GetWriteBuffer(), true, true, 1, 1024);
	//take a length 10 frame, set the control to 9B and repair the CRC
	REQUIRE(toHex(wrapper) == RepairCRC("05 64 05 9B 01 00 00 04 E9 21"));
}

TEST_CASE(SUITE("NotSupported"))
{
	StaticBuffer<292> buffer;

	// Not Supported - Slave (DFC = true)
	auto wrapper = LinkFrame::FormatNotSupported(buffer.GetWriteBuffer(), false, true, 1, 1024);
	REQUIRE(toHex(wrapper) == RepairCRC("05 64 05 1F 01 00 00 04 28 5A"));
}




