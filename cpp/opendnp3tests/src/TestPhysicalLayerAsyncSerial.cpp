#include "AsyncSerialTestObject.h"

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>

#include <openpal/Exception.h>

using namespace opendnp3;
using namespace boost;
using namespace openpal;


//run the tests on arm to give us some protection
BOOST_AUTO_TEST_SUITE(PhysicalLayerSerialSuite)
#ifdef SERIAL_PORT

BOOST_AUTO_TEST_CASE(TestSendReceiveLoopback)
{
	SerialSettings s;
	s.mDevice = TOSTRING(SERIAL_PORT);
	s.mBaud = 9600;
	s.mDataBits = 8;
	s.mStopBits = 1;
	s.mParity = PAR_NONE;
	s.mFlowType = FLOW_NONE;

	AsyncSerialTestObject t(s);

	t.mPort.AsyncOpen();
	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUpper)));

	ByteStr b(4096, 0);
	t.mUpper.SendDown(b, b.Size());

	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::SizeEquals, &t.mUpper, b.Size())));
}

#endif

BOOST_AUTO_TEST_SUITE_END()

