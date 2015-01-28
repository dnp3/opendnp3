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

#include "RangeParser.h"

#include "opendnp3/ErrorCodes.h"

#include "opendnp3/objects/Group1.h"

#include "opendnp3/objects/Group3.h"


#include "opendnp3/objects/Group10.h"
#include "opendnp3/objects/Group20.h"
#include "opendnp3/objects/Group21.h"
#include "opendnp3/objects/Group30.h"
#include "opendnp3/objects/Group40.h"
#include "opendnp3/objects/Group50.h"

/*
#include "opendnp3/objects/Group41.h"
#include "opendnp3/objects/Group42.h"
#include "opendnp3/objects/Group43.h"
#include "opendnp3/objects/Group51.h"
#include "opendnp3/objects/Group52.h"
*/


#include <openpal/logging/LogMacros.h>



namespace opendnp3
{

RangeParser::RangeParser(const Range& range_, uint32_t requiredSize_, HandleFun handler_) :
	range(range_),
	requiredSize(requiredSize_),
	handler(handler_)
{

}

ParseResult RangeParser::ParseHeader(openpal::ReadBufferView& buffer, const NumParser& numparser, const ParserSettings& settings, const HeaderRecord& record, openpal::Logger* pLogger, IAPDUHandler* pHandler)
{
	Range range;
	auto res = numparser.ParseRange(buffer, range, pLogger);
	if (res == ParseResult::OK)
	{
		FORMAT_LOGGER_BLOCK(pLogger, settings.Filters(),
			"%03u,%03u %s, %s [%u, %u]",
			record.group,
			record.variation,
			GroupVariationToString(record.enumeration),
			QualifierCodeToString(record.GetQualifierCode()),
			range.start,
			range.stop);

		if (settings.ExpectsContents())
		{
			return ParseRangeOfObjects(buffer, record, range, pLogger, pHandler);
		}
		else
		{
			if (pHandler)
			{
				pHandler->OnRangeRequest(record, range);
			}
			return ParseResult::OK;
		}
	}
	else
	{
		return res;
	}
}

ParseResult RangeParser::Process(const HeaderRecord& record, openpal::ReadBufferView& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger) const
{
	if (buffer.Size() < requiredSize)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified objects");
		return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{
		if (pHandler)
		{
			handler(record, range, buffer, *pHandler);
		}
		buffer.Advance(requiredSize);
		return ParseResult::OK;
	}
}

#define MACRO_PARSE_OBJECTS_WITH_RANGE(descriptor) \
	case(GroupVariation::descriptor): \
	return RangeParser::FromFixedSize<descriptor>(range).Process(record, buffer, pHandler, pLogger);	

ParseResult RangeParser::ParseRangeOfObjects(openpal::ReadBufferView& buffer, const HeaderRecord& record, const Range& range, openpal::Logger* pLogger, IAPDUHandler* pHandler)
{
	switch (record.enumeration)
	{		
		case(GroupVariation::Group1Var1) :
			return RangeParser::FromBitfield<BitfieldConversion<Binary>>(range).Process(record, buffer, pHandler, pLogger);	
		
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group1Var2);
		
		case(GroupVariation::Group3Var1) :
			return RangeParser::FromDoubleBitfield<DoubleBitfieldConversion<DoubleBitBinary>>(range).Process(record, buffer, pHandler, pLogger);	
		case(GroupVariation::Group10Var1):
			return RangeParser::FromBitfield<BitfieldConversion<BinaryOutputStatus>>(range).Process(record, buffer, pHandler, pLogger);
		
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group3Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group10Var2);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var6);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var6);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var9);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var10);

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

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group50Var4);
				
	case(GroupVariation::Group80Var1) :
		return RangeParser::FromBitfield<BitfieldConversion<IINValue>>(range).Process(record, buffer, pHandler, pLogger);
		
	case(GroupVariation::Group110AnyVar) :
		return ParseRangeOfOctetData(buffer, record, range, pLogger, pHandler);

	default:
		FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_ILLEGAL_QUALIFIER_AND_OBJECT,
			"Unsupported qualifier/object - %s - %i / %i",
			QualifierCodeToString(record.GetQualifierCode()), record.group, record.variation);

		return ParseResult::INVALID_OBJECT_QUALIFIER;
	}
}
	
ParseResult RangeParser::ParseRangeOfOctetData(openpal::ReadBufferView& buffer, const HeaderRecord& record, const Range& range, openpal::Logger* pLogger, IAPDUHandler* pHandler)
{
	if (record.variation > 0)
	{
		uint32_t size = record.variation * range.Count();
		if (buffer.Size() < size)
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified octet objects");
			return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
		}
		else
		{
			if (pHandler)
			{
				auto reader = [record, range](openpal::ReadBufferView & buff, uint32_t pos)
				{
					OctetString octets(buff.Take(record.variation));
					IndexedValue<OctetString, uint16_t> value(octets, range.start + pos);
					buff.Advance(record.variation);
					return value;
				};

				auto collection = CreateLazyIterable<IndexedValue<OctetString, uint16_t>>(buffer, range.Count(), reader);
				pHandler->OnRange(record, collection);
			}
			buffer.Advance(size);
			return ParseResult::OK;
		}
	}
	else
	{
		SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Octet string variation 0 may only be used in requests");
		return ParseResult::INVALID_OBJECT;
	}
}


}


