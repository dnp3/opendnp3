#include <boost/test/unit_test.hpp>
#include "TestHelpers.h"
#include "LogTester.h"

#include <asiopal/Log.h>

#include <openpal/Exception.h>

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;
using namespace opendnp3;
using namespace openpal;

BOOST_AUTO_TEST_SUITE(LogTest)

BOOST_AUTO_TEST_CASE( LogErrorCounting)
{
	LogTester log;
	Logger logger(&log, LogLevel::Debug, "test1");
	logger.Log( LogLevel::Debug, "LogEntryParamsTest", "MessageMessage", 5 );

	BOOST_REQUIRE_EQUAL(log.NextErrorCode(), 5);
	BOOST_REQUIRE_EQUAL(log.NextErrorCode(), -1);
}

BOOST_AUTO_TEST_SUITE_END()
