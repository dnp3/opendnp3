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

	static Result ParseHeader(openpal::ReadOnlyBuffer& buffer, Context& context, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseCountHeader(openpal::ReadOnlyBuffer& buffer, Context& context, QualifierCode qualifier, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler);

	template <class ParserType, class CountType>
	static Result ParseRangeHeader(openpal::ReadOnlyBuffer& buffer, Context& context, QualifierCode qualifier, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseIndexPrefixHeader(openpal::ReadOnlyBuffer& buffer, Context& context, QualifierCode qualifier, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler);

	template <class ParserType, class CountType>
	static Result ParseRange(openpal::ReadOnlyBuffer& buffer, Context& context, Range& range);

	template <class ParserType>
	static Result ParseCount(openpal::ReadOnlyBuffer& buffer, Context& context, typename ParserType::Type& count);	

	static Result ParseObjectsWithRange(QualifierCode qualifier, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord&, const Range& range, IAPDUHandler* pHandler);
	
	template <class IndexType>
	static Result ParseObjectsWithIndexPrefix(QualifierCode qualifier, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, uint32_t count, IAPDUHandler* pHandler);
	
	template <class Fun>
	static Result ParseRangeAsBitField(		
		openpal::ReadOnlyBuffer& buffer, 
		QualifierCode qualifier,
		const Range& range, 
		const Fun& action);

	static Result ParseRangeOfOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		QualifierCode qualifier,
		const Range& range, 
		IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseIndexPrefixedOctetData(
		const GroupVariationRecord& gvRecord,
		openpal::ReadOnlyBuffer& buffer, 
		QualifierCode qualifier,
		uint32_t count, 		
		IAPDUHandler* pHandler);
		
	template <class Target>
	static Result ParseRangeFixedSize(GroupVariation gv, QualifierCode qualifier, openpal::ISerializer<Target>* pSerializer, openpal::ReadOnlyBuffer& buffer, const Range& range, IAPDUHandler* pHandler);

	template <class Descriptor>
	static Result ParseCountOf(openpal::ReadOnlyBuffer& buffer, uint32_t count, IAPDUHandler* pHandler);

	template <class Target, class IndexType>
	static Result ParseCountFixedSizeWithIndex(
		GroupVariation gv,
		QualifierCode qualifier,
		openpal::ReadOnlyBuffer& buffer, 
		uint32_t count, 		
		openpal::ISerializer<Target>* pSerializer,
		IAPDUHandler* pHandler);	

	static IndexedValue<Binary, uint16_t> BoolToBinary(const IndexedValue<bool, uint16_t>& v);
	static IndexedValue<BinaryOutputStatus, uint16_t> BoolToBinaryOutputStatus(const IndexedValue<bool, uint16_t>& v);
};

template <class IndexType>
APDUParser::Result APDUParser::ParseCountHeader(openpal::ReadOnlyBuffer& buffer, Context& context, QualifierCode qualifier, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler)
{
	typename IndexType::Type count;
	auto res = ParseCount<IndexType>(buffer, context, count);
	if(res == Result::OK)
	{			
		if(context.ExpectsContents()) 
		{						
			return ParseObjectsWithRange(qualifier, buffer, gvRecord, Range::FromCount(count), pHandler);
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
APDUParser::Result APDUParser::ParseRangeHeader(openpal::ReadOnlyBuffer& buffer, Context& context, QualifierCode qualifier, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler)
{
	Range range;				
	auto res = ParseRange<ParserType, CountType>(buffer, context, range);
	if(res == Result::OK)
	{
		if(context.ExpectsContents()) 
		{
			return ParseObjectsWithRange(qualifier, buffer, gvRecord, range, pHandler);
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
APDUParser::Result APDUParser::ParseIndexPrefixHeader(openpal::ReadOnlyBuffer& buffer, Context& context, QualifierCode qualifier, const GroupVariationRecord& gvRecord, IAPDUHandler* pHandler)
{
	typename IndexType::Type count;
	auto res = ParseCount<IndexType>(buffer, context, count);
	if(res == Result::OK)
	{
		return ParseObjectsWithIndexPrefix<IndexType>(qualifier, buffer, gvRecord, count, pHandler);
	}
	else 
	{
		return res;
	}	
}

template <class Callback>
APDUParser::Result APDUParser::ParseRangeAsBitField(	
	openpal::ReadOnlyBuffer& buffer,
	QualifierCode qualifier,
	const Range& range, 
	const Callback& callback)
{
	uint32_t numBytes = NumBytesInBits(range.Count());
	if(buffer.Size() < numBytes) return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {	
		auto collection = IterableTransforms<IndexedValue<bool, uint16_t>>::From(buffer, range.Count(),
			[&](openpal::ReadOnlyBuffer& buffer, uint32_t pos) {
			return IndexedValue<bool, uint16_t>(GetBit(buffer, pos), pos + range.start);
			}
		);
		callback(qualifier, collection);
		buffer.Advance(numBytes);
		return Result::OK;
	}
}

template <class IndexType>
APDUParser::Result APDUParser::ParseIndexPrefixedOctetData(
	const GroupVariationRecord& gvRecord,
	openpal::ReadOnlyBuffer& buffer,
	QualifierCode qualifier,
	uint32_t count,
	IAPDUHandler* pHandler)
{
	uint32_t size = count * (IndexType::Size + gvRecord.variation);
	if (buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {

		if (pHandler) {
			auto iterable = IterableTransforms<IndexedValue<OctetString, uint16_t>>::From(buffer, count,
				[&](ReadOnlyBuffer& buffer, uint32_t position) {
				auto index = IndexType::ReadBuffer(buffer);
				OctetString octets(buffer.Truncate(gvRecord.variation));
				buffer.Advance(gvRecord.variation);
				return IndexedValue<OctetString, uint16_t>(octets, index);
			}
			);
			pHandler->OnIndexPrefix(gvRecord.enumeration, qualifier, iterable);
		}

		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class IndexType>
APDUParser::Result APDUParser::ParseObjectsWithIndexPrefix(QualifierCode qualifier, openpal::ReadOnlyBuffer& buffer, const GroupVariationRecord& gvRecord, uint32_t count, IAPDUHandler* pHandler)
{
	switch (gvRecord.enumeration)
	{
		case(GroupVariation::Group2Var1):
			return ParseCountFixedSizeWithIndex<Binary, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group2Var1Serializer::Inst(), pHandler);
		case(GroupVariation::Group2Var2) :
			return ParseCountFixedSizeWithIndex<Binary, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group2Var2Serializer::Inst(), pHandler);
		case(GroupVariation::Group2Var3) :
			return ParseCountFixedSizeWithIndex<Binary, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group2Var3Serializer::Inst(), pHandler);

		case(GroupVariation::Group12Var1) :
			return ParseCountFixedSizeWithIndex<ControlRelayOutputBlock, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group12Var1Serializer::Inst(), pHandler);

		case(GroupVariation::Group22Var1) :
			return ParseCountFixedSizeWithIndex<Counter, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group22Var1Serializer::Inst(), pHandler);
		case(GroupVariation::Group22Var2) :
			return ParseCountFixedSizeWithIndex<Counter, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group22Var2Serializer::Inst(), pHandler);
		case(GroupVariation::Group22Var5) :
			return ParseCountFixedSizeWithIndex<Counter, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group22Var5Serializer::Inst(), pHandler);
		case(GroupVariation::Group22Var6) :
			return ParseCountFixedSizeWithIndex<Counter, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group22Var6Serializer::Inst(), pHandler);
		
		case(GroupVariation::Group32Var1) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var1Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var2) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var2Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var3) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var3Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var4) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var4Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var5) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var5Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var6) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var6Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var7) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var7Serializer::Inst(), pHandler);
		case(GroupVariation::Group32Var8) :
			return ParseCountFixedSizeWithIndex<Analog, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group32Var8Serializer::Inst(), pHandler);

		case(GroupVariation::Group41Var1) :
			return ParseCountFixedSizeWithIndex<AnalogOutputInt32, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group41Var1Serializer::Inst(), pHandler);
		case(GroupVariation::Group41Var2) :
			return ParseCountFixedSizeWithIndex<AnalogOutputInt16, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group41Var2Serializer::Inst(), pHandler);
		case(GroupVariation::Group41Var3) :
			return ParseCountFixedSizeWithIndex<AnalogOutputFloat32, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group41Var3Serializer::Inst(), pHandler);
		case(GroupVariation::Group41Var4) :
			return ParseCountFixedSizeWithIndex<AnalogOutputDouble64, IndexType>(gvRecord.enumeration, qualifier, buffer, count, Group41Var4Serializer::Inst(), pHandler);
		
		case(GroupVariation::Group111AnyVar) :
			return ParseIndexPrefixedOctetData<IndexType>(gvRecord, buffer, qualifier, count, pHandler);

		default:
			return Result::ILLEGAL_OBJECT_QUALIFIER;
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
			return context.AddObjectCount(count) ? Result::UNREASONABLE_OBJECT_COUNT : Result::OK;
		}		
	}
}



