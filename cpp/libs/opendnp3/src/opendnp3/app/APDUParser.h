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
#ifndef OPENDNP3_APDUPARSER_H
#define OPENDNP3_APDUPARSER_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/logging/Logger.h>
#include <openpal/logging/LogMacros.h>
#include <openpal/serialization/Serializer.h>
#include <openpal/serialization/Serialization.h>

#include <openpal/util/Uncopyable.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/ErrorCodes.h"

#include "opendnp3/app/Range.h"
#include "opendnp3/app/IAPDUHandler.h"
#include "opendnp3/app/LazyIterable.h"
#include "opendnp3/app/IndexParser.h"
#include "opendnp3/app/BitReader.h"
#include "opendnp3/app/IterableTransforms.h"

#include "opendnp3/objects/Group1.h"
#include "opendnp3/objects/Group2.h"
#include "opendnp3/objects/Group3.h"
#include "opendnp3/objects/Group4.h"
#include "opendnp3/objects/Group10.h"
#include "opendnp3/objects/Group11.h"
#include "opendnp3/objects/Group12.h"
#include "opendnp3/objects/Group20.h"
#include "opendnp3/objects/Group21.h"
#include "opendnp3/objects/Group22.h"
#include "opendnp3/objects/Group23.h"
#include "opendnp3/objects/Group30.h"
#include "opendnp3/objects/Group32.h"
#include "opendnp3/objects/Group40.h"
#include "opendnp3/objects/Group41.h"
#include "opendnp3/objects/Group50.h"
#include "opendnp3/objects/Group51.h"
#include "opendnp3/objects/Group52.h"

namespace opendnp3
{

class APDUParser : private openpal::PureStatic
{
public:

	class Context
	{
	public:

		static Context Default()
		{
			return Context();
		}

		Context(bool expectContents_ = true, int32_t logFilters_ = flags::APP_OBJECT_RX) :
			expectContents(expectContents_),
			logFilters(logFilters_)			
		{}

		inline bool ExpectsContents() const
		{
			return expectContents;
		}		

		inline int32_t Filters() const
		{
			return logFilters;
		}

	private:


		const bool expectContents;
		int32_t logFilters;		
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
	    INVALID_OBJECT_QUALIFIER,
	    INVALID_OBJECT,
	    BAD_START_STOP,
	    COUNT_OF_ZERO
	};

	static Result ParseTwoPass(const openpal::ReadOnlyBuffer& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger, Context context = Context::Default());

private:

	static Result ParseSinglePass(const openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, IAPDUHandler* pHandler, Context context);

	static Result ParseHeaders(const openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context context, IAPDUHandler* pHandler);

	static Result ParseHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseCountHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, const HeaderRecord& record, IAPDUHandler* pHandler);

	template <class ParserType, class CountType>
	static Result ParseRangeHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, const HeaderRecord& record, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseIndexPrefixHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, const HeaderRecord& record, IAPDUHandler* pHandler);

	template <class ParserType, class CountType>
	static Result ParseRange(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, Range& range);

	template <class ParserType>
	static Result ParseCount(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, typename ParserType::Type& count);

	static Result ParseRangeOfObjects(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, const Range& range, IAPDUHandler* pHandler);

	static Result ParseCountOfObjects(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, uint16_t count, IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseObjectsWithIndexPrefix(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, uint32_t count, IAPDUHandler* pHandler);

	template <class Fun>
	static Result ParseRangeAsBitField(
	    openpal::ReadOnlyBuffer& buffer,
	    openpal::Logger* pLogger,
	    const HeaderRecord& record,
	    const Range& range,
	    const Fun& action);

	template <class Fun>
	static Result ParseRangeAsDoubleBitField(
	    openpal::ReadOnlyBuffer& buffer,
	    openpal::Logger* pLogger,
	    const HeaderRecord& record,
	    const Range& range,
	    const Fun& action);

	static Result ParseRangeOfOctetData(
	    openpal::ReadOnlyBuffer& buffer,
	    openpal::Logger* pLogger,
	    const HeaderRecord& record,
	    const Range& range,
	    IAPDUHandler* pHandler);

	template <class IndexType>
	static Result ParseIndexPrefixedOctetData(
	    openpal::ReadOnlyBuffer& buffer,
	    openpal::Logger* pLogger,
	    const HeaderRecord& record,
	    uint32_t count,
	    IAPDUHandler* pHandler);

	template <class Target>
	static Result ParseRangeFixedSize(const HeaderRecord& record, const openpal::Serializer<Target>& serializer, openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const Range& range, IAPDUHandler* pHandler);

	template <class Descriptor>
	static Result ParseCountOf(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, uint32_t count, IAPDUHandler* pHandler);

	template <class Target, class IndexType>
	static Result ParseCountFixedSizeWithIndex(
	    const HeaderRecord& record,
	    openpal::ReadOnlyBuffer& buffer,
	    openpal::Logger* pLogger,
	    uint32_t count,
	    const openpal::Serializer<Target>& serializer,
	    IAPDUHandler* pHandler);

	static IndexedValue<Binary, uint16_t> BoolToBinary(const IndexedValue<bool, uint16_t>& v);
	static IndexedValue<BinaryOutputStatus, uint16_t> BoolToBinaryOutputStatus(const IndexedValue<bool, uint16_t>& v);
};

