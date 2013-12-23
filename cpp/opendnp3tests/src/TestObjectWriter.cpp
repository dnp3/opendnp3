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

#include <opendnp3/objects/Group12.h>
#include <opendnp3/objects/Group20.h>
#include <opendnp3/objects/Group30.h>
#include <opendnp3/objects/Group50.h>
#include <opendnp3/objects/Group60.h>

using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(ObjectWriterTestSuite)

uint8_t fixedBuffer[2048];
const WriteBuffer buffer(fixedBuffer, 2048);

BOOST_AUTO_TEST_CASE(AllObjectsAndRollback)
{	
	ObjectWriter writer(buffer);
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
	
	auto iterator = writer.IterateOverRange<UInt8,Group20Var6>(QualifierCode::UINT8_START_STOP, 2);

	Group20Var6 value = { 9 };
	BOOST_REQUIRE(iterator.Write(value));
	value.count = 7;
	BOOST_REQUIRE(iterator.Write(value));
	BOOST_REQUIRE(iterator.Complete());

	BOOST_REQUIRE_EQUAL("14 06 00 02 03 09 00 07 00", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(EmptyHeadersWhenNotEnoughSpaceForSingleValue)
{	
	ObjectWriter writer(buffer.Truncate(6));  //requires 7
	
	auto iterator = writer.IterateOverRange<UInt8,Group20Var6>(QualifierCode::UINT8_START_STOP, 2);

	BOOST_REQUIRE(iterator.IsNull());

	BOOST_REQUIRE_EQUAL("", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(CountWriteIteratorAllowsCountOfZero)
{
	ObjectWriter writer(buffer);

	auto iter = writer.IterateOverCount<UInt16, Group30Var1>(QualifierCode::UINT16_CNT);
	BOOST_ASSERT(!iter.IsNull());
	BOOST_ASSERT(iter.Complete());

	BOOST_REQUIRE_EQUAL("1E 01 08 00 00", toHex(writer.ToReadOnly()));	

}

BOOST_AUTO_TEST_CASE(CountWriteIteratorFillsUpCorrectly)
{
	ObjectWriter writer(buffer.Truncate(11));

	auto iter = writer.IterateOverCount<UInt8, Group30Var2>(QualifierCode::UINT8_CNT);

	Group30Var2 obj = { 0xFF, 9 };
	BOOST_REQUIRE(iter.Write(obj));
	obj.value = 7;
	BOOST_REQUIRE(iter.Write(obj));
	BOOST_REQUIRE(!iter.Write(obj)); //we're full
	BOOST_REQUIRE(iter.Complete());

	BOOST_REQUIRE_EQUAL("1E 02 07 02 FF 09 00 FF 07 00", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(PrefixWriteIteratorWithSingleCROB)
{
	ObjectWriter writer(buffer);

	auto iter = writer.IterateOverCountWithPrefix<UInt8, Group12Var1>(QualifierCode::UINT8_CNT_UINT8_INDEX);
	BOOST_ASSERT(!iter.IsNull());

	Group12Var1 obj;
	obj.code = ControlCode::LATCH_ON;
	obj.count = 0x1F;
	obj.onTime = 0x10;
	obj.offTime = 0xAA;
	obj.status = CommandStatus::LOCAL;
	
	BOOST_REQUIRE(iter.Write(obj, 0x21));
	BOOST_REQUIRE(iter.Complete());

	BOOST_REQUIRE_EQUAL("0C 01 17 01 21 03 1F 10 00 00 00 AA 00 00 00 07", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(WriteSingleValue)
{
	ObjectWriter writer(buffer);
	Group50Var1 obj = { 0x1234 };
	BOOST_REQUIRE(writer.WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, obj));

	BOOST_REQUIRE_EQUAL("32 01 07 01 34 12 00 00 00 00", toHex(writer.ToReadOnly()));
}


BOOST_AUTO_TEST_SUITE_END()
