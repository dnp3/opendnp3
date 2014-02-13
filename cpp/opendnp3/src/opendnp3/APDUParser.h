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
#ifndef __APDU_PARSER_H_
#define __APDU_PARSER_H_

#include <functional>
#include <limits>

#include <openpal/BufferWrapper.h>
#include <openpal/ISerializer.h>
#include <opendnp3/Uncopyable.h>

#include "Range.h"
#include "IAPDUHeaderHandler.h"
#include "LazyIterable.h"
#include "IndexParser.h"
#include "BitReader.h"
#include "IterableTransforms.h"

namespace opendnp3
{

class APDUParser : private PureStatic
{
	public:

	class Context
	{
		public:

		static Context Default() { return Context(32768); }

		Context(uint32_t aMaxObjects) : count(0), MAX_OBJECTS(aMaxObjects) {}

		//return true if the count exceeds the max count
		bool AddObjectCount(uint32_t aCount)
		{
			count += aCount;
			return count > MAX_OBJECTS;
		}
		
		private:

		uint32_t count;
		const uint32_t MAX_OBJECTS;
	};

	enum class Result
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
	static Result ParseHeaders(openpal::ReadOnlyBuffer buffer, Context context, IAPDUHeaderHandler& output);

	private:

	// records the start and length of a header
	// has one method that returns a bounding wrapper 
	// on the entire object header
	class HeaderRecord
	{
		public:
		HeaderRecord(const openpal::ReadOnlyBuffer& aHeaderStart, uint32_t aHeaderSize) : 
			headerStart(aHeaderStart),
			headerSize(aHeaderSize)
		{}

		HeaderRecord Add(uint32_t aSize) const
		{
			return HeaderRecord(headerStart, headerSize + aSize);
		}

		openpal::ReadOnlyBuffer Complete(uint32_t aSize) const
		{
			return headerStart.Truncate(headerSize + aSize);
		}

		private:

		const openpal::ReadOnlyBuffer headerStart;
		const uint32_t headerSize;				
	};

	// return true if it's an error, false otherwise	

	static Result ParseHeader(openpal::ReadOnlyBuffer& buffer, Context& context, IAPDUHeaderHandler& output);

	template <class ParserType, class CountType>
	static Result ParseRange(openpal::ReadOnlyBuffer& buffer, Context& context, Range& range);

	template <class ParserType>
	static Result ParseCount(openpal::ReadOnlyBuffer& buffer, Context& context, uint32_t& count);	

	static Result ParseObjectsWithRange(const HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord&, const Range& range, IAPDUHeaderHandler&  output);	
	
	static Result ParseObjectsWithIndexPrefix(const HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, uint32_t count, IndexParser* pParser, IAPDUHeaderHandler&  output);
	
	template <class Fun>
	static Result ParseRangeAsBitField(		
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		const Range& range, 
		const Fun& action);

	static Result ParseRangeOfOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		const Range& range, 
		IAPDUHeaderHandler& handler);

	static Result ParseIndexPrefixedOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		uint32_t count, 
		IndexParser* pParser, 
		IAPDUHeaderHandler& handler);
		
	template <class Target>
	static Result ParseRangeFixedSize(GroupVariation gv, const HeaderRecord& record, openpal::ISerializer<Target>* pSerializer, openpal::ReadOnlyBuffer& buffer, const Range& range, IAPDUHeaderHandler& output);

	template <class Descriptor>
	static Result ParseCountOf(openpal::ReadOnlyBuffer& buffer, uint32_t count, IAPDUHeaderHandler& output);

	template <class Target>
	static Result ParseCountFixedSizeWithIndex(
		GroupVariation gv,
		const HeaderRecord& record,
		openpal::ReadOnlyBuffer& buffer, 
		uint32_t count, 
		IndexParser* pParser, 
		openpal::ISerializer<Target>* pSerializer,
		IAPDUHeaderHandler& handler);	

	static IndexedValue<Binary> BoolToBinary(const IndexedValue<bool>& v);
	static IndexedValue<ControlStatus> BoolToControlStatus(const IndexedValue<bool>& v);
};

