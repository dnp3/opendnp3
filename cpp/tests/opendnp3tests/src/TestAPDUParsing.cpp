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


#include "BufferHelpers.h"
#include "MockAPDUHeaderHandler.h"
#include "MeasurementComparisons.h"
#include "HexConversions.h"

#include <openpal/util/ToHex.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/parsing/APDUParser.h>
#include <opendnp3/app/parsing/APDUHeaderParser.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>
#include <opendnp3/app/IndexedValue.h>

#include <asiodnp3/ConsoleLogger.h>

#include <functional>

using namespace std;
using namespace openpal;
using namespace asiodnp3;
using namespace opendnp3;

void TestComplex(const std::string& hex, ParseResult expected, size_t numCalls, std::function<void (MockApduHeaderHandler&)> validate)
{
	HexSequence buffer(hex);
	MockApduHeaderHandler mock;	
	auto logger = mock.GetLogger();
	auto result = APDUParser::ParseAll(buffer.ToReadOnly(), mock, &logger);

	if (result != expected)
	{
		mock.Pop(ConsoleLogger::Instance());
	}

	REQUIRE((result == expected));
	REQUIRE(numCalls ==  mock.records.size());

	validate(mock);
}

void TestSimple(const std::string& hex, ParseResult expected, size_t numCalls)
{
	TestComplex(hex, expected, numCalls, [](MockApduHeaderHandler&) {});
}

std::string BufferToString(const ReadBufferView& buff)
{
	const uint8_t* pBuffer = buff;
	return std::string(reinterpret_cast<const char*>(pBuffer), buff.Size());
}

#define SUITE(name) "APDUParsingTestSuite - " name

TEST_CASE(SUITE("HeaderParsingEmptySring"))
{
	HexSequence buffer("");
	APDUHeader header;
	REQUIRE(!APDUHeaderParser::ParseRequest(buffer.ToReadOnly(), header));
}

TEST_CASE(SUITE("HeaderParsesReqeust"))
{
	HexSequence buffer("C0 02 AB CD");
	APDUHeader header;
	REQUIRE(APDUHeaderParser::ParseRequest(buffer.ToReadOnly(), header));
	REQUIRE(header.control.ToByte() == AppControlField(true, true, false, false, 0).ToByte());
	REQUIRE(header.function == FunctionCode::WRITE);
	REQUIRE("AB CD" ==  toHex(buffer.ToReadOnly().Skip(2)));
}

TEST_CASE(SUITE("ResponseLessThanFour"))
{
	HexSequence buffer("C0 02 01");
	APDUResponseHeader header;
	REQUIRE(!APDUHeaderParser::ParseResponse(buffer.ToReadOnly(), header));
}

TEST_CASE(SUITE("HeaderParsesResponse"))
{
	HexSequence buffer("C0 02 01 02 BE EF");
	APDUResponseHeader header;
	REQUIRE(APDUHeaderParser::ParseResponse(buffer.ToReadOnly(), header));
	REQUIRE(header.control.ToByte() == AppControlField(true, true, false, false, 0).ToByte());
	REQUIRE(header.function == FunctionCode::WRITE);
	REQUIRE(header.IIN == IINField(01, 02));
	REQUIRE("BE EF" ==  toHex(buffer.ToReadOnly().Skip(4)));
}


TEST_CASE(SUITE("EmptyStringParsesOK"))
{
	TestSimple("", ParseResult::OK, 0);
}

TEST_CASE(SUITE("NotEnoughData"))
{
	TestSimple("AB CD", ParseResult::NOT_ENOUGH_DATA_FOR_HEADER, 0);
}

TEST_CASE(SUITE("AllObjects"))
{
	// (2,2) all, (2,0) all
	TestComplex("02 02 06 02 00 06", ParseResult::OK, 2, [](MockApduHeaderHandler & mock)
	{
		REQUIRE((GroupVariation::Group2Var2 == mock.records[0].enumeration));
		REQUIRE((GroupVariation::Group2Var0 == mock.records[1].enumeration));
	});
}

TEST_CASE(SUITE("TestUnknownQualifier"))
{
	// (2,2) unknown qualifier 0xAB
	TestSimple("02 02 AB", ParseResult::UNKNOWN_QUALIFIER, 0);
}

TEST_CASE(SUITE("NotEnoughDataForObjects"))
{
	// 1 byte start/stop  1->4, 3 octests data
	TestSimple("01 02 00 01 04 FF FF FF", ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS, 0);
}

