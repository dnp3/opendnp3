#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"

#include <opendnp3/Objects.h>

using namespace opendnp3;
using namespace std;


BOOST_AUTO_TEST_SUITE(ObjectTests)
template<int T>
bool TestRange(size_t start)
{
	size_t stop = start + T * 8 - 1;

	Group1Var1* pObj = Group1Var1::Inst();

	assert( stop >= start );
	assert( (stop - start + 1) <= (T * 8) );

	uint8_t data[T];
	for(size_t i = 0; i < T; i++) data[i] = 0;

	//check that all bits can be set
	for(size_t i = start; i <= stop; i++) {
		pObj->Write(data, start, i, true);
	}
	for(size_t i = 0; i < T; i++) BOOST_REQUIRE_EQUAL(data[i], 255);

	//check that all bits can be unset
	for(size_t i = start; i <= stop; i++) {
		pObj->Write(data, start, i, false);
	}
	for(size_t i = 0; i < T; i++) BOOST_REQUIRE_EQUAL(data[i], 0);



	for(size_t i = start; i <= stop; i++) {
		uint8_t* pPos = data + (i - start) / 8; //the byte you are on
		size_t bit = (i - start) % 8;
		pObj->Write(data, start, i, true);
		BOOST_REQUIRE_EQUAL(*pPos, 1 << bit);
		pObj->Write(data, start, i, false);
		BOOST_REQUIRE_EQUAL(*pPos, 0);

	}

	return true;
}

BOOST_AUTO_TEST_CASE(BitfieldObjects)
{
	TestRange<1>(0);
	TestRange<1>(1);
	TestRange<2>(0);
	TestRange<2>(3);
	TestRange<4>(1234);
}
BOOST_AUTO_TEST_SUITE_END()
