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

#include <iostream>
#include <map>
#include <vector>
#include <stdarg.h>
#include <math.h>
#include <sstream>
#include <bitset>
#include <signal.h>

using namespace std;


#include <APL/Util.h>
#include <APL/Thread.h>

#include <vector>

using namespace apl;


BOOST_AUTO_TEST_SUITE(MiscTest)

BOOST_AUTO_TEST_CASE(ToUpperCase)
{
	string test("lower case");
	apl::toUpperCase(test);
	BOOST_REQUIRE_EQUAL(test, "LOWER CASE");

	string test2("UPPPER case");
	apl::toUpperCase(test2);
	BOOST_REQUIRE_EQUAL(test2, "UPPPER CASE");

	string test3("123456789abcDEF");
	apl::toUpperCase(test3);
	BOOST_REQUIRE_EQUAL(test3, "123456789ABCDEF");
}

BOOST_AUTO_TEST_SUITE_END()
