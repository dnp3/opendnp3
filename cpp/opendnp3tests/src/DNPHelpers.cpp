
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
// you under the terms of the License.
//

#include "DNPHelpers.h"

#include <boost/test/unit_test.hpp>

#include <opendnp3/DNPCrc.h>
#include <opendnp3/LinkFrame.h>

#include <openpal/ToHex.h>

#include "BufferHelpers.h"


namespace opendnp3
{

bool IsFrameEqual(LinkFrame& frame, const std::string& arData)
{

	HexSequence hs(arData);
	if(frame.GetSize() != hs.Size()) return false;
	uint8_t* buff = frame.GetBuffer();

	for(size_t i = 0; i < hs.Size(); i++) {
		if(buff[i] != hs[i]) return false;
	}

	return true;
}

std::string RepairCRC(const std::string& arData)
{
	HexSequence hs(arData);

	//validate the size of the data
	BOOST_REQUIRE(hs.Size() >= 10);
	BOOST_REQUIRE(hs.Size() <= 292);

	//first determine how much user data is present
	size_t full_blocks = (hs.Size() - 10) / 18;
	size_t partial_size = (hs.Size() - 10) % 18;

	//can't have a partial size < 3 since even 1 byte requires 2 CRC bytes
	if(partial_size > 0) {
		BOOST_REQUIRE(partial_size >= 3);
	}

	//repair the header crc
	DNPCrc::AddCrc(hs, 8);

	uint8_t* ptr = hs + 10;

	// repair the full blocks
	for(size_t i = 0; i < full_blocks; i++) {
		DNPCrc::AddCrc(ptr, 16);
		ptr += 18;
	}

	//repair the partial block
	if(partial_size > 0) DNPCrc::AddCrc(ptr, partial_size - 2);

	return openpal::toHex(hs, hs.Size(), true);
}

}

