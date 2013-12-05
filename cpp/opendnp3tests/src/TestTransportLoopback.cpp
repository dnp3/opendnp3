#include "TransportLoopbackTestObject.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <functional>

#include <openpal/Exception.h>

#include <opendnp3/ProtocolUtil.h>

#include <asiopal/Log.h>
#include <asiopal/PhysicalLayerAsyncSerial.h>

#include "TestHelpers.h"
#include "BufferHelpers.h"
#include "LoopbackPhysicalLayerAsync.h"

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace boost;

BOOST_AUTO_TEST_SUITE(TransportLoopbackTestSuite)

// Do a bidirectional send operation and proceed until both sides have correctly
// received all the data
void TestLoopback(TransportLoopbackTestObject* apTest, size_t aNumBytes)
{
	apTest->Start();

	BOOST_REQUIRE(apTest->ProceedUntil(std::bind(&TransportLoopbackTestObject::LayersUp, apTest)));

	ByteStr b(aNumBytes, 0);

	apTest->mUpperA.SendDown(b.ToReadOnly());
	apTest->mUpperB.SendDown(b.ToReadOnly());

	BOOST_REQUIRE(apTest->ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &(apTest->mUpperA), b.Size())));
	BOOST_REQUIRE(apTest->ProceedUntil(std::bind(&MockUpperLayer::SizeEquals, &(apTest->mUpperB), b.Size())));
	BOOST_REQUIRE(apTest->mUpperA.BufferEquals(b, b.Size()));
	BOOST_REQUIRE(apTest->mUpperB.BufferEquals(b, b.Size()));
}

BOOST_AUTO_TEST_CASE(TestTransportWithMockLoopback)
{
	LinkConfig cfgA(true, true);
	LinkConfig cfgB(false, true);

	EventLog log;
	boost::asio::io_service service;
	LoopbackPhysicalLayerAsync phys(Logger(&log, LogLevel::Warning, "loopback"), &service);
	TransportLoopbackTestObject t(&service, &phys, cfgA, cfgB);

	TestLoopback(&t, DEFAULT_FRAG_SIZE);
}

// Run this test on ARM to give us some regression protection for serial
#ifdef SERIAL_PORT
BOOST_AUTO_TEST_CASE(TestTransportWithSerialLoopback)
{
	LinkConfig cfgA(true, true);
	LinkConfig cfgB(false, true);

	cfgA.NumRetry = cfgB.NumRetry = 3;

	SerialSettings s;
	s.mDevice = TOSTRING(SERIAL_PORT);
	s.mBaud = 57600;
	s.mDataBits = 8;
	s.mStopBits = 1;
	s.mParity = PAR_NONE;
	s.mFlowType = FLOW_NONE;

	EventLog log;
	boost::asio::io_service service;
	PhysicalLayerAsyncSerial phys(log.GetLogger(LogLevel::Warning, "serial"), &service, s);
	TransportLoopbackTestObject t(&service, &phys, cfgA, cfgB);

	TestLoopback(&t, DEFAULT_FRAG_SIZE);
}
#endif

BOOST_AUTO_TEST_SUITE_END()
