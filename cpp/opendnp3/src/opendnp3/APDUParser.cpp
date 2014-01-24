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

APDUParser::Result APDUParser::ParseHeaders(openpal::ReadOnlyBuffer buffer, IAPDUHeaderHandler& output)
{
	return ParseHeaders(buffer, Context::Default(), output);
}

APDUParser::Result APDUParser::ParseHeaders(ReadOnlyBuffer aBuffer, Context context, IAPDUHeaderHandler& arHandler)
{	
	while(aBuffer.Size() > 0)
	{
		auto result = ParseHeader(aBuffer, context, arHandler);
		if(result != Result::OK) return result;
	}

	return Result::OK;
}

APDUParser::Result APDUParser::ParseHeader(ReadOnlyBuffer& buffer, Context& context, IAPDUHeaderHandler& handler)
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
				handler.AllObjects(gvRecord.enumeration, record.Complete(0));
				return Result::OK;
			}
			case(QualifierCode::UINT8_CNT):
			{
				uint32_t count;
				auto res = ParseCount<UInt8>(buffer, context, count);				
				return (res == Result::OK) ? ParseObjectsWithRange(record.Add(UInt8::Size), buffer, gvRecord, Range(0, count), handler) : res;				
			}
			case(QualifierCode::UINT16_CNT):
			{
				uint32_t count;
				auto res = ParseCount<UInt16>(buffer, context, count);
				return (res == Result::OK) ? ParseObjectsWithRange(record.Add(UInt16::Size), buffer, gvRecord, Range(0, count), handler) : res;				
			}			
			case(QualifierCode::UINT8_START_STOP):
			{
				Range range;				
				auto res = ParseRange<UInt8, uint16_t>(buffer, context, range);
				return (res == Result::OK) ? ParseObjectsWithRange(record.Add(2*UInt8::Size), buffer, gvRecord, range, handler) : res;
			}
			case(QualifierCode::UINT16_START_STOP):
			{
				Range range;
				auto res = ParseRange<UInt16, uint32_t>(buffer, context, range);
				return (res == Result::OK) ? ParseObjectsWithRange(record.Add(2*UInt16::Size), buffer, gvRecord, range, handler) : res;
			}			
			case(QualifierCode::UINT8_CNT_UINT8_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt8>(buffer, context, count);
				return (res == Result::OK) ? ParseObjectsWithIndexPrefix(record.Add(UInt8::Size), buffer, gvRecord, count, TypedIndexParser<UInt8>::Inst(), handler) : res;				
			}
			case(QualifierCode::UINT16_CNT_UINT16_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt16>(buffer, context, count);
				return (res == Result::OK) ? ParseObjectsWithIndexPrefix(record.Add(UInt16::Size),buffer, gvRecord, count, TypedIndexParser<UInt16>::Inst(), handler) : res;				
			}			
			default:
				return Result::UNKNOWN_QUALIFIER;
		}
	}
}

#define MACRO_PARSE_COUNT_FIXED_SIZE_WITH_INDEX(descriptor) \
case(GroupVariation::descriptor): \
	return ParseCountFixedSizeWithIndex(gvRecord.enumeration, record, buffer, count, pParser, descriptor##Serializer::Inst(), output);

APDUParser::Result APDUParser::ParseObjectsWithIndexPrefix(const HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, uint32_t count, IndexParser* pParser, IAPDUHeaderHandler&  output)
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
			return ParseIndexPrefixedOctetData(gvRecord, buffer, record, count, pParser, output);

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
		IAPDUHeaderHandler& handler)
{
	size_t size = count * (pParser->IndexSize() + gvRecord.variation);
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		
		auto iterable = Iterable<IndexedValue<ReadOnlyBuffer>>::From(buffer, count,
			[&](ReadOnlyBuffer& buffer, uint32_t position) {	
				auto index = pParser->ReadIndex(buffer);
				auto buff = buffer.Truncate(gvRecord.variation);
				buffer.Advance(gvRecord.variation);
				return IndexedValue<ReadOnlyBuffer>(buff, index);
			}	
		);		

		handler.OnIndexPrefixOfOctets(gvRecord.enumeration, record.Complete(size), iterable);
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
	return ParseRangeFixedSize(gvRecord.enumeration, record, descriptor##Serializer::Inst(), buffer, range, output);

APDUParser::Result APDUParser::ParseObjectsWithRange(const APDUParser::HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, const Range& range, IAPDUHeaderHandler& output)
{
	switch(gvRecord.enumeration)
	{	
		case(GroupVariation::Group1Var1):				
			return ParseRangeAsBitField(buffer, record, range, [&](const ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<bool>>& values) {
				auto mapped = Iterable<IndexedValue<bool>>::Map<IndexedValue<Binary>>(values, [](const IndexedValue<bool>& v) { return IndexedValue<Binary>(Binary(v.value), v.index); });
				output.OnRange(gvRecord.enumeration, header, mapped);
			});		
		
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group1Var2);

		case(GroupVariation::Group10Var1):				
			return ParseRangeAsBitField(buffer, record, range, [&](const ReadOnlyBuffer& header, IterableBuffer<IndexedValue<bool>>& values) {
				auto mapped = Iterable<IndexedValue<bool>>::Map<IndexedValue<ControlStatus>>(values, [](const IndexedValue<bool>& v) { return IndexedValue<ControlStatus>(ControlStatus(v.value), v.index); });
				output.OnRange(gvRecord.enumeration, header, mapped);								
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
			return ParseCountOf<Group50Var1>(buffer, range.count, output); 
		
		case(GroupVariation::Group52Var2):
			return ParseCountOf<Group52Var2>(buffer, range.count, output); 

		case(GroupVariation::Group80Var1):		
			return ParseRangeAsBitField(buffer, record, range, [&](const ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<bool>>& values) { 
				output.OnIIN(gvRecord.enumeration, header, values); 
			});

		case(GroupVariation::Group110AnyVar):
			return ParseRangeOfOctetData(gvRecord, buffer, record, range, output);
		
		default:
			return Result::ILLEGAL_OBJECT_QUALIFIER;
	}	
}

APDUParser::Result APDUParser::ParseRangeOfOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		const Range& range, 
		IAPDUHeaderHandler& handler)
{
	size_t size = gvRecord.variation*range.count;
	if(buffer.Size() < size) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	{
		auto collection = Iterable<IndexedValue<openpal::ReadOnlyBuffer>>::From(buffer, range.count, [&](ReadOnlyBuffer& buffer, uint32_t pos) {			
			IndexedValue<ReadOnlyBuffer> value(buffer.Truncate(gvRecord.variation), range.start + pos);
			buffer.Advance(gvRecord.variation);
			return value;
		});
		handler.OnRangeOfOctets(gvRecord.enumeration, record.Complete(size), collection);
		buffer.Advance(size);
		return Result::OK;
	}
}

}

