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

#include "FreeFormatParser.h"

#include "opendnp3/app/parsing/ObjectHeaderParser.h"
#include "opendnp3/app/GroupVariationRecord.h"
#include "opendnp3/LogLevels.h"

#include <openpal/serialization/Serialization.h>
#include <openpal/logging/LogMacros.h>


using namespace openpal;

namespace opendnp3
{
	ParseResult FreeFormatParser::ParseHeader(openpal::ReadBufferView& objects, const ParserSettings& settings, const HeaderRecord& record, openpal::Logger* pLogger, IAPDUHandler* pHandler)
	{
		if (objects.Size() < UInt16::Size)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for two byte free format length");
			return ParseResult::NOT_ENOUGH_DATA_FOR_HEADER;
		}
		else
		{
			const uint16_t SIZE = UInt16::ReadBuffer(objects);
			if (SIZE == objects.Size()) // must be exactly equal to the remainder
			{
				switch (record.enumeration)
				{
					case(GroupVariation::Group120Var1) :
						return ParseFreeFormat(ParseAny<Group120Var1>, record, SIZE, objects, pHandler, pLogger);

					case(GroupVariation::Group120Var2) :
						return ParseFreeFormat(ParseAny<Group120Var2>, record, SIZE, objects, pHandler, pLogger);

					case(GroupVariation::Group120Var6) :
						return ParseFreeFormat(ParseAny<Group120Var6>, record, SIZE, objects, pHandler, pLogger);

					default:
						FORMAT_LOGGER_BLOCK(
							pLogger, flags::WARN,
							"Unknown object (%i, %i) and qualifer (%i) in AuthRequest",
							record.group,
							record.variation,
							record.qualifier
							);
						return ParseResult::INVALID_OBJECT_QUALIFIER;
				}
			}
			else
			{
				FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unexpected size (%i) in free format (%i, %i)", SIZE, record.group, record.variation);
				return objects.Size() < SIZE ? ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS : ParseResult::TOO_MUCH_DATA_FOR_OBJECTS;
			}
		}
	}

	ParseResult FreeFormatParser::ParseFreeFormat(FreeFormatHandler parser, const HeaderRecord& record, uint16_t size, openpal::ReadBufferView& objects, IAPDUHandler* pHandler, openpal::Logger* pLogger)
	{				
		if (parser(record, objects, pHandler))
		{
			return ParseResult::OK;
		}
		else
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for (%i, %i)", record.group, record.variation);
			return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
		}		
	}

}