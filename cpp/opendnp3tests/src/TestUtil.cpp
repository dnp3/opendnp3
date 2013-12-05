#include <boost/test/unit_test.hpp>

#include <opendnp3/Util.h>

#include "TestHelpers.h"
#include "BufferHelpers.h"

using namespace std;
using namespace opendnp3;


BOOST_AUTO_TEST_SUITE(UtilSuite)
template <int N>
void TestHex(const std::string& aHex, uint8_t* aCompareBytes, size_t aCount)
{
	HexSequence hs(aHex);

	BOOST_REQUIRE(hs.Size() <= N);

	BOOST_REQUIRE_EQUAL(hs.Size(), aCount );
	for ( size_t i = 0; i < aCount; i++ )
		BOOST_REQUIRE_EQUAL(hs[i], aCompareBytes[i]);
}

BOOST_AUTO_TEST_CASE(HexToBytes2TestSmall)
{
	uint8_t values[] = { 0xAF, 0x23 };
	TestHex<2>( "AF23", values, 2 );
}
BOOST_AUTO_TEST_CASE(HexToBytes2Test64)
{
	uint8_t values[] = { 0x13, 0xA2, 0x00, 0x40, 0x56, 0x1D, 0x08 };
	TestHex<7>( "13A20040561D08", values, 7 );
}

BOOST_AUTO_TEST_CASE(HexToBytes2Test64TooBig)
{
	uint8_t values[] = { 0x13, 0xA2, 0x00, 0x40, 0x56, 0x1D, 0x08 };
	TestHex<8>( "13A20040561D08", values, 7 );
}

BOOST_AUTO_TEST_CASE(HexToBytes2Test64Hole)
{
	uint8_t values[] = { 0x13, 0xA2, 0x00, 0x40, 0x56, 0x1D, 0x08 };
	TestHex<8>( "13A200 40561   D08", values, 7 );
}

BOOST_AUTO_TEST_SUITE_END()
