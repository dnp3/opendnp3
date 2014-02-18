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

#include <opendnp3/APDURequest.h>
#include <opendnp3/APDUResponse.h>

#include <openpal/ToHex.h>
#include <openpal/Serialization.h>
#include <openpal/StaticArray.h>

#include <opendnp3/objects/Group12.h>
#include <opendnp3/objects/Group20.h>
#include <opendnp3/objects/Group30.h>
#include <opendnp3/objects/Group50.h>
#include <opendnp3/objects/Group60.h>

#include <assert.h>

using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(APDUWritingTestSuite)

const uint32_t SIZE = 2048;
uint8_t fixedBuffer[SIZE];

APDURequest Request(FunctionCode code, uint32_t size = SIZE)
{
	assert(size <= SIZE);
	WriteBuffer buffer(fixedBuffer, size);
	APDURequest request(buffer);
	request.SetFunction(code);
	request.SetControl(AppControlField(true, true, false, false, 0));
	return request;
}

APDUResponse Response(uint32_t size = SIZE)
{
	assert(size <= SIZE);
	WriteBuffer buffer(fixedBuffer, size);
	APDUResponse response(buffer);
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, 0));
	response.SetIIN(IINField::Empty);
	return response;
}

BOOST_AUTO_TEST_CASE(AllObjectsAndRollback)
{	
	APDURequest request(Request(FunctionCode::READ));
	auto writer = request.GetWriter();
	writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS);
	writer.WriteHeader(Group60Var2::ID, QualifierCode::ALL_OBJECTS);
	writer.Mark();
	writer.WriteHeader(Group60Var3::ID, QualifierCode::ALL_OBJECTS);
	writer.WriteHeader(Group60Var4::ID, QualifierCode::ALL_OBJECTS);

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06 3C 02 06 3C 03 06 3C 04 06", toHex(request.ToReadOnly()));
	
	writer.Rollback();
	
	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06 3C 02 06", toHex(request.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(AllObjectsReturnsFalseWhenFull)
{	
	APDURequest request(Request(FunctionCode::READ, 6));
	auto writer = request.GetWriter();

	BOOST_REQUIRE(writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS));
	BOOST_REQUIRE(!writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS));

	BOOST_REQUIRE_EQUAL("C0 01 3C 01 06", toHex(request.ToReadOnly()));
}


BOOST_AUTO_TEST_CASE(RangeWriteIteratorStartStop)
{	
	APDUResponse response(Response());
	auto writer = response.GetWriter();
	
	auto iterator = writer.IterateOverRange<UInt8, Counter>(QualifierCode::UINT8_START_STOP, Group20Var6Serializer::Inst(), 2);
	
	BOOST_REQUIRE(iterator.Write(Counter(9)));	
	BOOST_REQUIRE(iterator.Write(Counter(7)));
	BOOST_REQUIRE(iterator.Complete());

	BOOST_REQUIRE_EQUAL("C0 81 00 00 14 06 00 02 03 09 00 07 00", toHex(response.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(EmptyHeadersWhenNotEnoughSpaceForSingleValue)
{	
	APDUResponse response(Response(8));
	auto writer = response.GetWriter();
	
	auto iterator = writer.IterateOverRange<UInt8, Counter>(QualifierCode::UINT8_START_STOP, Group20Var6Serializer::Inst(), 2);

	BOOST_REQUIRE(iterator.IsNull());

	BOOST_REQUIRE_EQUAL("C0 81 00 00", toHex(response.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(CountWriteIteratorAllowsCountOfZero)
{
	APDUResponse response(Response());
	auto writer = response.GetWriter();

	auto iter = writer.IterateOverCount<UInt16, Analog>(QualifierCode::UINT16_CNT, Group30Var1Serializer::Inst());
	BOOST_ASSERT(!iter.IsNull());
	BOOST_ASSERT(iter.Complete());

	BOOST_REQUIRE_EQUAL("C0 81 00 00 1E 01 08 00 00", toHex(response.ToReadOnly()));	

}

BOOST_AUTO_TEST_CASE(CountWriteIteratorFillsUpCorrectly)
{
	APDUResponse response(Response(15));
	auto writer = response.GetWriter();	

	auto iter = writer.IterateOverCount<UInt8, Analog>(QualifierCode::UINT8_CNT, Group30Var2Serializer::Inst());

	
	BOOST_REQUIRE(iter.Write(Analog(9, 0xFF)));	
	BOOST_REQUIRE(iter.Write(Analog(7, 0xFF)));
	BOOST_REQUIRE(!iter.Write(Analog(7, 0xFF))); //we're full
	BOOST_REQUIRE(iter.Complete());

	BOOST_REQUIRE_EQUAL("C0 81 00 00 1E 02 07 02 FF 09 00 FF 07 00", toHex(response.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(PrefixWriteIteratorWithSingleCROB)
{
	APDUResponse response(Response());
	auto writer = response.GetWriter();	

	auto iter = writer.IterateOverCountWithPrefix<UInt8, ControlRelayOutputBlock>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group12Var1Serializer::Inst());
	BOOST_ASSERT(!iter.IsNull());

	ControlRelayOutputBlock crob(ControlCode::LATCH_ON, 0x1F, 0x10, 0xAA, CommandStatus::LOCAL);
	
	BOOST_REQUIRE(iter.Write(crob, 0x21));
	BOOST_REQUIRE(iter.Complete());

	BOOST_REQUIRE_EQUAL("C0 81 00 00 0C 01 17 01 21 03 1F 10 00 00 00 AA 00 00 00 07", toHex(response.ToReadOnly()));	
}


BOOST_AUTO_TEST_CASE(SingleValueWithIndexCROB)
{
	APDURequest request(Request(FunctionCode::SELECT));
	auto writer = request.GetWriter();		

	ControlRelayOutputBlock crob(ControlCode::LATCH_ON, 0x1F, 0x10, 0xAA, CommandStatus::LOCAL);	
	
	BOOST_REQUIRE(writer.WriteSingleIndexedValue<UInt16>(QualifierCode::UINT16_CNT, Group12Var1Serializer::Inst(), crob, 0x21));	

	BOOST_REQUIRE_EQUAL("C0 03 0C 01 08 01 00 21 00 03 1F 10 00 00 00 AA 00 00 00 07", toHex(request.ToReadOnly()));	
}


BOOST_AUTO_TEST_CASE(WriteSingleValue)
{
	APDURequest request(Request(FunctionCode::WRITE));
	auto writer = request.GetWriter();	

	Group50Var1 obj = { 0x1234 };
	BOOST_REQUIRE(writer.WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, obj));

	BOOST_REQUIRE_EQUAL("C0 02 32 01 07 01 34 12 00 00 00 00", toHex(request.ToReadOnly()));
}


BOOST_AUTO_TEST_SUITE_END()
