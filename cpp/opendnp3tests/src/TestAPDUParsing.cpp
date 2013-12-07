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
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"
#include "MockAPDUHeaderHandler.h"

#include <opendnp3/APDUParser.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(APDUParsingSuite)

BOOST_AUTO_TEST_CASE(EmptyStringParsesOK)
{
	HexSequence hex("");
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseHeaders(hex.ToReadOnly(), mock);

	BOOST_REQUIRE(result == APDUParser::Result::OK);
	BOOST_REQUIRE_EQUAL(0, mock.numRequests);
}

BOOST_AUTO_TEST_CASE(NotEnoughData)
{
	HexSequence hex("AB CD");	
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseHeaders(hex.ToReadOnly(), mock);

	BOOST_REQUIRE(result == APDUParser::Result::NOT_ENOUGH_DATA_FOR_HEADER);	
}

BOOST_AUTO_TEST_CASE(AllObjects)
{
	HexSequence hex("02 02 06 02 00 06"); // (2,2) all, (2,0) all
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseHeaders(hex.ToReadOnly(), mock);

	BOOST_REQUIRE(result == APDUParser::Result::OK);
	BOOST_REQUIRE_EQUAL(2, mock.numRequests);
	BOOST_REQUIRE_EQUAL(2, mock.allObjectRequests.size());
	BOOST_REQUIRE(GroupVariation::Group2Var2 == mock.allObjectRequests[0]);
	BOOST_REQUIRE(GroupVariation::Group2Var0 == mock.allObjectRequests[1]);
}

BOOST_AUTO_TEST_CASE(TestUnknownQualifier)
{
	HexSequence hex("02 02 AB"); // (2,2) unknown qualifier 0xAB
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseHeaders(hex.ToReadOnly(), mock);

	BOOST_REQUIRE(result == APDUParser::Result::UNKNOWN_QUALIFIER);
	BOOST_REQUIRE_EQUAL(0, mock.numRequests);	
}

BOOST_AUTO_TEST_SUITE_END()