TEST_CASE(SUITE("Group1Var2Range"))
{
	// 1 byte start/stop  3->5, 3 octests data
	TestComplex("01 02 00 03 05 81 01 81", ParseResult::OK, 1, [](MockApduHeaderHandler & mock)
	{
		REQUIRE(3 ==  mock.staticBinaries.size());
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 3);
			REQUIRE((value == mock.staticBinaries[0]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(false), 4);
			REQUIRE((value == mock.staticBinaries[1]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 5);
			REQUIRE((value == mock.staticBinaries[2]));
		}
	});
}

TEST_CASE(SUITE("Group1Var2RangeAsReadRange"))
{
	HexSequence buffer("01 02 00 03 05");
	MockApduHeaderHandler mock;
	auto result = APDUParser::ParseAll(buffer.ToReadOnly(), mock, nullptr, ParserSettings::NoContents());
	REQUIRE((result == ParseResult::OK));
}


TEST_CASE(SUITE("Group1Var2CountOfZero"))
{
	// 1 byte count == 0, 0 octets data
	TestSimple("01 02 07 00", ParseResult::BAD_COUNT, 0);
}

TEST_CASE(SUITE("Group1Var2With2Headers"))
{		
	// 1 -> 2 & 2 -> 3
	TestComplex("01 02 00 01 01 81 01 02 00 02 03 81 81", ParseResult::OK, 2, [](MockApduHeaderHandler & mock)
	{
		REQUIRE(2 == mock.records.size());
	});
}


TEST_CASE(SUITE("Group1Var2Range1to3"))
{
	// 1 byte start / stop, 1 -> 3, 3 octets data
	TestComplex("01 02 00 01 03 81 01 81", ParseResult::OK, 1, [](MockApduHeaderHandler & mock)
	{
		REQUIRE(3 ==  mock.staticBinaries.size());
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 1);
			REQUIRE((value == mock.staticBinaries[0]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(false), 2);
			REQUIRE((value == mock.staticBinaries[1]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 3);
			REQUIRE((value == mock.staticBinaries[2]));
		}
	});
}

TEST_CASE(SUITE("Group1Var2Range16"))
{
	// 2 byte count == 1, 1 octet data
	TestComplex("01 02 01 03 00 03 00 81", ParseResult::OK, 1, [](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 ==  mock.staticBinaries.size());
		IndexedValue<Binary, uint16_t> value(Binary(true), 3);
		REQUIRE((value == mock.staticBinaries[0]));
	});
}

TEST_CASE(SUITE("Group1Var2AllCountQualifiers"))
{
	auto validator = [](MockApduHeaderHandler & mock)
	{
		REQUIRE(2 ==  mock.staticBinaries.size());
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 0);
			REQUIRE((value == mock.staticBinaries[0]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(false), 1);
			REQUIRE((value == mock.staticBinaries[1]));
		}
	};

	TestComplex("01 02 00 00 01 81 01", ParseResult::OK, 1, validator);
	TestComplex("01 02 01 00 00 01 00 81 01", ParseResult::OK, 1, validator);
	TestSimple("01 02 09 02 00 00 00 81 01", ParseResult::UNKNOWN_QUALIFIER, 0);
}

TEST_CASE(SUITE("FlippedRange"))
{
	// 1 byte start/stop w/ start > stop
	TestSimple("01 02 00 05 03", ParseResult::BAD_START_STOP, 0);
	TestSimple("01 02 00 FF 00", ParseResult::BAD_START_STOP, 0);
}

TEST_CASE(SUITE("TestUnreasonableRanges"))
{
	// 2 byte start/stop 0->65535, no data - the default max objects is very low (32768)
	TestSimple("01 02 01 00 00 FF FF", ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS, 0);
}

TEST_CASE(SUITE("ParserDoesNotAllowEmptyOctetStrings"))
{
	HexSequence buffer("6E 00 00 00 FF"); // 255 + 256
	MockApduHeaderHandler mock;

	auto result = APDUParser::ParseAll(buffer.ToReadOnly(), mock, nullptr);

	REQUIRE((result == ParseResult::INVALID_OBJECT));
	REQUIRE(0 == mock.records.size());
}

