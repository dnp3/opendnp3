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
#ifndef __APDU_HEADER_PARSER_H_
#define __APDU_HEADER_PARSER_H_

#include <vector>
#include <functional>
#include <limits>

#include <openpal/BufferWrapper.h>

#include <opendnp3/Uncopyable.h>

#include "Range.h"
#include "IAPDUHeaderHandler.h"
#include "LazyFixedSizeCollection.h"
#include "ObjectDescriptors.h"
#include "IndexParser.h"

namespace opendnp3
{

class APDUParser : private PureStatic
{
	public:	

	enum class Result: int 
	{
		OK,
		NOT_ENOUGH_DATA_FOR_HEADER,
		NOT_ENOUGH_DATA_FOR_RANGE,
		NOT_ENOUGH_DATA_FOR_OBJECTS,
		UNREASONABLE_OBJECT_COUNT,
		UNKNOWN_OBJECT,
		UNKNOWN_QUALIFIER,
		ILLEGAL_OBJECT_QUALIFIER,
		BAD_START_STOP,
		COUNT_OF_ZERO
	};

	static Result ParseHeaders(openpal::ReadOnlyBuffer buffer, IAPDUHeaderHandler& output);

	private:

	// return true if it's an error, false otherwise	

	static Result ParseHeader(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output);

	static Result ParseObjectsWithRange(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler&  output, GroupVariation, const Range& range);	
	
	static Result ParseObjectsWithIndexPrefix(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler&  output, GroupVariation, uint32_t count, IndexParser* pParser);
	
	static Result ParseRangeAsBitField(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, const Range& range);
	
	template <class ParserType, class CountType>
	static Result ParseRange(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, GroupVariation gv, Range& range);

	template <class ParserType>
	static Result ParseCount(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, GroupVariation gv, uint32_t& count);	

	template <class Descriptor>
	static Result ParseRangeFixedSize(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, const Range& range);

	template <class Descriptor>
	static Result ParseCountFixedSizeWithIndex(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, uint32_t count, IndexParser* pParser);	
};

template <class ParserType, class RangeType>
APDUParser::Result APDUParser::ParseRange(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, GroupVariation gv, Range& range)
{
	if(buffer.Size() < (2*ParserType::Size)) return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	else {
		auto start = ParserType::ReadBuffer(buffer);
		auto stop = ParserType::ReadBuffer(buffer);
		if(start > stop) return Result::BAD_START_STOP;
		else {
			RangeType count = static_cast<RangeType>(stop) - static_cast<RangeType>(start) + 1;
			// 65535 is a reasonable upper bound for object counts. This
			// will ensure that size calculations never overflow with 2^32 sizes
			if(count > std::numeric_limits<uint16_t>::max()) return Result::UNREASONABLE_OBJECT_COUNT;
			else {				
				range.start = start;
				range.count = static_cast<size_t>(count);
				return Result::OK;
			}
		}
	}
}

template <class ParserType>
APDUParser::Result APDUParser::ParseCount(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, GroupVariation gv, uint32_t& count)
{
	if(buffer.Size() < ParserType::Size) return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	else {
		count = ParserType::ReadBuffer(buffer);
		if(count == 0) return Result::COUNT_OF_ZERO;
		else {
			if(count > std::numeric_limits<uint16_t>::max()) return Result::UNREASONABLE_OBJECT_COUNT;
			else return Result::OK;
		}
	}
}


template <class Descriptor>
APDUParser::Result APDUParser::ParseRangeFixedSize(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, const Range& range)
{
	size_t size = range.count * Descriptor::Underlying::SIZE;
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		auto read = [](ReadOnlyBuffer& buffer, size_t) { return Descriptor::Read(buffer); };
		LazyFixedSizeCollection<typename Descriptor::Target> collection(buffer, range.count, read);
		output.OnStaticData(range.start, collection);
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class Descriptor>
APDUParser::Result APDUParser::ParseCountFixedSizeWithIndex(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, uint32_t count, IndexParser* pParser)
{
	size_t size = count * (pParser->IndexSize() + Descriptor::Underlying::SIZE);
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		auto readWithIndex = [&](ReadOnlyBuffer& buffer, size_t) {
			uint32_t index = pParser->ReadIndex(buffer);
			auto value = Descriptor::Read(buffer);
			return IndexedValue<typename Descriptor::Target>(value, index);
		};		
		LazyFixedSizeCollection< IndexedValue <typename Descriptor::Target> > collection(buffer, count, readWithIndex);
		output.OnEventData(collection);
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

}

#endif
