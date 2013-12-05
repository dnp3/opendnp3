#include <boost/test/unit_test.hpp>

#include <asiopal/Log.h>

#include "PhysLoopback.h"
#include "MockExecutor.h"
#include "MockPhysicalLayerAsync.h"
#include "TestHelpers.h"

using namespace opendnp3;
using namespace openpal;
using namespace boost;

BOOST_AUTO_TEST_SUITE(PhysicalLayerLoopbackSuite)

class LoopbackTest
{
public:

	LoopbackTest() :
		log(),
		exe(),
		phys(Logger(&log, LogLevel::Info, "phys"), &exe),
		loopback(Logger(&log, LogLevel::Info, "loopback"), &phys) {
		loopback.Start();
	}

	asiopal::EventLog log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	PhysLoopback loopback;
};

BOOST_AUTO_TEST_CASE(StartsReadingWhenOpened)
{
	LoopbackTest test;

	BOOST_REQUIRE(test.phys.IsOpening());
	test.phys.SignalOpenSuccess();

	BOOST_REQUIRE(test.phys.IsReading());
	BOOST_REQUIRE_FALSE(test.phys.IsWriting());
}

BOOST_AUTO_TEST_CASE(EchosDataOnRead)
{
	LoopbackTest test;
	test.phys.SignalOpenSuccess();

	test.phys.TriggerRead("0A 0B 0C");
	BOOST_REQUIRE(test.phys.IsWriting());
	BOOST_REQUIRE_FALSE(test.phys.IsReading());
	BOOST_REQUIRE(test.phys.BufferEqualsHex("0A 0B 0C"));
}

BOOST_AUTO_TEST_CASE(ReadsAgainAfterDataIsWritten)
{
	LoopbackTest test;
	test.phys.SignalOpenSuccess();

	test.phys.TriggerRead("0A 0B 0C");
	test.phys.SignalSendSuccess();
	BOOST_REQUIRE(test.phys.IsReading());
	BOOST_REQUIRE_FALSE(test.phys.IsWriting());
	test.phys.TriggerRead("0D 0E 0F");
	BOOST_REQUIRE(test.phys.BufferEqualsHex("0A 0B 0C 0D 0E 0F"));
}

BOOST_AUTO_TEST_SUITE_END()
