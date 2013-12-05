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

