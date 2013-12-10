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
#include "LazyIterable.h"

#include "ObjectDescriptors.h"
#include "BitReader.h"

using namespace openpal;

namespace opendnp3
{

APDUParser::Result APDUParser::ParseHeaders(ReadOnlyBuffer aBuffer, IAPDUHeaderHandler& arHandler)
{
	while(aBuffer.Size() > 0)
	{
		auto result = ParseHeader(aBuffer, arHandler);
		if(result != APDUParser::Result::OK) return result;
	}

	return APDUParser::Result::OK;
}

APDUParser::Result APDUParser::ParseHeader(ReadOnlyBuffer& buffer, IAPDUHeaderHandler& handler)
{
	if(buffer.Size() < 3) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_HEADER; 
	else {
		uint8_t group = UInt8::ReadBuffer(buffer);		
		uint8_t variation = UInt8::ReadBuffer(buffer);		
		auto gv = GroupVariationEnum::Get(group, variation);
		if(gv == GroupVariation::UNKNOWN) return APDUParser::Result::UNKNOWN_OBJECT;
		QualifierCode qualifier = QualifierCodeFromType(UInt8::ReadBuffer(buffer));
		switch(qualifier)
		{
			case(QualifierCode::ALL_OBJECTS):
			{
				handler.AllObjects(gv);
				return APDUParser::Result::OK;
			}
			case(QualifierCode::UINT8_CNT):
			{
				uint32_t count;
				auto res = ParseCount<UInt8>(buffer, handler, gv, count);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithRange(buffer, handler, gv, Range(0, count)) : res;				
			}
			case(QualifierCode::UINT16_CNT):
			{
				uint32_t count;
				auto res = ParseCount<UInt16>(buffer, handler, gv, count);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithRange(buffer, handler, gv, Range(0, count)) : res;				
			}
			case(QualifierCode::UINT32_CNT):
			{
				uint32_t count;
				auto res = ParseCount<UInt32>(buffer, handler, gv, count);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithRange(buffer, handler, gv, Range(0, count)) : res;				
			}
			case(QualifierCode::UINT8_START_STOP):
			{
				Range range;
				auto res = ParseRange<UInt8, uint16_t>(buffer, handler, gv, range);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithRange(buffer, handler, gv, range) : res;
			}
			case(QualifierCode::UINT16_START_STOP):
			{
				Range range;
				auto res = ParseRange<UInt16, uint32_t>(buffer, handler, gv, range);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithRange(buffer, handler, gv, range) : res;
			}
			case(QualifierCode::UINT32_START_STOP):
			{
				Range range;
				auto res = ParseRange<UInt32, uint64_t>(buffer, handler, gv, range);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithRange(buffer, handler, gv, range) : res;			
			}
			case(QualifierCode::UINT8_CNT_UINT8_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt8>(buffer, handler, gv, count);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithIndexPrefix(buffer, handler, gv, count, TypedIndexParser<UInt8>::Inst()) : res;				
			}
			case(QualifierCode::UINT16_CNT_UINT16_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt16>(buffer, handler, gv, count);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithIndexPrefix(buffer, handler, gv, count, TypedIndexParser<UInt16>::Inst()) : res;				
			}
			case(QualifierCode::UINT32_CNT_UINT32_INDEX):
			{
				uint32_t count;
				auto res = ParseCount<UInt32>(buffer, handler, gv, count);
				return (res == APDUParser::Result::OK) ? ParseObjectsWithIndexPrefix(buffer, handler, gv, count, TypedIndexParser<UInt32>::Inst()) : res;				
			}
			default:
				return APDUParser::Result::UNKNOWN_QUALIFIER;
		}
	}
}

APDUParser::Result APDUParser::ParseObjectsWithIndexPrefix(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler&  output, GroupVariation gv, uint32_t count, IndexParser* pParser)
{
	switch(gv)
	{
		case(GroupVariation::Group2Var1):
			return ParseCountFixedSizeWithIndex<Group2Var1Parser>(buffer, output, count, pParser);			
		default:
			return APDUParser::Result::ILLEGAL_OBJECT_QUALIFIER;
	}
	
}

APDUParser::Result APDUParser::ParseObjectsWithRange(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, GroupVariation gv, const Range& range)
{
	switch(gv)
	{	
		case(GroupVariation::Group1Var1):
			return ParseRangeAsBitField(buffer, output, range);
		case(GroupVariation::Group1Var2):
			return ParseRangeFixedSize<Group1Var2Parser>(buffer, output, range);
		default:
			return APDUParser::Result::ILLEGAL_OBJECT_QUALIFIER;
	}	
}

APDUParser::Result APDUParser::ParseRangeAsBitField(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, const Range& range)
{
	size_t numBytes = NumBytesInBits(range.count);
	if(buffer.Size() < numBytes) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		LazyIterable<Binary> collection(buffer, range.count, GetBit);
		output.OnStaticData(range.start, collection);
		buffer.Advance(numBytes);
		return APDUParser::Result::OK;
	}
}

}