template <class IndexType>
APDUParser::Result APDUParser::ParseCountHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, const HeaderRecord& record, IAPDUHandler* pHandler)
{
	typename IndexType::Type count;
	auto res = ParseCount<IndexType>(buffer, pLogger, context, count);
	if(res == Result::OK)
	{
		FORMAT_LOGGER_BLOCK(pLogger, context.Filters(),
			"%03u,%03u %s, %s [%u]",
			record.group,
			record.variation,
			GroupVariationToString(record.enumeration),			
			QualifierCodeToString(record.qualifier),
			count);		

		if(context.ExpectsContents())
		{
			return ParseCountOfObjects(buffer, pLogger, record, count, pHandler);
		}
		else
		{
			if(pHandler)
			{
				pHandler->OnCountRequest(record, count);
			}

			return Result::OK;
		}
	}
	else
	{
		return res;
	}
}

template <class ParserType, class CountType>
APDUParser::Result APDUParser::ParseRangeHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, const HeaderRecord& record, IAPDUHandler* pHandler)
{
	Range range;
	auto res = ParseRange<ParserType, CountType>(buffer, pLogger, context, range);
	if(res == Result::OK)
	{
		FORMAT_LOGGER_BLOCK(pLogger, context.Filters(),
			"%03u,%03u %s, %s [%u, %u]",
			record.group,
			record.variation,
			GroupVariationToString(record.enumeration),
			QualifierCodeToString(record.qualifier),
			range.start,
			range.stop);		

		if(context.ExpectsContents())
		{
			return ParseRangeOfObjects(buffer, pLogger, record, range, pHandler);
		}
		else
		{
			if(pHandler)
			{
				pHandler->OnRangeRequest(record, Range(range.start, range.stop));
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
APDUParser::Result APDUParser::ParseIndexPrefixHeader(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, const HeaderRecord& record, IAPDUHandler* pHandler)
{
	typename IndexType::Type count;
	auto res = ParseCount<IndexType>(buffer, pLogger, context, count);
	if(res == Result::OK)
	{

		FORMAT_LOGGER_BLOCK(pLogger, context.Filters(),
			"%03u,%03u %s, %s [%u]",
			record.group,
			record.variation,
			GroupVariationToString(record.enumeration),
			QualifierCodeToString(record.qualifier),
			count);

		return ParseObjectsWithIndexPrefix<IndexType>(buffer, pLogger, record, count, pHandler);
	}
	else
	{
		return res;
	}
}

template <class Callback>
APDUParser::Result APDUParser::ParseRangeAsBitField(
    openpal::ReadOnlyBuffer& buffer,
    openpal::Logger* pLogger,
    const HeaderRecord& record,
    const Range& range,
    const Callback& callback)
{
	uint32_t numBytes = NumBytesInBits(range.Count());
	if (buffer.Size() < numBytes)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified bitfield objects");
		return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{
		auto collection = CreateLazyIterable<IndexedValue<bool, uint16_t>>(buffer, range.Count(),
		                  [range](openpal::ReadOnlyBuffer & buffer, uint32_t pos)
		{
			return IndexedValue<bool, uint16_t>(GetBit(buffer, pos), pos + range.start);
		}
		                                                                  );
		callback(collection);
		buffer.Advance(numBytes);
		return Result::OK;
	}
}

template <class Callback>
APDUParser::Result APDUParser::ParseRangeAsDoubleBitField(
    openpal::ReadOnlyBuffer& buffer,
    openpal::Logger* pLogger,
    const HeaderRecord& record,
    const Range& range,
    const Callback& callback)
{
	uint32_t numBytes = NumBytesInDoubleBits(range.Count());
	if (buffer.Size() < numBytes)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified double bitfield objects");
		return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{
		auto collection = CreateLazyIterable<IndexedValue<DoubleBit, uint16_t>>(buffer, range.Count(),
		                  [range](openpal::ReadOnlyBuffer & buffer, uint32_t pos)
		{
			return IndexedValue<DoubleBit, uint16_t>(GetDoubleBit(buffer, pos), pos + range.start);
		}
		                                                                       );
		callback(collection);
		buffer.Advance(numBytes);
		return Result::OK;
	}
}

template <class IndexType>
APDUParser::Result APDUParser::ParseIndexPrefixedOctetData(
    openpal::ReadOnlyBuffer& buffer,
    openpal::Logger* pLogger,
    const HeaderRecord& record,
    uint32_t count,
    IAPDUHandler* pHandler)
{
	if (record.variation > 0)
	{
		uint32_t size = count * (IndexType::Size + record.variation);
		if (buffer.Size() < size)
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified bitfield objects");
			return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
		}
		else
		{
			if (pHandler)
			{
				auto iterable = CreateLazyIterable<IndexedValue<OctetString, uint16_t>>(buffer, count,
				                [record](openpal::ReadOnlyBuffer & buff, uint32_t position)
				{
					auto index = IndexType::ReadBuffer(buff);
					OctetString octets(buff.Take(record.variation));
					buff.Advance(record.variation);
					return IndexedValue<OctetString, uint16_t>(octets, index);
				}
				                                                                       );
				pHandler->OnIndexPrefix(record, iterable);
			}

			buffer.Advance(size);
			return APDUParser::Result::OK;
		}
	}
	else
	{
		SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Octet string variation 0 may only be used in requests");
		return Result::INVALID_OBJECT;
	}
}

template <class IndexType>
APDUParser::Result APDUParser::ParseObjectsWithIndexPrefix(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, uint32_t count, IAPDUHandler* pHandler)
{
	switch (record.enumeration)
	{
	case(GroupVariation::Group2Var1):
		return ParseCountFixedSizeWithIndex<Binary, IndexType>(record, buffer, pLogger, count, Group2Var1::Inst(), pHandler);
	case(GroupVariation::Group2Var2) :
		return ParseCountFixedSizeWithIndex<Binary, IndexType>(record, buffer, pLogger, count, Group2Var2::Inst(), pHandler);
	case(GroupVariation::Group2Var3) :
		return ParseCountFixedSizeWithIndex<Binary, IndexType>(record, buffer, pLogger, count, Group2Var3::Inst(), pHandler);

	case(GroupVariation::Group4Var1) :
		return ParseCountFixedSizeWithIndex<DoubleBitBinary, IndexType>(record, buffer, pLogger, count, Group4Var1::Inst(), pHandler);
	case(GroupVariation::Group4Var2) :
		return ParseCountFixedSizeWithIndex<DoubleBitBinary, IndexType>(record, buffer, pLogger, count, Group4Var2::Inst(), pHandler);
	case(GroupVariation::Group4Var3) :
		return ParseCountFixedSizeWithIndex<DoubleBitBinary, IndexType>(record, buffer, pLogger, count, Group4Var3::Inst(), pHandler);

	case(GroupVariation::Group11Var1):
		return ParseCountFixedSizeWithIndex<BinaryOutputStatus, IndexType>(record, buffer, pLogger, count, Group11Var1::Inst(), pHandler);
		
	case(GroupVariation::Group12Var1) :
		return ParseCountFixedSizeWithIndex<ControlRelayOutputBlock, IndexType>(record, buffer, pLogger, count, Group12Var1::Inst(), pHandler);

	case(GroupVariation::Group22Var1) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var1::Inst(), pHandler);
	case(GroupVariation::Group22Var2) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var2::Inst(), pHandler);
	case(GroupVariation::Group22Var5) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var5::Inst(), pHandler);
	case(GroupVariation::Group22Var6) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var6::Inst(), pHandler);

	case(GroupVariation::Group23Var1) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var1::Inst(), pHandler);
	case(GroupVariation::Group23Var2) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var2::Inst(), pHandler);
	case(GroupVariation::Group23Var5) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var5::Inst(), pHandler);
	case(GroupVariation::Group23Var6) :
		return ParseCountFixedSizeWithIndex<Counter, IndexType>(record, buffer, pLogger, count, Group22Var6::Inst(), pHandler);

	case(GroupVariation::Group32Var1) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var1::Inst(), pHandler);
	case(GroupVariation::Group32Var2) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var2::Inst(), pHandler);
	case(GroupVariation::Group32Var3) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var3::Inst(), pHandler);
	case(GroupVariation::Group32Var4) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var4::Inst(), pHandler);
	case(GroupVariation::Group32Var5) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var5::Inst(), pHandler);
	case(GroupVariation::Group32Var6) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var6::Inst(), pHandler);
	case(GroupVariation::Group32Var7) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var7::Inst(), pHandler);
	case(GroupVariation::Group32Var8) :
		return ParseCountFixedSizeWithIndex<Analog, IndexType>(record, buffer, pLogger, count, Group32Var8::Inst(), pHandler);

	case(GroupVariation::Group41Var1) :
		return ParseCountFixedSizeWithIndex<AnalogOutputInt32, IndexType>(record, buffer, pLogger, count, Group41Var1::Inst(), pHandler);
	case(GroupVariation::Group41Var2) :
		return ParseCountFixedSizeWithIndex<AnalogOutputInt16, IndexType>(record, buffer, pLogger, count, Group41Var2::Inst(), pHandler);
	case(GroupVariation::Group41Var3) :
		return ParseCountFixedSizeWithIndex<AnalogOutputFloat32, IndexType>(record, buffer, pLogger, count, Group41Var3::Inst(), pHandler);
	case(GroupVariation::Group41Var4) :
		return ParseCountFixedSizeWithIndex<AnalogOutputDouble64, IndexType>(record, buffer, pLogger, count, Group41Var4::Inst(), pHandler);

	case(GroupVariation::Group111AnyVar) :
		return ParseIndexPrefixedOctetData<IndexType>(buffer, pLogger, record, count, pHandler);

	default:
		
		FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_ILLEGAL_QUALIFIER_AND_OBJECT, 
			"Unsupported qualifier/object - %s - %i / %i",
			QualifierCodeToString(record.qualifier), record.group, record.variation);

		return Result::INVALID_OBJECT_QUALIFIER;
	}
}

