#ifndef __TEST_HELPERS_H_
#define __TEST_HELPERS_H_

#define BOOST_REQUIRE_FLOAT_EQUAL_TOL(a,b,tol) BOOST_REQUIRE(FloatEqual(a,b,tol))

#define BOOST_REQUIRE_FLOAT_EQUAL(a,b) BOOST_REQUIRE(FloatEqual(a,b,1e-6))

#define BOOST_REQUIRE_NOT_EQUAL(a,b) BOOST_REQUIRE(a != b)

#define BOOST_REQUIRE_FALSE(a) BOOST_REQUIRE(!a)

#endif
