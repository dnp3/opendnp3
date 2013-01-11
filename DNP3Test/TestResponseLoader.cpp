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
#include <APLTestTools/TestHelpers.h>

#include "ResponseLoaderTestObject.h"


using namespace apl;
using namespace apl::dnp;
using namespace boost;


BOOST_AUTO_TEST_SUITE(ResponseLoaderSuite)
BOOST_AUTO_TEST_CASE(Group1Var1)
{
	ResponseLoaderTestObject t;
	t.CheckBinaries("C0 81 00 00 01 01 00 01 03 02");
}

BOOST_AUTO_TEST_CASE(Group1Var2)
{
	ResponseLoaderTestObject t;
	t.CheckBinaries("C0 81 00 00 01 02 00 01 03 01 81 01");
}

BOOST_AUTO_TEST_CASE(Group20Var1)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 01 00 00 01 01 04 00 00 00 01 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group20Var2)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 02 00 00 01 01 04 00 01 09 00");
}

BOOST_AUTO_TEST_CASE(Group20Var3)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 03 00 00 01 01 04 00 00 00 01 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group20Var4)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 04 00 00 01 01 04 00 01 09 00");
}

BOOST_AUTO_TEST_CASE(Group20Var5)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 05 00 00 01 04 00 00 00 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group20Var6)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 06 00 00 01 04 00 09 00");
}

BOOST_AUTO_TEST_CASE(Group20Var7)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 07 00 00 01 04 00 00 00 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group20Var8)
{
	ResponseLoaderTestObject t;
	t.CheckCounters("C0 81 00 00 14 08 00 00 01 04 00 09 00");
}

BOOST_AUTO_TEST_CASE(Group30Var1)
{
	ResponseLoaderTestObject t;
	t.CheckAnalogs("C0 81 00 00 1E 01 00 00 01 01 04 00 00 00 01 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group30Var2)
{
	ResponseLoaderTestObject t;
	t.CheckAnalogs("C0 81 00 00 1E 02 00 00 01 01 04 00 01 09 00");
}

BOOST_AUTO_TEST_CASE(Group30Var3)
{
	ResponseLoaderTestObject t;
	t.CheckAnalogs("C0 81 00 00 1E 03 00 00 01 04 00 00 00 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group30Var4)
{
	ResponseLoaderTestObject t;
	t.CheckAnalogs("C0 81 00 00 1E 04 00 00 01 04 00 09 00");
}

BOOST_AUTO_TEST_CASE(Group30Var6)
{
	ResponseLoaderTestObject t;
	t.Load("C0 81 00 00 1E 06 00 00 00 01 20 74 85 2F C7 2B A2 C0");

	BOOST_REQUIRE_EQUAL(t.fdo.mAnalogMap.size(), 1);
	BOOST_REQUIRE_EQUAL(t.fdo.GetTotalCount(), 1);
}

BOOST_AUTO_TEST_CASE(Group40Var1)
{
	ResponseLoaderTestObject t;
	t.CheckSetpointStatii("C0 81 00 00 28 01 00 00 01 01 04 00 00 00 01 09 00 00 00");
}

BOOST_AUTO_TEST_CASE(Group40Var2)
{
	ResponseLoaderTestObject t;
	t.CheckSetpointStatii("C0 81 00 00 28 02 00 00 01 01 04 00 01 09 00");
}

BOOST_AUTO_TEST_SUITE_END() //end suite

