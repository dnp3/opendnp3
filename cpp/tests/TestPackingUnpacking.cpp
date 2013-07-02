
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
// you under the terms of the License.
//

#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/Types.h>
#include <opendnp3/Util.h>
#include <opendnp3/PackingUnpacking.h>

#include <memory>

using namespace std;
using namespace opendnp3;

template <class T>
bool TestReadWrite(typename T::Type aValue)
{
	uint8_t data[2 * T::Size];
	for(size_t i = 0; i < T::Size; ++i) {
		uint8_t* pos = data + i;
		T::Write(pos, aValue);
		typename T::Type r = T::Read(pos);
		if(aValue != r) return false;
	}
	return true;
}

template <class T>
bool TestReadWriteDouble(typename T::Type aValue)
{
	ByteStr data(2 * T::Size);

	for(size_t i = 0; i < T::Size; ++i) {
		uint8_t* pos = data + i;
		T::Write(pos, aValue);
		typename T::Type r = T::Read(pos);
		if(!FloatEqual(aValue, r)) return false;
	}
	return true;
}

template <class T>
bool TestFloatParsing(std::string aHex, typename T::Type aValue)
{
	HexSequence hs(aHex);
	size_t type_size = sizeof(typename T::Type);
	BOOST_REQUIRE_EQUAL(hs.Size(), type_size);

	ByteStr buff(2 * type_size);

	for(size_t i = 0; i < type_size; ++i) {
		memcpy(buff + i, hs, type_size);
		typename T::Type val = T::Read(buff + i);
		if(!FloatEqual(val, aValue)) return false;
	}

	return true;
}

BOOST_AUTO_TEST_SUITE(PackingUnpacking)
BOOST_AUTO_TEST_CASE(DoublePacking)
{
	BOOST_REQUIRE(TestFloatParsing<opendnp3::DoubleFloat>("20 74 85 2F C7 2B A2 C0", -2.3258890344E3));
	BOOST_REQUIRE(TestFloatParsing<opendnp3::DoubleFloat>("00 00 00 00 64 89 67 41", 12340000.0));
	BOOST_REQUIRE(TestFloatParsing<opendnp3::DoubleFloat>("00 00 00 00 00 00 34 C0", -20.0));
	BOOST_REQUIRE(TestFloatParsing<opendnp3::DoubleFloat>("8F 81 9C 95 2D F9 64 BB", -13.879E-23));
	BOOST_REQUIRE(TestFloatParsing<opendnp3::DoubleFloat>("00 00 00 00 00 00 59 40", 100.0));
}

BOOST_AUTO_TEST_CASE(SinglePacking)
{
	BOOST_REQUIRE(TestFloatParsing<opendnp3::SingleFloat>("20 4B 3C 4B", 12340000.0f));
	BOOST_REQUIRE(TestFloatParsing<opendnp3::SingleFloat>("6D C9 27 9B", -13.879E-23f));
	BOOST_REQUIRE(TestFloatParsing<opendnp3::SingleFloat>("00 00 A0 C1", -20.0));
}


BOOST_AUTO_TEST_CASE(DoubleFloat)
{
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::DoubleFloat>(0.0));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::DoubleFloat>(-100000));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::DoubleFloat>(-2.3258890344E3));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::DoubleFloat>(1E20));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::DoubleFloat>(100.0));
}

BOOST_AUTO_TEST_CASE(SingleFloat)
{
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::SingleFloat>(0.0f));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::SingleFloat>(-100000.0f));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::SingleFloat>(-2.3258890344E3f));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::SingleFloat>(1E20f));
	BOOST_REQUIRE(TestReadWriteDouble<opendnp3::SingleFloat>(100.0f));
}

BOOST_AUTO_TEST_CASE(UInt8)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt8>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt8>(123));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt8>(255));
}

BOOST_AUTO_TEST_CASE(UInt16LE)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt16LE>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt16LE>(123));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt16LE>(65535));
}

BOOST_AUTO_TEST_CASE(Int16LE)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::Int16LE>(-32768));
	BOOST_REQUIRE(TestReadWrite<opendnp3::Int16LE>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::Int16LE>(32767));
}

BOOST_AUTO_TEST_CASE(UInt32LE)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt32LE>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt32LE>(123));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt32LE>(4294967295UL));
}

BOOST_AUTO_TEST_CASE(Int32LE)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::Int32LE>(0x80000000));
	BOOST_REQUIRE(TestReadWrite<opendnp3::Int32LE>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::Int32LE>(0x7fffffff));
}

BOOST_AUTO_TEST_CASE(UInt48LE)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt48LE>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt48LE>(123));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt48LE>(281474976710655LL));
}

BOOST_AUTO_TEST_CASE(UInt64BE)
{
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt64BE>(0));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt64BE>(123));
	BOOST_REQUIRE(TestReadWrite<opendnp3::UInt64BE>(5000000000ULL));
}

BOOST_AUTO_TEST_SUITE_END()
