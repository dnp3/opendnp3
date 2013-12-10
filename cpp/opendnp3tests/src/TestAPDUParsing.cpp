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
#include "MeasurementComparisons.h"

#include <openpal/ToHex.h>

#include <opendnp3/APDUParser.h>

#include <functional>

using namespace std;
using namespace openpal;
using namespace opendnp3;

void TestComplex(const std::string& hex, APDUParser::Result expected, size_t numCalls, std::function<void (MockApduHeaderHandler&)> validate)
{
	HexSequence buffer(hex);
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseHeaders(buffer.ToReadOnly(), mock);

	BOOST_REQUIRE(result == expected);
	BOOST_REQUIRE_EQUAL(numCalls, mock.numRequests);

	validate(mock);
}

void TestSimple(const std::string& hex, APDUParser::Result expected, size_t numCalls)
{
	TestComplex(hex, expected, numCalls, [](MockApduHeaderHandler&) {});
}

BOOST_AUTO_TEST_SUITE(APDUParsingTestSuite)

BOOST_AUTO_TEST_CASE(EmptyStringParsesOK)
{
	TestSimple("", APDUParser::Result::OK, 0);
}

BOOST_AUTO_TEST_CASE(NotEnoughData)
{
	TestSimple("AB CD", APDUParser::Result::NOT_ENOUGH_DATA_FOR_HEADER, 0);
}

BOOST_AUTO_TEST_CASE(AllObjects)
{
	// (2,2) all, (2,0) all
	TestComplex("02 02 06 02 00 06", APDUParser::Result::OK, 2, [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(2, mock.allObjectRequests.size());
		BOOST_REQUIRE(GroupVariation::Group2Var2 == mock.allObjectRequests[0]);
		BOOST_REQUIRE(GroupVariation::Group2Var0 == mock.allObjectRequests[1]);
	});	
}

BOOST_AUTO_TEST_CASE(TestUnknownQualifier)
{
	// (2,2) unknown qualifier 0xAB
	TestSimple("02 02 AB", APDUParser::Result::UNKNOWN_QUALIFIER, 0);	
}

BOOST_AUTO_TEST_CASE(NotEnoughDataForObjects)
{
	// 1 byte start/stop  1->4, 3 octests data
	TestSimple("01 02 00 01 04 FF FF FF", APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS, 0);
}

BOOST_AUTO_TEST_CASE(Group1Var2Range)
{
	// 1 byte start/stop  3->5, 3 octests data
	TestComplex("01 02 00 03 05 81 01 81", APDUParser::Result::OK, 1, [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(3, mock.staticBinaries.size());
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 3) == mock.staticBinaries[0]);
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(false), 4) == mock.staticBinaries[1]);
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 5) == mock.staticBinaries[2]);
	});	
}

BOOST_AUTO_TEST_CASE(Group1Var2CountOfZero)
{
	// 1 byte count == 0, 0 octets data
	TestSimple("01 02 07 00", APDUParser::Result::COUNT_OF_ZERO, 0);
}

BOOST_AUTO_TEST_CASE(Group1Var2HeaderWrappers)
{
	
	TestComplex("01 02 07 01 81 01 02 07 02 81 81", APDUParser::Result::OK, 2, [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(2, mock.headers.size());
		auto hex1 = toHex(mock.headers[0]);
		auto hex2 = toHex(mock.headers[1]);

		BOOST_REQUIRE_EQUAL("01 02 07 01 81", hex1);
		BOOST_REQUIRE_EQUAL("01 02 07 02 81 81", hex2);
	});
}


BOOST_AUTO_TEST_CASE(Group1Var2Count8)
{
	// 1 byte count == 3, 3 octets data
	TestComplex("01 02 07 03 81 01 81", APDUParser::Result::OK, 1, [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(3, mock.staticBinaries.size());
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 0) == mock.staticBinaries[0]);
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(false), 1) == mock.staticBinaries[1]);
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 2) == mock.staticBinaries[2]);
	});	
}

BOOST_AUTO_TEST_CASE(Group1Var2Count16)
{
	// 2 byte count == 1, 1 octet data 
	TestComplex("01 02 08 01 00 81", APDUParser::Result::OK, 1, [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(1, mock.staticBinaries.size());
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 0) == mock.staticBinaries[0]);
	});
}

BOOST_AUTO_TEST_CASE(Group1Var2AllCountQualifiers)
{
	auto validator = [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(2, mock.staticBinaries.size());
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 0) == mock.staticBinaries[0]);
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(false), 1) == mock.staticBinaries[1]);
	};
	
	TestComplex("01 02 07 02 81 01", APDUParser::Result::OK, 1, validator);
	TestComplex("01 02 08 02 00 81 01", APDUParser::Result::OK, 1, validator);
	TestComplex("01 02 09 02 00 00 00 81 01", APDUParser::Result::OK, 1, validator);	
}

BOOST_AUTO_TEST_CASE(FlippedRange)
{
	// 1 byte start/stop w/ start > stop
	TestSimple("01 02 00 05 03", APDUParser::Result::BAD_START_STOP, 0);
	TestSimple("01 02 00 FF 00", APDUParser::Result::BAD_START_STOP, 0);
}

BOOST_AUTO_TEST_CASE(TestUnreasonableRanges)
{
	// 2 byte start/stop 0->65535, no data
	TestSimple("01 02 01 00 00 FF FF", APDUParser::Result::UNREASONABLE_OBJECT_COUNT, 0);

	// 4 byte start/stop 0->(2^32-1), no data
	TestSimple("01 02 02 00 00 00 00 FF FF FF FF", APDUParser::Result::UNREASONABLE_OBJECT_COUNT, 0);

	// 4 byte start/stop 0->65535, no data
	TestSimple("01 02 02 00 00 00 00 FF FF 00 00", APDUParser::Result::UNREASONABLE_OBJECT_COUNT, 0);
}

BOOST_AUTO_TEST_CASE(Group1Var2CountWithIndexUInt8)
{
	// 1 byte count, 1 byte index, index == 09, value = 0x81
	TestSimple("01 02 17 01 09 81", APDUParser::Result::ILLEGAL_OBJECT_QUALIFIER, 0);	
}

BOOST_AUTO_TEST_CASE(Group2Var1CountWithAllIndexSizes)
{	
	auto validator = [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(1, mock.eventBinaries.size());
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 9) == mock.eventBinaries[0]);		
	};

	// 1 byte count, 1 byte index, index == 09, value = 0x81
	TestComplex("02 01 17 01 09 81", APDUParser::Result::OK, 1, validator);
	TestComplex("02 01 28 01 00 09 00 81", APDUParser::Result::OK, 1, validator);
	TestComplex("02 01 39 01 00 00 00 09 00 00 00 81", APDUParser::Result::OK, 1, validator);
}

BOOST_AUTO_TEST_CASE(Group1Var1ByRange)
{
	// 1 byte start/stop 3 -> 6
	TestComplex("01 01 00 03 06 09", APDUParser::Result::OK, 1, [](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(4, mock.staticBinaries.size());
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 3) == mock.staticBinaries[0]);		
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(false), 4) == mock.staticBinaries[1]);	
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(false), 5) == mock.staticBinaries[2]);	
		BOOST_REQUIRE(IndexedValue<Binary>(Binary(true), 6) == mock.staticBinaries[3]);
	});
}

BOOST_AUTO_TEST_SUITE_END()

