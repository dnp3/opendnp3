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
#include <opendnp3/APDUHeaderParser.h>
#include <opendnp3/ControlRelayOutputBlock.h>

#include <functional>

using namespace std;
using namespace openpal;
using namespace opendnp3;

void TestComplex(const std::string& hex, APDUParser::Result expected, size_t numCalls, std::function<void (MockApduHeaderHandler&)> validate)
{
	HexSequence buffer(hex);
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseTwoPass(buffer.ToReadOnly(), &mock);

	BOOST_REQUIRE(result == expected);
	BOOST_REQUIRE_EQUAL(numCalls, mock.groupVariations.size());

	validate(mock);
}

void TestSimple(const std::string& hex, APDUParser::Result expected, size_t numCalls)
{
	TestComplex(hex, expected, numCalls, [](MockApduHeaderHandler&) {});
}

std::string BufferToString(const ReadOnlyBuffer& buff)
{
	const uint8_t* pBuffer = buff;	
	return std::string(reinterpret_cast<const char*>(pBuffer), buff.Size());
}

BOOST_AUTO_TEST_SUITE(APDUParsingTestSuite)

BOOST_AUTO_TEST_CASE(HeaderParsingEmptySring)
{
	HexSequence buffer("");
	APDURecord rec;
	BOOST_REQUIRE(APDUHeaderParser::Result::NOT_ENOUGH_DATA_FOR_HEADER == APDUHeaderParser::ParseRequest(buffer.ToReadOnly(), rec));
}

BOOST_AUTO_TEST_CASE(HeaderParsesReqeust)
{
	HexSequence buffer("C0 02 AB CD");
	APDURecord rec;
	BOOST_REQUIRE(APDUHeaderParser::Result::OK == APDUHeaderParser::ParseRequest(buffer.ToReadOnly(), rec));
	BOOST_REQUIRE_EQUAL(rec.control.ToByte(), AppControlField(true, true, false, false, 0).ToByte());
	BOOST_REQUIRE(rec.function == FunctionCode::WRITE);
	BOOST_REQUIRE_EQUAL("AB CD", toHex(rec.objects));
}

BOOST_AUTO_TEST_CASE(ResponseLessThanFour)
{
	HexSequence buffer("C0 02 01");
	APDUResponseRecord rec;
	BOOST_REQUIRE(APDUHeaderParser::Result::NOT_ENOUGH_DATA_FOR_HEADER == APDUHeaderParser::ParseResponse(buffer.ToReadOnly(), rec));	
}

BOOST_AUTO_TEST_CASE(HeaderParsesResponse)
{
	HexSequence buffer("C0 02 01 02 BE EF");
	APDUResponseRecord rec;
	BOOST_REQUIRE(APDUHeaderParser::Result::OK == APDUHeaderParser::ParseResponse(buffer.ToReadOnly(), rec));
	BOOST_REQUIRE_EQUAL(rec.control.ToByte(), AppControlField(true, true, false, false, 0).ToByte());
	BOOST_REQUIRE(rec.function == FunctionCode::WRITE);
	BOOST_REQUIRE(rec.IIN == IINField(01, 02));
	BOOST_REQUIRE_EQUAL("BE EF", toHex(rec.objects));
}


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
		BOOST_REQUIRE(GroupVariation::Group2Var2 == mock.groupVariations[0]);
		BOOST_REQUIRE(GroupVariation::Group2Var0 == mock.groupVariations[1]);
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
	TestSimple("01 02 09 02 00 00 00 81 01", APDUParser::Result::UNKNOWN_QUALIFIER, 0);
}

BOOST_AUTO_TEST_CASE(FlippedRange)
{
	// 1 byte start/stop w/ start > stop
	TestSimple("01 02 00 05 03", APDUParser::Result::BAD_START_STOP, 0);
	TestSimple("01 02 00 FF 00", APDUParser::Result::BAD_START_STOP, 0);
}

BOOST_AUTO_TEST_CASE(TestUnreasonableRanges)
{
	// 2 byte start/stop 0->65535, no data - the default max objects is very low (32768)
	TestSimple("01 02 01 00 00 FF FF", APDUParser::Result::UNREASONABLE_OBJECT_COUNT, 0);	
}

BOOST_AUTO_TEST_CASE(MaxCountAccumlatesOverHeaders)
{
	HexSequence buffer("01 02 00 01 02 81 81 01 02 00 01 02 81 81"); // total of four objects
	MockApduHeaderHandler mock;

	APDUParser::Context ctx(3); //maximum of the 3 objects	
	auto result = APDUParser::ParseTwoPass(buffer.ToReadOnly(), &mock, ctx);

	BOOST_REQUIRE(result == APDUParser::Result::UNREASONABLE_OBJECT_COUNT);
	BOOST_REQUIRE_EQUAL(0, mock.groupVariations.size()); // 0 calls because parser rejects bad count on first pass
}