TEST_CASE(SUITE("Group1Var2CountWithIndexUInt8"))
{
	// 1 byte count, 1 byte index, index == 09, value = 0x81
	TestSimple("01 02 17 01 09 81", ParseResult::INVALID_OBJECT_QUALIFIER, 0);
}

TEST_CASE(SUITE("Group2Var1CountWithAllIndexSizes"))
{
	auto validator = [](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 ==  mock.eventBinaries.size());
		IndexedValue<Binary, uint16_t> value(Binary(true), 9);
		REQUIRE((value == mock.eventBinaries[0]));
	};

	// 1 byte count, 1 byte index, index == 09, value = 0x81
	TestComplex("02 01 17 01 09 81", ParseResult::OK, 1, validator);
	TestComplex("02 01 28 01 00 09 00 81", ParseResult::OK, 1, validator);
}

TEST_CASE(SUITE("Group1Var1ByRange"))
{
	// 1 byte start/stop 3 -> 6
	TestComplex("01 01 00 03 06 09", ParseResult::OK, 1, [](MockApduHeaderHandler & mock)
	{
		REQUIRE(4 ==  mock.staticBinaries.size());
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 3);
			REQUIRE((value == mock.staticBinaries[0]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(false), 4);
			REQUIRE((value == mock.staticBinaries[1]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(false), 5);
			REQUIRE((value == mock.staticBinaries[2]));
		}
		{
			IndexedValue<Binary, uint16_t> value(Binary(true), 6);
			REQUIRE((value == mock.staticBinaries[3]));
		}
	});
}

TEST_CASE(SUITE("Group12Var1WithIndexSizes"))
{
	auto hex = "0C 01 17 01 09 03 01 64 00 00 00 C8 00 00 00 00";

	auto validator = [&](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 ==  mock.crobRequests.size());
		ControlRelayOutputBlock crob(ControlCode::LATCH_ON, 1, 100, 200);
		IndexedValue<ControlRelayOutputBlock, uint16_t> value(crob, 9);
		REQUIRE((value == mock.crobRequests[0]));

		REQUIRE(1 == mock.records.size());
	};


	TestComplex(hex, ParseResult::OK, 1, validator);
}

TEST_CASE(SUITE("TestIINValue"))
{
	TestComplex("50 01 00 07 07 00", ParseResult::OK, 1, [&](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 ==  mock.iinBits.size());
		IndexedValue<IINValue, uint16_t> value(false, 7);
		REQUIRE((value.value.value == mock.iinBits[0].value.value));
	});
}

TEST_CASE(SUITE("Group60Var1Var2Var3Var4"))
{
	TestComplex("3C 01 06 3C 02 06 3C 03 06 3C 04 06", ParseResult::OK, 4, [&](MockApduHeaderHandler & mock)
	{
		REQUIRE(4 == mock.records.size());
		REQUIRE((GroupVariation::Group60Var1 == mock.records[0].enumeration));
		REQUIRE((GroupVariation::Group60Var2 == mock.records[1].enumeration));
		REQUIRE((GroupVariation::Group60Var3 == mock.records[2].enumeration));
		REQUIRE((GroupVariation::Group60Var4 == mock.records[3].enumeration));
	});
}

TEST_CASE(SUITE("TestDoubleBitCTO"))
{
	TestComplex("04 03 17 02 03 C1 07 00 05 41 09 00", ParseResult::OK, 1, [&](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 == mock.records.size());
		REQUIRE(2 ==  mock.eventDoubleBinaries.size());

		IndexedValue<DoubleBitBinary, uint16_t> event1(DoubleBitBinary(DoubleBit::INDETERMINATE, 0x01, 7), 3);
		IndexedValue<DoubleBitBinary, uint16_t> event2(DoubleBitBinary(DoubleBit::DETERMINED_OFF, 0x01, 9), 5);

		REQUIRE((event1 == mock.eventDoubleBinaries[0]));
		REQUIRE((event2 == mock.eventDoubleBinaries[1]));
	});
}