template <class ParserType, class RangeType>
APDUParser::Result APDUParser::ParseRange(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, Range& range)
{
	if (buffer.Size() < (2 * ParserType::Size))
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_HEADER, "Not enough data for start / stop");
		return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	}
	else
	{
		auto start = ParserType::ReadBuffer(buffer);
		auto stop = ParserType::ReadBuffer(buffer);
		if (start > stop)
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_START_STOP_MISMATCH, "start > stop");
			return Result::BAD_START_STOP;
		}
		else
		{					
			range.start = start;
			range.stop = stop;
			return Result::OK;			
		}
	}
}

template <class ParserType>
APDUParser::Result APDUParser::ParseCount(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, typename ParserType::Type& count)
{
	if (buffer.Size() < ParserType::Size)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_HEADER, "Not enough data for count");
		return Result::NOT_ENOUGH_DATA_FOR_RANGE;
	}
	else
	{
		count = ParserType::ReadBuffer(buffer);
		if (count == 0)
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_COUNT_OF_ZERO, "count of 0");
			return Result::COUNT_OF_ZERO;
		}
		else
		{			
			return Result::OK;			
		}
	}
}

template <class Target>
APDUParser::Result APDUParser::ParseRangeFixedSize(const HeaderRecord& record, const openpal::Serializer<Target>& serializer, openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const Range& range, IAPDUHandler* pHandler)
{
	uint32_t size = range.Count() * serializer.Size();
	if (buffer.Size() < size)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified objects");
		return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{

		if(pHandler)
		{
			auto pSerializer = &serializer;
			auto collection = CreateLazyIterable<IndexedValue<Target, uint16_t>>(buffer, range.Count(), [range, pSerializer](openpal::ReadOnlyBuffer & buffer, uint32_t pos)
			{
				return IndexedValue<Target, uint16_t>(pSerializer->Read(buffer), range.start + pos);
			});
			pHandler->OnRange(record, collection);
		}
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class Descriptor>
APDUParser::Result APDUParser::ParseCountOf(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, uint32_t count, IAPDUHandler* pHandler)
{
	uint32_t size = count * Descriptor::Size();
	if (buffer.Size() < size)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified objects");
		return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{
		if(pHandler)
		{
			auto collection = CreateLazyIterable<Descriptor>(buffer, count, [](openpal::ReadOnlyBuffer & buffer, uint32_t)
			{
				return Descriptor::Read(buffer);
			});
			pHandler->OnCountOf(record, collection);
		}
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}

template <class Target, class IndexType>
APDUParser::Result APDUParser::ParseCountFixedSizeWithIndex(
    const HeaderRecord& record,
    openpal::ReadOnlyBuffer& buffer,
    openpal::Logger* pLogger,
    uint32_t count,
    const openpal::Serializer<Target>& serializer,
    IAPDUHandler* pHandler)
{
	uint32_t size = count * (IndexType::Size + serializer.Size());
	if (buffer.Size() < size)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified objects");
		return APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{
		if(pHandler)
		{
			auto pSerializer = &serializer;
			auto collection = CreateLazyIterable<IndexedValue<Target, typename IndexType::Type>>(buffer, count,
			                  [pSerializer](openpal::ReadOnlyBuffer & buffer, uint32_t)
			{
				auto index = IndexType::ReadBuffer(buffer);
				auto value = pSerializer->Read(buffer);
				return IndexedValue<Target, typename IndexType::Type>(value, index);
			}
			                                                                                    );
			pHandler->OnIndexPrefix(record, collection);
		}
		buffer.Advance(size);
		return APDUParser::Result::OK;
	}
}



}

#endif
