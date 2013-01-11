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

#include <DNP3/LinkRoute.h>
#include <APLTestTools/TestHelpers.h>

#include <map>

using namespace std;
using namespace apl;
using namespace apl::dnp;

BOOST_AUTO_TEST_SUITE(LinkRouteSuite)

BOOST_AUTO_TEST_CASE(ComparisonFunctor)
{
	LinkRoute lr1(1, 2);
	LinkRoute lr2(1, 3);
	LinkRoute lr3(2, 1);

	LinkRoute::LessThan lt;

	BOOST_REQUIRE(lt(lr1, lr3));
	BOOST_REQUIRE_FALSE(lt(lr3, lr1));

	BOOST_REQUIRE(lt(lr1, lr2));
	BOOST_REQUIRE_FALSE(lt(lr2, lr1));

	BOOST_REQUIRE_FALSE(lt(lr1, lr1));
}

BOOST_AUTO_TEST_CASE(UseWithStlMap)
{
	typedef map<LinkRoute, int, LinkRoute::LessThan> RouteMap;

	RouteMap rm;
	rm[LinkRoute(4, 1)] = 13;
	rm[LinkRoute(4, 1)] = 12;
	BOOST_CHECK_EQUAL(12, rm[LinkRoute(4, 1)]);

	{
		RouteMap::iterator i = rm.find(LinkRoute(1, 4));
		BOOST_CHECK(rm.end() == i);
	}
	{
		RouteMap::iterator i = rm.find(LinkRoute(4, 1));
		BOOST_REQUIRE_EQUAL(4, i->first.remote);
		BOOST_REQUIRE_EQUAL(1, i->first.local);
		BOOST_REQUIRE_EQUAL(12, i->second);
	}
}

BOOST_AUTO_TEST_SUITE_END()
