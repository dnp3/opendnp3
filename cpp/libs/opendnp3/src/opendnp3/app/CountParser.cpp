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

#include "CountParser.h"

#include "opendnp3/ErrorCodes.h"
#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

CountParser::CountParser(uint16_t count_, uint32_t requiredSize_, HandleFun handler_) :
	count(count_),
	requiredSize(requiredSize_),
	handler(handler_)
{
	
}

ParseResult CountParser::Process(const HeaderRecord& record, openpal::ReadBufferView& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger) const
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
			handler(record, count, buffer, *pHandler);			
		}	
		buffer.Advance(requiredSize);
		return ParseResult::OK;
	}
}

ParseResult CountParser::ParseHeader(openpal::ReadBufferView& buffer, const NumParser& numParser, const ParserSettings& settings, const HeaderRecord& record, openpal::Logger* pLogger, IAPDUHandler* pHandler)
{
	uint16_t count;
	auto result = numParser.ParseCount(buffer, count, pLogger);
	if (result == ParseResult::OK)
	{
		FORMAT_LOGGER_BLOCK(pLogger, settings.Filters(),
			"%03u,%03u %s, %s [%u]",
			record.group,
			record.variation,
			GroupVariationToString(record.enumeration),
			QualifierCodeToString(record.GetQualifierCode()),
			count);

		if (settings.ExpectsContents())
		{
			return ParseCountOfObjects(buffer, record, count, pLogger, pHandler);
		}
		else
		{
			if (pHandler)
			{
				pHandler->OnCountRequest(record, count);
			}

			return ParseResult::OK;
		}
	}
	else
	{
		return result;
	}
}

ParseResult CountParser::ParseCountOfObjects(openpal::ReadBufferView& buffer, const HeaderRecord& record, uint16_t count, openpal::Logger* pLogger, IAPDUHandler* pHandler)
{
	switch (record.enumeration)
	{
		case(GroupVariation::Group50Var1) :
			return CountParser::From<Group50Var1>(count).Process(record, buffer, pHandler, pLogger);

		case(GroupVariation::Group51Var1) :
			return CountParser::From<Group51Var1>(count).Process(record, buffer, pHandler, pLogger);

		case(GroupVariation::Group51Var2) :
			return CountParser::From<Group51Var2>(count).Process(record, buffer, pHandler, pLogger);

		case(GroupVariation::Group52Var2) :
			return CountParser::From<Group52Var2>(count).Process(record, buffer, pHandler, pLogger);

		default:
			return ParseResult::INVALID_OBJECT_QUALIFIER;
	}
}

}


