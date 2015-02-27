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

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>

#include <openpal/container/DynamicBuffer.h>
#include <openpal/serialization/Parse.h>
#include <openpal/serialization/Format.h>
#include <openpal/serialization/Serialization.h>
#include <openpal/util/Comparisons.h>

#include <memory>

using namespace openpal;
using namespace testlib;

using namespace std;

template <class T>
bool TestReadWrite(T value)
{
	DynamicBuffer buffer(2 * sizeof(T));

	for (uint32_t i = 0; i < sizeof(T); ++i)
	{
		auto dest = buffer.GetWriteBufferView();
		dest.Advance(i);
		if (!Format::Write(dest, value))
		{
			return false;
		}

		auto written = buffer.ToReadOnly().Skip(i);		
		T readValue;
		if (!(Parse::Read(written, readValue) && value == readValue))
		{
			return false;
		}		
	}

	return true;
}

template <class T>
bool TestReadWriteFloat(T value)
{
	DynamicBuffer buffer(2 * sizeof(T));

	for (uint32_t i = 0; i < sizeof(T); ++i)
	{
		auto dest = buffer.GetWriteBufferView();
		dest.Advance(i);
		if (!Format::Write(dest, value))
		{
			return false;
		}

		auto written = buffer.ToReadOnly().Skip(i);
		T readValue;
		if (!(Parse::Read(written, readValue) && FloatEqual(value,readValue)))
		{
			return false;
		}
	}

	return true;
}

template <class T>
bool TestFloatParsing(std::string hex, typename T::Type value)
{
	HexSequence hs(hex);
	const uint32_t TYPE_SIZE = static_cast<uint32_t>(sizeof(typename T::Type));
	REQUIRE(hs.Size() == TYPE_SIZE);

	DynamicBuffer buffer(2 * TYPE_SIZE);

	for (uint32_t i = 0; i < TYPE_SIZE; ++i)
	{
		auto dest = buffer.GetWriteBufferView();
		dest.Advance(i);
		if (!Format::Write(dest, value))
		{
			return false;
		}
		auto written = buffer.ToReadOnly().Skip(i);

		typename T::Type val = 0;
		if (!(Parse::Read(written, val) && openpal::FloatEqual(val, value)))
		{
			return false;
		}		
	}

	return true;
}

#define SUITE(name) "PackingUnpacking - " name

TEST_CASE(SUITE("DoublePacking"))
{
	REQUIRE(TestFloatParsing<openpal::DoubleFloat>("20 74 85 2F C7 2B A2 C0", -2.3258890344E3));
	REQUIRE(TestFloatParsing<openpal::DoubleFloat>("00 00 00 00 64 89 67 41", 12340000.0));
	REQUIRE(TestFloatParsing<openpal::DoubleFloat>("00 00 00 00 00 00 34 C0", -20.0));
	REQUIRE(TestFloatParsing<openpal::DoubleFloat>("8F 81 9C 95 2D F9 64 BB", -13.879E-23));
	REQUIRE(TestFloatParsing<openpal::DoubleFloat>("00 00 00 00 00 00 59 40", 100.0));
}

TEST_CASE(SUITE("SinglePacking"))
{
	REQUIRE(TestFloatParsing<openpal::SingleFloat>("20 4B 3C 4B", 12340000.0f));
	REQUIRE(TestFloatParsing<openpal::SingleFloat>("6D C9 27 9B", -13.879E-23f));
	REQUIRE(TestFloatParsing<openpal::SingleFloat>("00 00 A0 C1", -20.0));
}


TEST_CASE(SUITE("DoubleFloat"))
{
	REQUIRE(TestReadWriteFloat<double>(0.0));
	REQUIRE(TestReadWriteFloat<double>(-100000));
	REQUIRE(TestReadWriteFloat<double>(-2.3258890344E3));
	REQUIRE(TestReadWriteFloat<double>(1E20));
	REQUIRE(TestReadWriteFloat<double>(100.0));
}

TEST_CASE(SUITE("SingleFloat"))
{
	REQUIRE(TestReadWriteFloat<float>(0.0f));
	REQUIRE(TestReadWriteFloat<float>(-100000.0f));
	REQUIRE(TestReadWriteFloat<float>(-2.3258890344E3f));
	REQUIRE(TestReadWriteFloat<float>(1E20f));
	REQUIRE(TestReadWriteFloat<float>(100.0f));
}

TEST_CASE(SUITE("UInt8"))
{
	REQUIRE(TestReadWrite<uint8_t>(0));
	REQUIRE(TestReadWrite<uint8_t>(123));
	REQUIRE(TestReadWrite<uint8_t>(255));
}

TEST_CASE(SUITE("UInt16"))
{
	REQUIRE(TestReadWrite<uint16_t>(0));
	REQUIRE(TestReadWrite<uint16_t>(123));
	REQUIRE(TestReadWrite<uint16_t>(65535));
}

TEST_CASE(SUITE("Int16"))
{
	REQUIRE(TestReadWrite<int16_t>(-32768));
	REQUIRE(TestReadWrite<int16_t>(0));
	REQUIRE(TestReadWrite<int16_t>(32767));
}

TEST_CASE(SUITE("UInt32"))
{
	REQUIRE(TestReadWrite<uint32_t>(0));
	REQUIRE(TestReadWrite<uint32_t>(123));
	REQUIRE(TestReadWrite<uint32_t>(4294967295UL));
}

TEST_CASE(SUITE("Int32"))
{
	REQUIRE(TestReadWrite<int32_t>(0x80000000));
	REQUIRE(TestReadWrite<int32_t>(0));
	REQUIRE(TestReadWrite<int32_t>(0x7fffffff));
}

TEST_CASE(SUITE("UInt48LE"))
{	
	REQUIRE(TestReadWrite<UInt48Type>(UInt48Type(0)));
	REQUIRE(TestReadWrite<UInt48Type>(UInt48Type(123)));
	REQUIRE(TestReadWrite<UInt48Type>(UInt48Type(281474976710655ULL)));
}

TEST_CASE(SUITE("ParseMany"))
{
	HexSequence hex("FF AB BA 01 00 00 00 CC");
		
	uint8_t first = 0; 
	uint16_t second = 0;
	uint32_t third = 0;	

	{
		auto input = hex.ToReadOnly();		
		REQUIRE(Parse::Many(input, first, second, third));
		REQUIRE(first == 255);
		REQUIRE(second == 0xBAAB);
		REQUIRE(third == 1);
		REQUIRE(input.Size() == 1); // 1 byte remaining
	}

	{
		auto input = hex.ToReadOnly().Skip(2);		
		REQUIRE_FALSE(Parse::Many(input, first, second, third));
	}
}


TEST_CASE(SUITE("FormatMany"))
{

	uint8_t first = 255;
	uint16_t second = 0xBAAB;
	uint32_t third = 1;

	const uint32_t SIZE = 7;

	DynamicBuffer output(SIZE + 3);

	{
		auto dest = output.GetWriteBufferView();
		REQUIRE(Format::Many(dest, first, second, third));
		REQUIRE(dest.Size() == (output.Size() - SIZE));
		auto written = ToHex(output.ToReadOnly().Take(SIZE));
		REQUIRE(written == "FF AB BA 01 00 00 00");
	}

	{
		auto dest = output.GetWriteBufferView(SIZE - 1);
		REQUIRE_FALSE(Format::Many(dest, first, second, third));
	}
}