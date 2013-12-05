#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/DNPCrc.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(CRC)

BOOST_AUTO_TEST_CASE(CrcTest)
{
	HexSequence hs("05 64 05 C0 01 00 00 04 E9 21");
	BOOST_REQUIRE_EQUAL(hs.Size(), 10);
	BOOST_REQUIRE_EQUAL(DNPCrc::CalcCrc(hs, 8), 0x21E9);
}

BOOST_AUTO_TEST_SUITE_END()