template <class Callback>
APDUParser::Result APDUParser::ParseRangeAsBitField(	
	openpal::ReadOnlyBuffer& buffer,
	const HeaderRecord& record,
	const Range& range, 
	const Callback& callback)
{
	size_t numBytes = NumBytesInBits(range.count);
	if(buffer.Size() < numBytes) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {	
		auto collection = IterableTransforms<IndexedValue<bool>>::From(buffer, range.count, 
			[&](openpal::ReadOnlyBuffer& buffer, uint32_t pos) {
				return IndexedValue<bool>(GetBit(buffer, pos), pos + range.start);
			}
		);
		callback(record.Complete(numBytes), collection);		
		buffer.Advance(numBytes);
		return Result::OK;
	}
}

template <class ParserType, class RangeType>
APDUParser::Result APDUParser::ParseRange(openpal::ReadOnlyBuffer& buffer, Context& context, Range& range)
{
	if(buffer.Size() < (2*ParserType::Size)) return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	else {
		auto start = ParserType::ReadBuffer(buffer);
		auto stop = ParserType::ReadBuffer(buffer);
		if(start > stop) return Result::BAD_START_STOP;
		else {
			RangeType count = static_cast<RangeType>(stop) - static_cast<RangeType>(start) + 1;
			if(context.AddObjectCount(count)) return Result::UNREASONABLE_OBJECT_COUNT;
			else
			{
				range.start = start;
				range.count = static_cast<size_t>(count);
				return Result::OK;
			}			
		}
	}
}

template <class ParserType>
APDUParser::Result APDUParser::ParseCount(openpal::ReadOnlyBuffer& buffer, Context& context, uint32_t& count)
{
	if(buffer.Size() < ParserType::Size) return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	else {
		count = ParserType::ReadBuffer(buffer);
		if(count == 0) return Result::COUNT_OF_ZERO;
		else 
		{
			bool overflow = context.AddObjectCount(count);
			return context.AddObjectCount(count) ? Result::UNREASONABLE_OBJECT_COUNT : Result::OK;
		}		
	}
}



template <class Target>
APDUParser::Result APDUParser::ParseRangeFixedSize(GroupVariation gv, const HeaderRecord& record, openpal::ISerializer<Target>* pSerializer, openpal::ReadOnlyBuffer& buffer, const Range& range, IAPDUHeaderHandler& output)
{	
	uint32_t size = range.count * pSerializer->Size();
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
	
		auto collection = IterableTransforms<IndexedValue<Target>>::From(buffer, range.count, [range, pSerializer](openpal::ReadOnlyBuffer& buffer, uint32_t pos) {
			return IndexedValue<Target>(pSerializer->Read(buffer), range.start + pos);
		});

		output.OnRange(gv, record.Complete(size), collection);
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class Descriptor>
APDUParser::Result APDUParser::ParseCountOf(openpal::ReadOnlyBuffer& buffer, uint32_t count, IAPDUHeaderHandler& output)
{
	uint32_t size = count * Descriptor::SIZE;
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {				
		
		auto collection = IterableTransforms<Descriptor>::From(buffer, count, [](openpal::ReadOnlyBuffer& buffer, uint32_t) {
			return Descriptor::Read(buffer);
		});
		output.OnCountOf(collection);
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class Target>
APDUParser::Result APDUParser::ParseCountFixedSizeWithIndex(
	GroupVariation gv,
	const HeaderRecord& record,	
	openpal::ReadOnlyBuffer& buffer, 
	uint32_t count, 
	IndexParser* pParser, 
	openpal::ISerializer<Target>* pSerializer,
	IAPDUHeaderHandler& handler)
{
	uint32_t size = count * (pParser->IndexSize() + pSerializer->Size());
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		
		auto collection = IterableTransforms<IndexedValue<Target>>::From(buffer, count, [pParser, pSerializer](openpal::ReadOnlyBuffer& buffer, uint32_t) {
			auto index = pParser->ReadIndex(buffer);
			auto value = pSerializer->Read(buffer);
			return IndexedValue<Target>(value, index);
		});
		
		handler.OnIndexPrefix(gv, record.Complete(size), collection);
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}



}

#endif
