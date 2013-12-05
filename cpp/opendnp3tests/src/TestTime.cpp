#include <boost/test/unit_test.hpp>

#include <opendnp3/Util.h>

#include "TestHelpers.h"
#include "Timeout.h"
#include "StopWatch.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(TimeoutTests)

BOOST_AUTO_TEST_CASE(TimeoutCorrectlyNotExpired)
{
	//create a timeout of 1 millisecond
	Timeout to(milliseconds(1000));

	//check that the timeout starts out unexpired
	BOOST_REQUIRE_EQUAL(to.IsExpired(), false);
	BOOST_REQUIRE(to.Remaining() > milliseconds(1));
}

BOOST_AUTO_TEST_CASE(TimeoutCorrectlyExpires)
{
	//create a timeout of 1 millisecond
	Timeout to(milliseconds(1));

	std::this_thread::sleep_for(milliseconds(10));

	//check that the timeout starts out unexpired
	BOOST_REQUIRE(to.IsExpired());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(StopWatchTests)

BOOST_AUTO_TEST_CASE(BasicTest)
{
	StopWatch sw;

	BOOST_REQUIRE(sw.Elapsed(false) <= milliseconds(100));
}

BOOST_AUTO_TEST_SUITE_END()

