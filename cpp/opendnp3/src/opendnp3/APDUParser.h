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
#include "IAPDUHandler.h"
#include "LazyIterable.h"
#include "IndexParser.h"
#include "BitReader.h"
#include "IterableTransforms.h"
#include "StaticSizeConfiguration.h"

namespace opendnp3
{

class APDUParser : private PureStatic
{
	public:

	class Context
	{
		public:		

		static Context Default() { return Context(); }

		Context(bool expectContents_ = true, uint32_t aMaxObjects = SizeConfiguration::MAX_OBJECTS_PER_APDU) : 
			expectContents(expectContents_),
			count(0), 
			MAX_OBJECTS(aMaxObjects) 
		{}

		bool ExpectsContents() const
		{
			return expectContents;
		}

		// return true if the count exceeds the max count
		bool AddObjectCount(uint32_t aCount)
		{
			count += aCount;
			return count > MAX_OBJECTS;
		}
		
		private:


		const bool expectContents;
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
	
	static Result ParseTwoPass(const openpal::ReadOnlyBuffer& buffer, IAPDUHandler* pHandler, Context context = Context::Default());
	
	private:

	static Result ParseSinglePass(const openpal::ReadOnlyBuffer& buffer, IAPDUHandler* pHandler, Context context);

	static Result ParseHeaders(const openpal::ReadOnlyBuffer& buffer, Context context, IAPDUHandler* pHandler);

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

	static Result ParseHeader(openpal::ReadOnlyBuffer& buffer, Context& context, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseCountHeader(openpal::ReadOnlyBuffer& buffer, Context& context, const HeaderRecord& record, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler);

	template <class ParserType, class CountType>
	static Result ParseRangeHeader(openpal::ReadOnlyBuffer& buffer, Context& context, const HeaderRecord& record, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseIndexPrefixHeader(openpal::ReadOnlyBuffer& buffer, Context& context, const HeaderRecord& record, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler);

	template <class ParserType, class CountType>
	static Result ParseRange(openpal::ReadOnlyBuffer& buffer, Context& context, Range& range);

	template <class ParserType>
	static Result ParseCount(openpal::ReadOnlyBuffer& buffer, Context& context, typename ParserType::Type& count);	

	static Result ParseObjectsWithRange(const HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord&, const Range& range, IAPDUHandler* pHandler);	
	
	static Result ParseObjectsWithIndexPrefix(const HeaderRecord& record, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, uint32_t count, IndexParser* pParser, IAPDUHandler* pHandler);
	
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
		IAPDUHandler* pHandler);

	static Result ParseIndexPrefixedOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		const HeaderRecord& record,
		uint32_t count, 
		IndexParser* pParser, 
		IAPDUHandler* pHandler);
		
	template <class Target>
	static Result ParseRangeFixedSize(GroupVariation gv, const HeaderRecord& record, openpal::ISerializer<Target>* pSerializer, openpal::ReadOnlyBuffer& buffer, const Range& range, IAPDUHandler* pHandler);

	template <class Descriptor>
	static Result ParseCountOf(openpal::ReadOnlyBuffer& buffer, uint32_t count, IAPDUHandler* pHandler);

	template <class Target>
	static Result ParseCountFixedSizeWithIndex(
		GroupVariation gv,
		const HeaderRecord& record,
		openpal::ReadOnlyBuffer& buffer, 
		uint32_t count, 
		IndexParser* pParser, 
		openpal::ISerializer<Target>* pSerializer,
		IAPDUHandler* pHandler);	

