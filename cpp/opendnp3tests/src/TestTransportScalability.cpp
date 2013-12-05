#include "TransportScalabilityTestObject.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/ProtocolUtil.h>

#include <openpal/Exception.h>

#include <functional>

using namespace std;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(AsyncTransportScalability)


BOOST_AUTO_TEST_CASE(TestSimpleSend)
{
	LinkConfig client(true, true);
	LinkConfig server(false, true);

	// Ubuntu and windows use different ephemeral port ranges...

#ifdef WIN32
	uint32_t port = 50000;
#else
	uint32_t port = 30000;
#endif

	// turned down the number of pairs for arm b/c of how long it takes to run.
#ifdef ARM
	uint16_t NUM_PAIRS = 50;
#else
	uint16_t NUM_PAIRS = 100;
#endif

	TransportScalabilityTestObject t(client, server, port, NUM_PAIRS);

	t.Start();

	BOOST_REQUIRE(t.ProceedUntil(std::bind(&TransportScalabilityTestObject::AllLayersUp, &t)));

	ByteStr b(2048, 0);

	t.SendToAll(b.ToReadOnly());

	BOOST_REQUIRE(t.ProceedUntil(std::bind(&TransportScalabilityTestObject::AllLayerReceived, &t, b.Size())));
	BOOST_REQUIRE(t.AllLayerEqual(b.ToReadOnly()));
}




BOOST_AUTO_TEST_SUITE_END()
