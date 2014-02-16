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

#include <opendnp3/ObjectWriter.h>

#include <openpal/ToHex.h>
#include <openpal/Serialization.h>
#include <openpal/StaticArray.h>

#include <opendnp3/objects/Group12.h>
#include <opendnp3/objects/Group20.h>
#include <opendnp3/objects/Group30.h>
#include <opendnp3/objects/Group50.h>
#include <opendnp3/objects/Group60.h>

using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(ObjectWriterTestSuite)

const uint32_t SIZE = 2048;
uint8_t fixedBuffer[SIZE];

BOOST_AUTO_TEST_CASE(AllObjectsAndRollback)
{	
	WriteBuffer buffer(fixedBuffer, SIZE);
	ObjectWriter writer(&buffer);
	writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS);
	writer.WriteHeader(Group60Var2::ID, QualifierCode::ALL_OBJECTS);
	writer.Mark();
	writer.WriteHeader(Group60Var3::ID, QualifierCode::ALL_OBJECTS);
	writer.WriteHeader(Group60Var4::ID, QualifierCode::ALL_OBJECTS);

	BOOST_REQUIRE_EQUAL("3C 01 06 3C 02 06 3C 03 06 3C 04 06", toHex(writer.ToReadOnly()));
	
	writer.Rollback();
	
	BOOST_REQUIRE_EQUAL("3C 01 06 3C 02 06", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(AllObjectsReturnsFalseWhenFull)
{	
	ObjectWriter writer(buffer.Truncate(4));

	BOOST_REQUIRE(writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS));
	BOOST_REQUIRE(!writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS));

	BOOST_REQUIRE_EQUAL("3C 01 06", toHex(writer.ToReadOnly()));
}

BOOST_AUTO_TEST_CASE(RangeWriteIteratorStartStop)
{	
	ObjectWriter writer(buffer);
	
	auto iterator = writer.IterateOverRange<UInt8, Counter>(QualifierCode::UINT8_START_STOP, Group20Var6Serializer::Inst(), 2);
	
	BOOST_REQUIRE(iterator.Write(Counter(9)));	
	BOOST_REQUIRE(iterator.Write(Counter(7)));
	BOOST_REQUIRE(iterator.Complete());

	BOOST_REQUIRE_EQUAL("14 06 00 02 03 09 00 07 00", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(EmptyHeadersWhenNotEnoughSpaceForSingleValue)
{	
	ObjectWriter writer(buffer.Truncate(6));  //requires 7
	
	auto iterator = writer.IterateOverRange<UInt8, Counter>(QualifierCode::UINT8_START_STOP, Group20Var6Serializer::Inst(), 2);

	BOOST_REQUIRE(iterator.IsNull());

	BOOST_REQUIRE_EQUAL("", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(CountWriteIteratorAllowsCountOfZero)
{
	ObjectWriter writer(buffer);

	auto iter = writer.IterateOverCount<UInt16, Analog>(QualifierCode::UINT16_CNT, Group30Var1Serializer::Inst());
	BOOST_ASSERT(!iter.IsNull());
	BOOST_ASSERT(iter.Complete());

	BOOST_REQUIRE_EQUAL("1E 01 08 00 00", toHex(writer.ToReadOnly()));	

}

BOOST_AUTO_TEST_CASE(CountWriteIteratorFillsUpCorrectly)
{
	ObjectWriter writer(buffer.Truncate(11));

	auto iter = writer.IterateOverCount<UInt8, Analog>(QualifierCode::UINT8_CNT, Group30Var2Serializer::Inst());

	
	BOOST_REQUIRE(iter.Write(Analog(9, 0xFF)));	
	BOOST_REQUIRE(iter.Write(Analog(7, 0xFF)));
	BOOST_REQUIRE(!iter.Write(Analog(7, 0xFF))); //we're full
	BOOST_REQUIRE(iter.Complete());

	BOOST_REQUIRE_EQUAL("1E 02 07 02 FF 09 00 FF 07 00", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(PrefixWriteIteratorWithSingleCROB)
{
	ObjectWriter writer(buffer);

	auto iter = writer.IterateOverCountWithPrefix<UInt8, ControlRelayOutputBlock>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group12Var1Serializer::Inst());
	BOOST_ASSERT(!iter.IsNull());

	ControlRelayOutputBlock crob(ControlCode::LATCH_ON, 0x1F, 0x10, 0xAA, CommandStatus::LOCAL);
	
	BOOST_REQUIRE(iter.Write(crob, 0x21));
	BOOST_REQUIRE(iter.Complete());

	BOOST_REQUIRE_EQUAL("0C 01 17 01 21 03 1F 10 00 00 00 AA 00 00 00 07", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(SingleValueWithIndexCROB)
{
	ObjectWriter writer(buffer);	

	ControlRelayOutputBlock crob(ControlCode::LATCH_ON, 0x1F, 0x10, 0xAA, CommandStatus::LOCAL);	
	
	BOOST_REQUIRE(writer.WriteSingleIndexedValue<UInt16>(QualifierCode::UINT16_CNT, Group12Var1Serializer::Inst(), crob, 0x21));	

	BOOST_REQUIRE_EQUAL("0C 01 08 01 00 21 00 03 1F 10 00 00 00 AA 00 00 00 07", toHex(writer.ToReadOnly()));	
}


BOOST_AUTO_TEST_CASE(WriteSingleValue)
{
	ObjectWriter writer(buffer);
	Group50Var1 obj = { 0x1234 };
	BOOST_REQUIRE(writer.WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, obj));

	BOOST_REQUIRE_EQUAL("32 01 07 01 34 12 00 00 00 00", toHex(writer.ToReadOnly()));
}


BOOST_AUTO_TEST_SUITE_END()
