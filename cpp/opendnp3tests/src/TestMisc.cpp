#include <boost/test/unit_test.hpp>
#include "TestHelpers.h"

#include <iostream>
#include <map>
#include <vector>
#include <stdarg.h>
#include <math.h>
#include <sstream>
#include <bitset>
#include <signal.h>
#include <vector>

#include <opendnp3/Util.h>

using namespace std;
using namespace opendnp3;


BOOST_AUTO_TEST_SUITE(MiscTest)

BOOST_AUTO_TEST_CASE(ToUpperCase)
{
	string test("lower case");
	toUpperCase(test);
	BOOST_REQUIRE_EQUAL(test, "LOWER CASE");

	string test2("UPPPER case");
	toUpperCase(test2);
	BOOST_REQUIRE_EQUAL(test2, "UPPPER CASE");

	string test3("123456789abcDEF");
	toUpperCase(test3);
	BOOST_REQUIRE_EQUAL(test3, "123456789ABCDEF");
}

BOOST_AUTO_TEST_SUITE_END()