TEST_CASE(SUITE("OctetStrings"))
{
	// "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	// "world" == [0x77, 0x6F, 0x72, 0x6C, 0x64]

	// Group 111 (0x6F) Variation (length == 5), 1 byte count / 1 byte index (4), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	TestComplex("6F 05 17 02 04 68 65 6C 6C 6F FF 77 6F 72 6C 64", ParseResult::OK, 1, [&](MockApduHeaderHandler & mock)
	{
		REQUIRE(2 ==  mock.indexPrefixedOctets.size());
		REQUIRE(4 ==  mock.indexPrefixedOctets[0].index);
		REQUIRE("hello" ==  BufferToString(mock.indexPrefixedOctets[0].value.ToReadOnly()));
		REQUIRE(255 ==  mock.indexPrefixedOctets[1].index);
		REQUIRE("world" ==  BufferToString(mock.indexPrefixedOctets[1].value.ToReadOnly()));
	});

	// Group 110 (0x6E) Variation (length == 5), 1 byte start/stop (7), count of 1, "hello" == [0x68, 0x65, 0x6C, 0x6C, 0x6F]
	TestComplex("6E 05 00 07 08 68 65 6C 6C 6F 77 6F 72 6C 64", ParseResult::OK, 1, [&](MockApduHeaderHandler & mock)
	{
		REQUIRE(2 ==  mock.rangedOctets.size());
		REQUIRE(7 ==  mock.rangedOctets[0].index);
		REQUIRE("hello" ==  BufferToString(mock.rangedOctets[0].value.ToReadOnly()));
		REQUIRE(8 ==  mock.rangedOctets[1].index);
		REQUIRE("world" ==  BufferToString(mock.rangedOctets[1].value.ToReadOnly()));
	});
}

TEST_CASE(SUITE("Group13Var1CountWithAllIndexSizes"))
{
	auto validator = [](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 == mock.binaryCommandEvents.size());
		IndexedValue<BinaryCommandEvent, uint16_t> value(BinaryCommandEvent(true, CommandStatus::TIMEOUT), 9);
		REQUIRE((value == mock.binaryCommandEvents[0]));
	};

	// 1 byte count, 1 byte index, index == 09, value = 0x81
	TestComplex("0D 01 17 01 09 81", ParseResult::OK, 1, validator);
	TestComplex("0D 01 28 01 00 09 00 81", ParseResult::OK, 1, validator);
}

TEST_CASE(SUITE("Group13Var2CountWithAllIndexSizes"))
{
	auto validator = [](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 == mock.binaryCommandEvents.size());
		IndexedValue<BinaryCommandEvent, uint16_t> value(BinaryCommandEvent(true, CommandStatus::TIMEOUT, 1419802341000), 9);
		REQUIRE((value == mock.binaryCommandEvents[0]));
	};

	// 1 byte count, 1 byte index, index == 09, value = 0x81, time = 1419802341000 (014A92D06E88 in hex / 886ED0924A01 little endian)
	TestComplex("0D 02 17 01 09 81 88 6E D0 92 4A 01", ParseResult::OK, 1, validator);
	TestComplex("0D 02 28 01 00 09 00 81 88 6E D0 92 4A 01", ParseResult::OK, 1, validator);
}

TEST_CASE(SUITE("Group43Var1CountWithAllIndexSizes"))
{
	auto validator = [](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 == mock.analogCommandEvents.size());
		IndexedValue<AnalogCommandEvent, uint16_t> value(AnalogCommandEvent(50, CommandStatus::TIMEOUT), 9);
		REQUIRE((value == mock.analogCommandEvents[0]));
	};

	// 1 byte count, 1 byte index, index == 09, status = 0x01, value = 0x32
	TestComplex("2B 01 17 01 09 01 32 00 00 00", ParseResult::OK, 1, validator);
	TestComplex("2B 01 28 01 00 09 00 01 32 00 00 00", ParseResult::OK, 1, validator);
}

TEST_CASE(SUITE("Group43Var3CountWithAllIndexSizes"))
{
	auto validator = [](MockApduHeaderHandler & mock)
	{
		REQUIRE(1 == mock.analogCommandEvents.size());
		IndexedValue<AnalogCommandEvent, uint16_t> value(AnalogCommandEvent(50, CommandStatus::TIMEOUT, 1419802341000), 9);
		REQUIRE((value == mock.analogCommandEvents[0]));
	};

	// 1 byte count, 1 byte index, index == 09, status = 0x01, value = 0x32, time = 1419802341000 (014A92D06E88 in hex / 886ED0924A01 little endian)
	TestComplex("2B 03 17 01 09 01 32 00 00 00 88 6E D0 92 4A 01", ParseResult::OK, 1, validator);
	TestComplex("2B 03 28 01 00 09 00 01 32 00 00 00 88 6E D0 92 4A 01", ParseResult::OK, 1, validator);
}


