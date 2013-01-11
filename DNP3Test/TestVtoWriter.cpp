//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include <boost/test/unit_test.hpp>

#include <APL/RandomizedBuffer.h>
#include <APL/Log.h>
#include <DNP3/VtoWriter.h>

using namespace std;
using namespace apl;
using namespace apl::dnp;

BOOST_AUTO_TEST_SUITE(VtoWriterSuite)

BOOST_AUTO_TEST_CASE(OnlyAcceptsMaximumSize)
{
	EventLog log;
	VtoWriter writer(log.GetLogger(LEV_DEBUG, "writer"), 3);

	/* Initialize the data stream to a pseudo-random sequence */
	RandomizedBuffer data(1024);

	BOOST_REQUIRE_EQUAL(writer.Write(data, data.Size(), 5), 255 * 3);
	BOOST_REQUIRE_EQUAL(writer.Write(data, data.Size(), 5), 0);
}

BOOST_AUTO_TEST_SUITE_END()