template <class Target>
APDUParser::Result APDUParser::ParseRangeFixedSize(GroupVariation gv, QualifierCode qualifier, openpal::ISerializer<Target>* pSerializer, openpal::ReadOnlyBuffer& buffer, const Range& range, IAPDUHandler* pHandler)
{	
	uint32_t size = range.Count() * pSerializer->Size();
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
	
		if(pHandler) {
			auto collection = IterableTransforms<IndexedValue<Target, uint16_t>>::From(buffer, range.Count(), [range, pSerializer](openpal::ReadOnlyBuffer& buffer, uint32_t pos) {
				return IndexedValue<Target, uint16_t>(pSerializer->Read(buffer), range.start + pos);
			});
			pHandler->OnRange(gv, qualifier, collection);
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

template <class Target, class IndexType>
APDUParser::Result APDUParser::ParseCountFixedSizeWithIndex(
	GroupVariation gv,
	QualifierCode qualifier,
	openpal::ReadOnlyBuffer& buffer, 
	uint32_t count, 	
	openpal::ISerializer<Target>* pSerializer,
	IAPDUHandler* pHandler)
{
	uint32_t size = count * (IndexType::Size + pSerializer->Size());
	if(buffer.Size() < size) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	else {
		if(pHandler) {
			auto collection = IterableTransforms<IndexedValue<Target, typename IndexType::Type>>::From(buffer, count, [pSerializer](openpal::ReadOnlyBuffer& buffer, uint32_t) {
				auto index = IndexType::ReadBuffer(buffer);
				auto value = pSerializer->Read(buffer);
				return IndexedValue<Target, typename IndexType::Type>(value, index);
			});
			pHandler->OnIndexPrefix(gv, qualifier, collection);
		}
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}



}

#endif
