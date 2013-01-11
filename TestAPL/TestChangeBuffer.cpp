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

#include <APL/ChangeBuffer.h>
#include <APL/FlexibleDataObserver.h>

using namespace apl;

BOOST_AUTO_TEST_SUITE(ChangeBufferTestSuite)

BOOST_AUTO_TEST_CASE(ChangeBufferMarshallsUpdates)
{
	Binary b(true);
	Analog a(-32);
	Counter c(64);

	ChangeBuffer cb;
	{
		Transaction t(&cb);
		cb.Update(b, 0);
		cb.Update(a, 1);
		cb.Update(c, 2);
	}

	FlexibleDataObserver fdo;
	
	{
		Transaction t(&cb);
		size_t num = cb.FlushUpdates(&fdo);
		BOOST_REQUIRE_EQUAL(num, 3);
		BOOST_REQUIRE_EQUAL(fdo.mBinaryMap[0], b);
		BOOST_REQUIRE_EQUAL(fdo.mAnalogMap[1], a);
		BOOST_REQUIRE_EQUAL(fdo.mCounterMap[2], c);
	}

}

BOOST_AUTO_TEST_SUITE_END()
