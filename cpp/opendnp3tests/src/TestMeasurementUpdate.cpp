
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

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