	static IndexedValue<Binary> BoolToBinary(const IndexedValue<bool>& v);
	static IndexedValue<ControlStatus> BoolToControlStatus(const IndexedValue<bool>& v);
};

template <class IndexType>
APDUParser::Result APDUParser::ParseCountHeader(openpal::ReadOnlyBuffer& buffer, Context& context, const HeaderRecord& record, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler)
{
	typename IndexType::Type count;
	auto res = ParseCount<IndexType>(buffer, context, count);
	if(res == Result::OK)
	{			
		if(context.ExpectsContents()) 
		{						
			return ParseObjectsWithRange(record.Add(IndexType::Size), buffer, gvRecord, Range::FromCount(count), pHandler);
		}
		else 
		{
			if(pHandler) 
			{
				pHandler->OnCountRequest(gvRecord.enumeration, count);
			}
			return Result::OK;
		}		
	}
	else return res;		
}

template <class ParserType, class CountType>
APDUParser::Result APDUParser::ParseRangeHeader(openpal::ReadOnlyBuffer& buffer, Context& context, const HeaderRecord& record, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler)
{
	Range range;				
	auto res = ParseRange<ParserType, CountType>(buffer, context, range);
	if(res == Result::OK)
	{
		if(context.ExpectsContents()) 
		{
			return ParseObjectsWithRange(record.Add(2*ParserType::Size), buffer, gvRecord, range, pHandler);
		}
		else
		{
			if(pHandler) 
			{				
				pHandler->OnRangeRequest(gvRecord.enumeration, StaticRange(range.start, range.stop));
			}
			return Result::OK;
		}
	}
	else 
	{
		return res;
	}
}

template <class IndexType>
APDUParser::Result APDUParser::ParseIndexPrefixHeader(openpal::ReadOnlyBuffer& buffer, Context& context, const HeaderRecord& record, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler)
{
	typename IndexType::Type count;
	auto res = ParseCount<IndexType>(buffer, context, count);
	if(res == Result::OK)
	{
		return ParseObjectsWithIndexPrefix(record.Add(IndexType::Size), buffer, gvRecord, count, TypedIndexParser<IndexType>::Inst(), pHandler);
	}
	else 
	{
		return res;
	}	
}

template <class Callback>
APDUParser::Result APDUParser::ParseRangeAsBitField(	
	openpal::ReadOnlyBuffer& buffer,
	const HeaderRecord& record,
	const Range& range, 
	const Callback& callback)
{
	size_t numBytes = NumBytesInBits(range.Count());
	if(buffer.Size() < numBytes) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {	
		auto collection = IterableTransforms<IndexedValue<bool>>::From(buffer, range.Count(), 
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
				range.stop = stop;
				return Result::OK;
			}			
		}
	}
}

template <class ParserType>
APDUParser::Result APDUParser::ParseCount(openpal::ReadOnlyBuffer& buffer, Context& context, typename ParserType::Type& count)
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
APDUParser::Result APDUParser::ParseRangeFixedSize(GroupVariation gv, const HeaderRecord& record, openpal::ISerializer<Target>* pSerializer, openpal::ReadOnlyBuffer& buffer, const Range& range, IAPDUHandler* pHandler)
{	
	uint32_t size = range.Count() * pSerializer->Size();
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
	
		if(pHandler) {
			auto collection = IterableTransforms<IndexedValue<Target>>::From(buffer, range.Count(), [range, pSerializer](openpal::ReadOnlyBuffer& buffer, uint32_t pos) {
				return IndexedValue<Target>(pSerializer->Read(buffer), range.start + pos);
			});
			pHandler->OnRange(gv, record.Complete(size), collection);
		}
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class Descriptor>
APDUParser::Result APDUParser::ParseCountOf(openpal::ReadOnlyBuffer& buffer, uint32_t count, IAPDUHandler* pHandler)
{
	uint32_t size = count * Descriptor::SIZE;
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {				
		if(pHandler) {
			auto collection = IterableTransforms<Descriptor>::From(buffer, count, [](openpal::ReadOnlyBuffer& buffer, uint32_t) {
				return Descriptor::Read(buffer);
			});
			pHandler->OnCountOf(collection);
		}	
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
	IAPDUHandler* pHandler)
{
	uint32_t size = count * (pParser->IndexSize() + pSerializer->Size());
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		if(pHandler) {
			auto collection = IterableTransforms<IndexedValue<Target>>::From(buffer, count, [pParser, pSerializer](openpal::ReadOnlyBuffer& buffer, uint32_t) {
				auto index = pParser->ReadIndex(buffer);
				auto value = pSerializer->Read(buffer);
				return IndexedValue<Target>(value, index);
			});
			pHandler->OnIndexPrefix(gv, record.Complete(size), collection);
		}
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}



}

#endif
