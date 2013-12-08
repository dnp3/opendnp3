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

#include "Range.h"
#include "IAPDUHeaderHandler.h"
#include "LazyFixedSizeCollection.h"
#include "ObjectDescriptors.h"

namespace opendnp3
{

class APDUParser
{
	public:	

	enum Result: int 
	{
		OK,
		NOT_ENOUGH_DATA_FOR_HEADER,
		NOT_ENOUGH_DATA_FOR_RANGE,
		NOT_ENOUGH_DATA_FOR_OBJECTS,
		UNREASONABLE_OBJECT_COUNT,
		UNKNOWN_OBJECT,
		UNKNOWN_QUALIFIER,
		ILLEGAL_OBJECT_QUALIFIER,
		BAD_START_STOP		
	};

	static Result ParseHeaders(openpal::ReadOnlyBuffer, IAPDUHeaderHandler& output);

	private:

	static Result ParseHeader(openpal::ReadOnlyBuffer&, IAPDUHeaderHandler& output);

	static Result ParseRange(openpal::ReadOnlyBuffer& arBuffer, IAPDUHeaderHandler& output, GroupVariation gv, const Range& range);

	APDUParser();
	APDUParser(const APDUParser&);

	template <class ParserType, class CountType>
	static Result ParseRange(openpal::ReadOnlyBuffer& arBuffer, IAPDUHeaderHandler& output, GroupVariation gv);	

	template <class Descriptor>
	static Result ParseRangeFixedSize(openpal::ReadOnlyBuffer& arBuffer, IAPDUHeaderHandler& output, const Range& range);	
};

template <class ParserType, class CountType>
APDUParser::Result APDUParser::ParseRange(openpal::ReadOnlyBuffer& arBuffer, IAPDUHeaderHandler& output, GroupVariation gv)
{
	if(arBuffer.Size() < (2*ParserType::Size)) return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	else {
		auto start = ParserType::ReadBuffer(arBuffer);
		auto stop = ParserType::ReadBuffer(arBuffer);
		if(start > stop) return BAD_START_STOP;
		else {
			CountType count = static_cast<CountType>(stop) - static_cast<CountType>(start) + 1;
			// 65535 is a reasonable upper bound for object counts. This
			// will ensure that size calculations never overflow with 2^32 sizes
			if(count > std::numeric_limits<uint16_t>::max()) return UNREASONABLE_OBJECT_COUNT;
			else {
				Range range(start, stop, static_cast<size_t>(count));
				return ParseRange(arBuffer, output, gv, range);
			}
		}
	}
}


template <class Descriptor>
APDUParser::Result APDUParser::ParseRangeFixedSize(openpal::ReadOnlyBuffer& buffer, IAPDUHeaderHandler& output, const Range& range)
{
	auto size = range.count * Descriptor::Underlying::SIZE;
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		LazyFixedSizeCollection<typename Descriptor::Target> collection(buffer, range.count, Compose<Descriptor>::Read);
		output.OnStaticData(range.start, collection);
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

}

#endif
