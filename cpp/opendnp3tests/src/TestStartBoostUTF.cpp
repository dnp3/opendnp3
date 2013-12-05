#define BOOST_TEST_MODULE opendnp3

/*
 * If we aren't using dynamic linking, make sure a main() function
 * is created anyway.
 */
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#endif

/*
 * Include the Boost UTF subsystem.  The rest is automagic.
 */
#include <boost/test/unit_test.hpp>
