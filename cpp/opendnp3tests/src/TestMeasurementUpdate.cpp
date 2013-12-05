#include <boost/test/unit_test.hpp>

#include <opendnp3/MeasurementUpdate.h>
#include <openpal/MoveOnCopy.h>

#include <functional>

using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(MeasurementUpdateSuite)

BOOST_AUTO_TEST_CASE(MovingMeasurementUpdates)
{
	MeasurementUpdate update1;
	update1.Add(Binary(true), 42);

	BOOST_REQUIRE(update1.HasUpdates());

	MeasurementUpdate update2(std::move(update1));
	BOOST_REQUIRE(!update1.HasUpdates());
	BOOST_REQUIRE(update2.HasUpdates());
}

BOOST_AUTO_TEST_CASE(LambdaVariableCaptureCanMove)
{
	MeasurementUpdate update;
	update.Add(Binary(true), 42);
	update.Add(Binary(false), 13);

	std::function<int (const IMeasurementUpdate&)> process = [](const IMeasurementUpdate& arUpdate) {
		int i = 0;
		for(auto binary: arUpdate.BinaryUpdates()) ++i;
		return i;					
	};

	openpal::move_on_copy<MeasurementUpdate> moved(std::move(update));

	BOOST_REQUIRE(!update.HasUpdates());  //value has been moved

	std::function<int ()> capture = [&process, moved]() {
		return process(moved.Value());
	};

	BOOST_REQUIRE_EQUAL(2, capture());
}

BOOST_AUTO_TEST_SUITE_END()
