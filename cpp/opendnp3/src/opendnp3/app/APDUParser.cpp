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

#include "APDUParser.h"

#include "opendnp3/gen/QualifierCode.h"
#include "opendnp3/app/GroupVariation.h"
#include "opendnp3/app/MeasurementFactory.h"

using namespace openpal;

namespace opendnp3
{

APDUParser::Result APDUParser::ParseTwoPass(const openpal::ReadOnlyBuffer& buffer, IAPDUHandler* pHandler, Context context)
{
	if(pHandler)
	{
		// do a single pass without the callbacks to validate that the message is well formed
		auto result = ParseSinglePass(buffer, nullptr, context); 
		return (result == Result::OK) ? ParseSinglePass(buffer, pHandler, context) : result;		
	}
	else
	{
		return ParseSinglePass(buffer, pHandler, context);
	}	
}

APDUParser::Result APDUParser::ParseSinglePass(const openpal::ReadOnlyBuffer& buffer, IAPDUHandler* pHandler, Context context)
{
	ReadOnlyBuffer copy(buffer);
	while(copy.Size() > 0)
	{
		auto result = ParseHeader(copy, context, pHandler);
		if(result != Result::OK) return result;
	}
	return Result::OK;
}

APDUParser::Result APDUParser::ParseHeader(ReadOnlyBuffer& buffer, Context& context, IAPDUHandler* pHandler)
{
	if(buffer.Size() < 3) return Result::NOT_ENOUGH_DATA_FOR_HEADER; 
	else {	
		// record an immutable start position with 3 bytes for group, variation, qualifier
		//const HeaderRecord record(buffer, 3); 
		uint8_t group = UInt8::ReadBuffer(buffer);		
		uint8_t variation = UInt8::ReadBuffer(buffer);		
		auto enumeration = GroupVariationRecord::GetEnum(group, variation);
		GroupVariationRecord gvRecord = {enumeration, group, variation};
		if(gvRecord.enumeration == GroupVariation::UNKNOWN) return Result::UNKNOWN_OBJECT;
		QualifierCode qualifier = QualifierCodeFromType(UInt8::ReadBuffer(buffer));
		switch(qualifier)
		{
			case(QualifierCode::ALL_OBJECTS):
			{
				if(pHandler) pHandler->AllObjects(gvRecord.enumeration);				
				return Result::OK;
			}

			case(QualifierCode::UINT8_CNT):
				return ParseCountHeader<UInt8>(buffer, context, qualifier, gvRecord, pHandler);

			case(QualifierCode::UINT16_CNT):
				return ParseCountHeader<UInt16>(buffer, context, qualifier, gvRecord, pHandler);
			
			case(QualifierCode::UINT8_START_STOP):
				return ParseRangeHeader<UInt8, uint16_t>(buffer, context, qualifier, gvRecord, pHandler);

			case(QualifierCode::UINT16_START_STOP):
				return ParseRangeHeader<UInt16, uint32_t>(buffer, context, qualifier, gvRecord, pHandler);
					
			case(QualifierCode::UINT8_CNT_UINT8_INDEX):
				return ParseIndexPrefixHeader<UInt8>(buffer, context, qualifier, gvRecord, pHandler);
			
			case(QualifierCode::UINT16_CNT_UINT16_INDEX):
				return ParseIndexPrefixHeader<UInt16>(buffer, context, qualifier, gvRecord, pHandler);
				
			default:
				return Result::UNKNOWN_QUALIFIER;
		}
	}
}

IndexedValue<Binary, uint16_t> APDUParser::BoolToBinary(const IndexedValue<bool, uint16_t>& v)
{
	return IndexedValue<Binary, uint16_t>(Binary(v.value), v.index);
}

IndexedValue<BinaryOutputStatus, uint16_t> APDUParser::BoolToBinaryOutputStatus(const IndexedValue<bool, uint16_t>& v)
{
	return IndexedValue<BinaryOutputStatus, uint16_t>(BinaryOutputStatus(v.value), v.index);
}

#define MACRO_PARSE_OBJECTS_WITH_RANGE(descriptor) \
	case(GroupVariation::descriptor): \
	return ParseRangeFixedSize(gvRecord.enumeration, qualifier, descriptor##Serializer::Inst(), buffer, range, pHandler);

APDUParser::Result APDUParser::ParseObjectsWithRange(QualifierCode qualifier, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, const Range& range, IAPDUHandler* pHandler)
{
	switch(gvRecord.enumeration)
	{	
		case(GroupVariation::Group1Var1):				
			return ParseRangeAsBitField(buffer, qualifier, range, [&](QualifierCode qualifier, const IterableBuffer<IndexedValue<bool, uint16_t>>& values) {
				if(pHandler) {
					auto mapped = IterableTransforms<IndexedValue<bool, uint16_t>>::Map<IndexedValue<Binary, uint16_t>>(values, [](const IndexedValue<bool, uint16_t>& v) { return IndexedValue<Binary, uint16_t>(Binary(v.value), v.index); });
					pHandler->OnRange(gvRecord.enumeration, qualifier, mapped);
				}
			});		
		
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group1Var2);

		case(GroupVariation::Group10Var1):				
			return ParseRangeAsBitField(buffer, qualifier, range, [&](QualifierCode qualifier, IterableBuffer<IndexedValue<bool, uint16_t>>& values) {
				if(pHandler) {
					auto mapped = IterableTransforms<IndexedValue<bool, uint16_t>>::Map<IndexedValue<BinaryOutputStatus, uint16_t>>(values, [](const IndexedValue<bool, uint16_t>& v) { return IndexedValue<BinaryOutputStatus, uint16_t>(BinaryOutputStatus(v.value), v.index); });
					pHandler->OnRange(gvRecord.enumeration, qualifier, mapped);
				}
			});

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group3Var2);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group10Var2);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var2);		
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var6);		

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var3);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var4);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var6);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var3);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var4);

		case(GroupVariation::Group50Var1):
			return ParseCountOf<Group50Var1>(buffer, range.Count(), pHandler); 
		
		case(GroupVariation::Group52Var2):
			return ParseCountOf<Group52Var2>(buffer, range.Count(), pHandler); 

		case(GroupVariation::Group80Var1):		
			return ParseRangeAsBitField(buffer, qualifier, range, [&](QualifierCode qualifier, const IterableBuffer<IndexedValue<bool, uint16_t>>& values) {
				if(pHandler) {
					pHandler->OnIIN(gvRecord.enumeration, qualifier, values);
				}
			});

		case(GroupVariation::Group110AnyVar):
			return ParseRangeOfOctetData(gvRecord, buffer, qualifier, range, pHandler);
		
		default:
			return Result::ILLEGAL_OBJECT_QUALIFIER;
	}	
}

APDUParser::Result APDUParser::ParseRangeOfOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		QualifierCode qualifier,
		const Range& range, 
		IAPDUHandler* pHandler)
{
	uint32_t size = gvRecord.variation*range.Count();
	if(buffer.Size() < size) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	{
		if(pHandler) {
			auto collection = IterableTransforms<IndexedValue<OctetString, uint16_t>>::From(buffer, range.Count(), [&](ReadOnlyBuffer& buffer, uint32_t pos) {
				OctetString octets(buffer.Truncate(gvRecord.variation));
				IndexedValue<OctetString, uint16_t> value(octets, range.start + pos);
				buffer.Advance(gvRecord.variation);
				return value;
			});
			pHandler->OnRange(gvRecord.enumeration, qualifier, collection);
		}
		buffer.Advance(size);
		return Result::OK;
	}
}

}

