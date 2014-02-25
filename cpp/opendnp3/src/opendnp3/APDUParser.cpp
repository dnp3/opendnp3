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

#include <openpal/Serialization.h>

#include "gen/QualifierCode.h"
#include "GroupVariation.h"

#include "objects/Group1.h"
#include "objects/Group2.h"
#include "objects/Group10.h"
#include "objects/Group12.h"
#include "objects/Group20.h"
#include "objects/Group22.h"
#include "objects/Group30.h"
#include "objects/Group32.h"
#include "objects/Group40.h"
#include "objects/Group41.h"
#include "objects/Group50.h"
#include "objects/Group52.h"

#include "objects/MeasurementFactory.h"

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
		const HeaderRecord record(buffer, 3); 
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
				if(pHandler) pHandler->AllObjects(gvRecord.enumeration, record.Complete(0));				
				return Result::OK;
			}

			case(QualifierCode::UINT8_CNT):
				return ParseCountHeader<UInt8>(buffer, context, record, gvRecord, pHandler);

			case(QualifierCode::UINT16_CNT):
				return ParseCountHeader<UInt16>(buffer, context, record, gvRecord, pHandler);
			
			case(QualifierCode::UINT8_START_STOP):
				return ParseRangeHeader<UInt8, uint16_t>(buffer, context, record, gvRecord, pHandler);

			case(QualifierCode::UINT16_START_STOP):
				return ParseRangeHeader<UInt16, uint32_t>(buffer, context, record, gvRecord, pHandler);
					
			case(QualifierCode::UINT8_CNT_UINT8_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt8>(buffer, context, count);
				return (res == Result::OK) ? ParseObjectsWithIndexPrefix(record.Add(UInt8::Size), buffer, gvRecord, count, TypedIndexParser<UInt8>::Inst(), pHandler) : res;				
			}
			case(QualifierCode::UINT16_CNT_UINT16_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt16>(buffer, context, count);
				return (res == Result::OK) ? ParseObjectsWithIndexPrefix(record.Add(UInt16::Size),buffer, gvRecord, count, TypedIndexParser<UInt16>::Inst(), pHandler) : res;				
			}			
			default:
				return Result::UNKNOWN_QUALIFIER;
		}
	}
}

#define MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(descriptor) \
case(GroupVariation::descriptor): \
	return ParseCountFixedSizeWithIndex(gvRecord.enumeration, record, buffer, count, pParser, descriptor##Serializer::Inst(), pHandler);

APDUParser::Result APDUParser::ParseObjectsWithIndexPrefix(const HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, uint32_t count, IndexParser* pParser, IAPDUHandler* pHandler)
{
	switch(gvRecord.enumeration)
	{
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group2Var1);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group2Var2);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group2Var3);	

		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group12Var1);

		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group22Var1);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group22Var2);		
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group22Var5);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group22Var6);
		
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var1);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var2);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var3);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var4);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var5);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var6);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var7);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group32Var8);

		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group41Var1);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group41Var2);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group41Var3);
		MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(Group41Var4);

		case(GroupVariation::Group111AnyVar):
			return ParseIndexPrefixedOctetData(gvRecord, buffer, record, count, pParser, pHandler);

		default:
			return Result::ILLEGAL_OBJECT_QUALIFIER;
	}	
}

APDUParser::Result APDUParser::ParseIndexPrefixedOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		uint32_t count, 
		IndexParser* pParser, 
		IAPDUHandler* pHandler)
{
	size_t size = count * (pParser->IndexSize() + gvRecord.variation);
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		
		if(pHandler) {
			auto iterable = IterableTransforms<IndexedValue<OctetString>>::From(buffer, count,
				[&](ReadOnlyBuffer& buffer, uint32_t position) {	
					auto index = pParser->ReadIndex(buffer);				
					OctetString octets(buffer.Truncate(gvRecord.variation));
					buffer.Advance(gvRecord.variation);
					return IndexedValue<OctetString>(octets, index);
				}	
			);
			pHandler->OnIndexPrefix(gvRecord.enumeration, record.Complete(size), iterable);
		}

		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

IndexedValue<Binary> APDUParser::BoolToBinary(const IndexedValue<bool>& v)
{
	return IndexedValue<Binary>(Binary(v.value), v.index);
}

IndexedValue<ControlStatus> APDUParser::BoolToControlStatus(const IndexedValue<bool>& v)
{
	return IndexedValue<ControlStatus>(ControlStatus(v.value), v.index);
}

#define MACRO_PARSE_OBJECTS_WITH_RANGE(descriptor) \
	case(GroupVariation::descriptor): \
	return ParseRangeFixedSize(gvRecord.enumeration, record, descriptor##Serializer::Inst(), buffer, range, pHandler);

APDUParser::Result APDUParser::ParseObjectsWithRange(const APDUParser::HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, const Range& range, IAPDUHandler* pHandler)
{
	switch(gvRecord.enumeration)
	{	
		case(GroupVariation::Group1Var1):				
			return ParseRangeAsBitField(buffer, record, range, [&](const ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<bool>>& values) {
				if(pHandler) {
					auto mapped = IterableTransforms<IndexedValue<bool>>::Map<IndexedValue<Binary>>(values, [](const IndexedValue<bool>& v) { return IndexedValue<Binary>(Binary(v.value), v.index); });
					pHandler->OnRange(gvRecord.enumeration, header, mapped);
				}
			});		
		
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group1Var2);

		case(GroupVariation::Group10Var1):				
			return ParseRangeAsBitField(buffer, record, range, [&](const ReadOnlyBuffer& header, IterableBuffer<IndexedValue<bool>>& values) {
				if(pHandler) {
					auto mapped = IterableTransforms<IndexedValue<bool>>::Map<IndexedValue<ControlStatus>>(values, [](const IndexedValue<bool>& v) { return IndexedValue<ControlStatus>(ControlStatus(v.value), v.index); });
					pHandler->OnRange(gvRecord.enumeration, header, mapped);
				}
			});

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
			return ParseCountOf<Group50Var1>(buffer, range.count, pHandler); 
		
		case(GroupVariation::Group52Var2):
			return ParseCountOf<Group52Var2>(buffer, range.count, pHandler); 

		case(GroupVariation::Group80Var1):		
			return ParseRangeAsBitField(buffer, record, range, [&](const ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<bool>>& values) { 
				if(pHandler) {
					pHandler->OnIIN(gvRecord.enumeration, header, values); 
				}
			});

		case(GroupVariation::Group110AnyVar):
			return ParseRangeOfOctetData(gvRecord, buffer, record, range, pHandler);
		
		default:
			return Result::ILLEGAL_OBJECT_QUALIFIER;
	}	
}

APDUParser::Result APDUParser::ParseRangeOfOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		const Range& range, 
		IAPDUHandler* pHandler)
{
	size_t size = gvRecord.variation*range.count;
	if(buffer.Size() < size) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	{
		if(pHandler) {
			auto collection = IterableTransforms<IndexedValue<OctetString>>::From(buffer, range.count, [&](ReadOnlyBuffer& buffer, uint32_t pos) {
				OctetString octets(buffer.Truncate(gvRecord.variation));
				IndexedValue<OctetString> value(octets, range.start + pos);
				buffer.Advance(gvRecord.variation);
				return value;
			});
			pHandler->OnRange(gvRecord.enumeration, record.Complete(size), collection);
		}
		buffer.Advance(size);
		return Result::OK;
	}
}

}

