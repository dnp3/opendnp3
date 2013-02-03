
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
#include "TestHelpers.h"

#include <iostream>
#include <map>
#include <vector>
#include <stdarg.h>
#include <math.h>
#include <sstream>
#include <bitset>
#include <signal.h>
#include <vector>

#include <opendnp3/Util.h>
#include <opendnp3/Thread.h>

using namespace std;
using namespace opendnp3;


BOOST_AUTO_TEST_SUITE(MiscTest)

BOOST_AUTO_TEST_CASE(ToUpperCase)
{
	string test("lower case");
	toUpperCase(test);
	BOOST_REQUIRE_EQUAL(test, "LOWER CASE");

	string test2("UPPPER case");
	toUpperCase(test2);
	BOOST_REQUIRE_EQUAL(test2, "UPPPER CASE");

	string test3("123456789abcDEF");
	toUpperCase(test3);
	BOOST_REQUIRE_EQUAL(test3, "123456789ABCDEF");
}

BOOST_AUTO_TEST_SUITE_END()
