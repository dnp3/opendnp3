#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "AsyncPhysBaseTest.h"

#include <openpal/Exception.h>

using namespace opendnp3;
using namespace openpal;

BOOST_AUTO_TEST_SUITE(PhysicalLayerAsyncBaseSuite)
BOOST_AUTO_TEST_CASE(ClosedState)
{
	AsyncPhysBaseTest t;
	uint8_t buff;
	WriteBuffer wb(&buff, 1);

	BOOST_REQUIRE_THROW(t.phys.AsyncClose(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.upper.SendDown("00"), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.AsyncRead(wb), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalOpenFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalOpenSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalSendFailure(), InvalidStateException);

}

BOOST_AUTO_TEST_CASE(OpenCloseNotification)
{
	AsyncPhysBaseTest t;
	const size_t NUM = 3;

	for(size_t i = 1; i <= NUM; ++i) {
		t.phys.AsyncOpen();
		t.phys.SignalOpenSuccess();
		BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerUp, i);
		t.phys.AsyncClose();

		BOOST_REQUIRE_THROW(t.phys.SignalOpenFailure(), InvalidStateException);
		BOOST_REQUIRE_THROW(t.phys.SignalOpenSuccess(), InvalidStateException);
		BOOST_REQUIRE_THROW(t.phys.SignalSendSuccess(), InvalidStateException);
		BOOST_REQUIRE_THROW(t.phys.SignalSendFailure(), InvalidStateException);

		BOOST_REQUIRE_EQUAL(t.phys.NumClose(), i);
		BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, i);
	}
}

BOOST_AUTO_TEST_CASE(ReadState)
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();
	t.adapter.StartRead(); //start a read

	BOOST_REQUIRE_THROW(t.phys.SignalOpenFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalOpenSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalSendFailure(), InvalidStateException);

	t.phys.TriggerRead("00");
	t.upper.BufferEqualsHex("00");
}

BOOST_AUTO_TEST_CASE(CloseWhileReading)
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();
	t.adapter.StartRead();

	t.phys.AsyncClose();
	BOOST_REQUIRE_EQUAL(t.phys.NumClose(), 1);
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 0); //layer shouldn't go down until the outstanding read comes back
	t.phys.SignalReadFailure();
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 1);
}

BOOST_AUTO_TEST_CASE(WriteState)
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	BOOST_REQUIRE_EQUAL(t.phys.Size(), 1);

	BOOST_REQUIRE_THROW(t.phys.SignalOpenFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalOpenSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.TriggerRead(""), InvalidStateException);
	BOOST_REQUIRE_THROW(t.phys.SignalReadFailure(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(CloseWhileWriting)
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	t.phys.AsyncClose();
	BOOST_REQUIRE_EQUAL(t.phys.NumClose(), 1);
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 0); //layer shouldn't go down until the outstanding write comes back
	t.phys.SignalSendFailure();
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 1);
}

BOOST_AUTO_TEST_CASE(CloseWhileReadingWriting)
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	t.adapter.StartRead();
	t.phys.AsyncClose();
	BOOST_REQUIRE_EQUAL(t.phys.NumClose(), 1);
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 0);
	t.phys.SignalSendFailure();
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 0);
	t.phys.SignalReadFailure();
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 1);
}

BOOST_AUTO_TEST_CASE(CloseWhileWritingReading)
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	t.adapter.StartRead();
	t.phys.AsyncClose();
	BOOST_REQUIRE_EQUAL(t.phys.NumClose(), 1);
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 0);
	t.phys.SignalReadFailure();
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 0);
	t.phys.SignalSendFailure();
	BOOST_REQUIRE_EQUAL(t.upper.GetState().mNumLayerDown, 1);
}

BOOST_AUTO_TEST_CASE(CloseWhileOpening)
{
	AsyncPhysBaseTest t;

	t.phys.AsyncOpen();
	t.phys.AsyncClose();
	BOOST_REQUIRE(t.phys.IsOpening());
	BOOST_REQUIRE(t.phys.IsClosing());

	/* this could happen for some layers, but we
	   still need to return an open failure to the handler */
	t.phys.SignalOpenSuccess();

	BOOST_REQUIRE_EQUAL(0, t.upper.GetState().mNumLayerUp);
	BOOST_REQUIRE_EQUAL(1, t.adapter.GetNumOpenFailure());
}

BOOST_AUTO_TEST_SUITE_END()

