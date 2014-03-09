/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include <catch.hpp>

#include <opendnp3/link/LinkRoute.h>



#include <map>

using namespace std;
using namespace opendnp3;

#define SUITE(name) "LinkRouteSuite - " name

TEST_CASE(SUITE("ComparisonFunctor"))
{
	LinkRoute lr1(1, 2);
	LinkRoute lr2(1, 3);
	LinkRoute lr3(2, 1);

	LinkRoute::LessThan lt;

	REQUIRE(lt(lr1, lr3));
	REQUIRE_FALSE(lt(lr3, lr1));

	REQUIRE(lt(lr1, lr2));
	REQUIRE_FALSE(lt(lr2, lr1));

	REQUIRE_FALSE(lt(lr1, lr1));
}

TEST_CASE(SUITE("UseWithStlMap"))
{
	typedef map<LinkRoute, int, LinkRoute::LessThan> RouteMap;

	RouteMap rm;
	rm[LinkRoute(4, 1)] = 13;
	rm[LinkRoute(4, 1)] = 12;
	REQUIRE(12 == rm[LinkRoute(4, 1)]);

	{
		RouteMap::iterator i = rm.find(LinkRoute(1, 4));
		REQUIRE(rm.end() == i);
	}
	{
		RouteMap::iterator i = rm.find(LinkRoute(4, 1));
		REQUIRE(4 ==  i->first.remote);
		REQUIRE(1 ==  i->first.local);
		REQUIRE(12 ==  i->second);
	}
}