BOOST_AUTO_TEST_CASE(ParserHandlesSmallNumberOfEmptyOctetStringsWithDefaultSettings)
{
	HexSequence buffer("6E 00 08 FF 01"); // 255 + 256
	MockApduHeaderHandler mock;

	auto result = APDUParser::ParseTwoPass(buffer.ToReadOnly(), &mock);

	BOOST_REQUIRE(result == APDUParser::Result::OK);
	BOOST_REQUIRE_EQUAL(1, mock.groupVariations.size());
}

BOOST_AUTO_TEST_CASE(ParserRejectsLargeEmptyOctetStringsWithDefaultSettings)
{
	HexSequence buffer("6E 00 08 FF FF"); // count of 65535 empty strings
	MockApduHeaderHandler mock;

	auto result = APDUParser::ParseTwoPass(buffer.ToReadOnly(), &mock);

	BOOST_REQUIRE(result == APDUParser::Result::UNREASONABLE_OBJECT_COUNT);
	BOOST_REQUIRE_EQUAL(0, mock.groupVariations.size());
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

BOOST_AUTO_TEST_CASE(Group12Var1WithIndexSizes)
{	
	auto hex = "0C 01 17 01 09 03 01 64 00 00 00 C8 00 00 00 00";

	auto validator = [&](MockApduHeaderHandler& mock) {		
		BOOST_REQUIRE_EQUAL(1, mock.crobRequests.size());
		ControlRelayOutputBlock crob(ControlCode::LATCH_ON, 1, 100, 200);
		BOOST_REQUIRE(IndexedValue<ControlRelayOutputBlock>(crob, 9) == mock.crobRequests[0]);

		BOOST_REQUIRE_EQUAL(1, mock.headers.size());		
		BOOST_REQUIRE_EQUAL(hex, toHex(mock.headers[0]));
	};

	
	TestComplex(hex, APDUParser::Result::OK, 1, validator);
}

BOOST_AUTO_TEST_CASE(TestIINValue)
{
	TestComplex("50 01 00 07 07 00", APDUParser::Result::OK, 1, [&](MockApduHeaderHandler& mock) {
		BOOST_REQUIRE_EQUAL(1, mock.iinBits.size());		
		BOOST_REQUIRE(IndexedValue<bool>(false, 7) == mock.iinBits[0]);	
	});
}

BOOST_AUTO_TEST_CASE(Group60Var1Var2Var3Var4)
{
	TestComplex("3C 01 06 3C 02 06 3C 03 06 3C 04 06", APDUParser::Result::OK, 4, [&](MockApduHeaderHandler& mock) {
		BOOST_REQUIRE_EQUAL(4, mock.groupVariations.size());		
		BOOST_REQUIRE(GroupVariation::Group60Var1 == mock.groupVariations[0]);
		BOOST_REQUIRE(GroupVariation::Group60Var2 == mock.groupVariations[1]);
		BOOST_REQUIRE(GroupVariation::Group60Var3 == mock.groupVariations[2]);
		BOOST_REQUIRE(GroupVariation::Group60Var4 == mock.groupVariations[3]);
	});
}

BOOST_AUTO_TEST_CASE(OctetStrings)
{
	// "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	// "world" == [0x77, 0x6F, 0x72, 0x6C, 0x64]

	// Group 111 (0x6F) Variation (length == 5), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	TestComplex("6F 05 17 02 04 68 65 6C 6C 6F FF 77 6F 72 6C 64", APDUParser::Result::OK, 1, [&](MockApduHeaderHandler& mock) {
		BOOST_REQUIRE_EQUAL(2, mock.indexPrefixedOctets.size());		
		BOOST_REQUIRE_EQUAL(4, mock.indexPrefixedOctets[0].index);
		BOOST_REQUIRE_EQUAL("hello", BufferToString(mock.indexPrefixedOctets[0].value.ToReadOnly()));
		BOOST_REQUIRE_EQUAL(255, mock.indexPrefixedOctets[1].index);
		BOOST_REQUIRE_EQUAL("world", BufferToString(mock.indexPrefixedOctets[1].value.ToReadOnly()));
	});

	// Group 110 (0x6E) Variation (length == 5), 1 byte start/stop (7), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	TestComplex("6E 05 00 07 08 68 65 6C 6C 6F 77 6F 72 6C 64", APDUParser::Result::OK, 1, [&](MockApduHeaderHandler& mock) {
		BOOST_REQUIRE_EQUAL(2, mock.rangedOctets.size());		
		BOOST_REQUIRE_EQUAL(7, mock.rangedOctets[0].index);
		BOOST_REQUIRE_EQUAL("hello", BufferToString(mock.rangedOctets[0].value.ToReadOnly()));
		BOOST_REQUIRE_EQUAL(8, mock.rangedOctets[1].index);
		BOOST_REQUIRE_EQUAL("world", BufferToString(mock.rangedOctets[1].value.ToReadOnly()));		
	});
}

BOOST_AUTO_TEST_SUITE_END()
