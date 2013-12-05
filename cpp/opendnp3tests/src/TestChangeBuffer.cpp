#include <boost/test/unit_test.hpp>

#include <opendnp3/ChangeBuffer.h>

#include "FlexibleDataObserver.h"

using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(ChangeBufferTestSuite)

BOOST_AUTO_TEST_CASE(ChangeBufferMarshallsUpdates)
{
	Binary b(true);
	Analog a(-32);
	Counter c(64);

	ChangeBuffer cb;
	{
		Transaction t(&cb);
		cb.Update(b, 0);
		cb.Update(a, 1);
		cb.Update(c, 2);
	}

	FlexibleDataObserver fdo;

	{
		Transaction t(&cb);
		size_t num = cb.FlushUpdates(&fdo);
		BOOST_REQUIRE_EQUAL(num, 3);
		BOOST_REQUIRE_EQUAL(fdo.mBinaryMap[0], b);
		BOOST_REQUIRE_EQUAL(fdo.mAnalogMap[1], a);
		BOOST_REQUIRE_EQUAL(fdo.mCounterMap[2], c);
	}

}

BOOST_AUTO_TEST_SUITE_END()
