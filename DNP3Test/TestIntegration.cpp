
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>
#include <APLTestTools/StopWatch.h>

#include <APL/Log.h>
#include <APL/LogToStdio.h>
#include <APL/Exception.h>

#include <boost/asio.hpp>

#include "IntegrationTest.h"

#define OUTPUT_PERF_NUMBERS	(0)

using namespace apl;
using namespace apl::dnp;
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
	EventLog log;
	//LogToStdio::Inst()->SetPrintLocation(true);
	//log.AddLogSubscriber(LogToStdio::Inst());

	IntegrationTest t(log.GetLogger(FILTER_LEVEL, "test"), FILTER_LEVEL, START_PORT,
	                  NUM_PAIRS, NUM_POINTS);

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

// TODO - Factor this test into smaller tests
BOOST_AUTO_TEST_CASE(IntegrationTestConstructionDestruction)
{
	EventLog log;
	//log.AddLogSubscriber(LogToStdio::Inst());

	IntegrationTest t(log.GetLogger(LEV_WARNING, "test"), LEV_WARNING, START_PORT,
	                  NUM_PAIRS, NUM_POINTS);

	/* Verify that the Master and Slave stacks were created */
	{
		std::vector<std::string> list;

		std::ostringstream oss;
		oss << "Port: " << START_PORT << " Client ";

		list = t.GetManager()->GetStackNames();
		BOOST_REQUIRE_EQUAL(list.size(), NUM_PAIRS * 2);
		BOOST_REQUIRE_EQUAL(list[0], oss.str());
	}

	/* Verify that the Master and Slave ports were created */
	{
		std::vector<std::string> list;

		std::ostringstream oss;
		oss << "Port: " << START_PORT << " Client ";

		list = t.GetManager()->GetPortNames();
		BOOST_REQUIRE_EQUAL(list.size(), NUM_PAIRS * 2);
		BOOST_REQUIRE_EQUAL(list[0], oss.str());
	}

	/* Verify that GetVtoWriter behaves as expected */
	{
		std::ostringstream oss;
		oss << "Port: " << START_PORT;

		std::string client1 = oss.str() + " Client ";
		std::string client2 = oss.str() + " Server ";

		IVtoWriter* pWriter1a = NULL;
		IVtoWriter* pWriter1b = NULL;
		IVtoWriter* pWriter2 = NULL;

		BOOST_REQUIRE_NO_THROW(pWriter1a = t.GetManager()->GetVtoWriter(client1));
		BOOST_REQUIRE_NO_THROW(pWriter1b = t.GetManager()->GetVtoWriter(client1));
		BOOST_REQUIRE_NO_THROW(pWriter2 = t.GetManager()->GetVtoWriter(client2));

		BOOST_REQUIRE(pWriter1a == pWriter1b);
		BOOST_REQUIRE(pWriter1a != pWriter2);

		BOOST_REQUIRE_THROW(t.GetManager()->GetVtoWriter("trash"), ArgumentException);
	}

	/* Remove a stack, but leave the port associated */
	{
		std::vector<std::string> list;

		std::ostringstream oss;
		oss << "Port: " << START_PORT << " Client ";

		list = t.GetManager()->GetStackNames();
		BOOST_REQUIRE_EQUAL(list.size(), NUM_PAIRS * 2);
		BOOST_REQUIRE(list[0] == oss.str());

		list = t.GetManager()->GetPortNames();
		BOOST_REQUIRE_EQUAL(list.size(), NUM_PAIRS * 2);
		BOOST_REQUIRE(list[0] == oss.str());

		BOOST_REQUIRE_NO_THROW(t.GetManager()->RemoveStack(oss.str()));

		list = t.GetManager()->GetStackNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 1);
		BOOST_REQUIRE(list[0] != oss.str());

		list = t.GetManager()->GetPortNames();
		BOOST_REQUIRE_EQUAL(list.size(), NUM_PAIRS * 2);
		BOOST_REQUIRE(list[0] == oss.str());

		BOOST_REQUIRE_NO_THROW(t.GetManager()->RemovePort(oss.str()));

		list = t.GetManager()->GetStackNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 1);
		BOOST_REQUIRE(list[0] != oss.str());

		list = t.GetManager()->GetPortNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 1);
		BOOST_REQUIRE(list[0] != oss.str());
	}

	/* Remove a port and its associated stacks */
	{
		std::vector<std::string> list;

		std::ostringstream oss;
		oss << "Port: " << START_PORT << " Server ";

		list = t.GetManager()->GetStackNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 1);
		BOOST_REQUIRE(list[0] == oss.str());

		list = t.GetManager()->GetPortNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 1);
		BOOST_REQUIRE(list[0] == oss.str());

		BOOST_REQUIRE_NO_THROW(t.GetManager()->RemovePort(oss.str()));

		list = t.GetManager()->GetStackNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 2);
		BOOST_REQUIRE(list[0] != oss.str());

		list = t.GetManager()->GetPortNames();
		BOOST_REQUIRE_EQUAL(list.size(), (NUM_PAIRS * 2) - 2);
		BOOST_REQUIRE(list[0] != oss.str());
	}

	/* Try to remove a non-existant stack and port */
	{
		BOOST_REQUIRE_THROW(t.GetManager()->RemoveStack("trash"), ArgumentException);
		BOOST_REQUIRE_THROW(t.GetManager()->RemovePort("trash"), ArgumentException);
	}
}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
