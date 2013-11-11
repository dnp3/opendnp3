
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

#include "TestHelpers.h"
#include "StopWatch.h"

#include <opendnp3/Log.h>
#include <opendnp3/LogToStdio.h>

#include <openpal/Exception.h>

#include <boost/asio.hpp>

#include "IntegrationTest.h"

#define OUTPUT_PERF_NUMBERS	(0)

using namespace opendnp3;
using namespace openpal;
using namespace std;
using namespace std::chrono;

/*
 * Each platform needs slightly different values.  Figure out what they are,
 * and use the macros in the test cases below.
 */
#if defined(WIN32)
/* Windows platform */
#define MACRO_PORT_START	(50000)
#define MACRO_NUM_PAIRS		(10)
#elif defined(ARM)
/* Linux on ARM platform */
#define MACRO_PORT_START	(30000)
#define MACRO_NUM_PAIRS		(10)
#else
/* Generic Linux platform */
#define MACRO_PORT_START	(30000)
#define MACRO_NUM_PAIRS		(100)
#endif

BOOST_AUTO_TEST_SUITE(IntegrationSuite)

const boost::uint16_t START_PORT = MACRO_PORT_START;
const size_t NUM_PAIRS = MACRO_NUM_PAIRS;
const size_t NUM_POINTS = 500;
const size_t NUM_CHANGE_SETS = 10;
const FilterLevel FILTER_LEVEL = LEV_INFO;

BOOST_AUTO_TEST_CASE(MasterToSlaveThroughput)
{
	IntegrationTest t(FILTER_LEVEL, START_PORT, NUM_PAIRS, NUM_POINTS);

	size_t num_points_per_pair = 0;
	StopWatch sw;
	for (size_t j = 0; j < NUM_CHANGE_SETS; ++j) {
		num_points_per_pair += t.IncrementData();
		BOOST_REQUIRE(t.WaitForSameData(60000, true));
		//cout << "iteration: " << j << endl;
	}

	if (OUTPUT_PERF_NUMBERS) {
		double elapsed_sec = duration_cast<milliseconds>(sw.Elapsed()).count() / 1000.0;
		size_t points = num_points_per_pair * NUM_PAIRS * 2;
		cout << "num points: " << points << endl;
		cout << "elapsed seconds: " << elapsed_sec << endl;
		cout << "points/sec: " << points / elapsed_sec << endl;
	}

}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
