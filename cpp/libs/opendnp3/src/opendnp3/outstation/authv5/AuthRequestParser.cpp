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

#include "AuthRequestParser.h"

#include "opendnp3/app/parsing/ObjectHeaderParser.h"
#include "opendnp3/app/GroupVariationRecord.h"
#include "opendnp3/LogLevels.h"

#include <openpal/serialization/Serialization.h>
#include <openpal/logging/LogMacros.h>


using namespace openpal;

namespace opendnp3
{

	ParseResult AuthRequestParser::Parse(const ReadBufferView& objects, IAuthRequestParserHandler& handler, Logger* pLogger)
	{
		ObjectHeader ohdr;
		ReadBufferView copy(objects);

		auto result = ObjectHeaderParser::ParseObjectHeader(ohdr, copy, pLogger);
		
		if (result == ParseResult::OK)
		{
			HeaderRecord record(GroupVariationRecord::GetRecord(ohdr.group, ohdr.variation), ohdr.qualifier);
			switch (record.GetQualifierCode())
			{
				case(QualifierCode::UINT8_CNT) :
					return ParseOneOctetCount(record, copy, handler, pLogger);
				case(QualifierCode::UINT16_FREE_FORMAT):
					return ParseTwoOctetFreeFormat(record, copy, handler, pLogger);
				default:
					FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unsupported qualifier code in AuthRequest: %i", ohdr.qualifier);
					return ParseResult::UNKNOWN_QUALIFIER;
			}
		}
		else
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for object header");
			return result;
		}
	}

	ParseResult AuthRequestParser::ParseOneOctetCount(const HeaderRecord& record, openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler, openpal::Logger* pLogger)
	{
		if (objects.Size() < 1)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for one byte count");
			return ParseResult::NOT_ENOUGH_DATA_FOR_HEADER;
		}
		else
		{
			uint8_t count = UInt8::ReadBuffer(objects);
			if (count == 1) // only a count of 1 is allowed in this parser
			{
				switch (record.enumeration)
				{
					case(GroupVariation::Group120Var4) :
						return ParseRequestSessionKeyStatus(record, objects, handler, pLogger);

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
				FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Bad count in AuthRequest: %i", count);
				return ParseResult::BAD_COUNT;
			}
		}		
	}

	ParseResult AuthRequestParser::ParseRequestSessionKeyStatus(const HeaderRecord& record, openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler, openpal::Logger* pLogger)
	{
		if (objects.Size() == Group120Var4::Size())
		{
			auto request = Group120Var4::Read(objects);
			handler.OnRequestKeyStatus(request);
			return ParseResult::OK;
		}
		else
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unexpected size in (%i, %i)", record.group, record.variation);
			return (objects.Size() < Group120Var4::Size()) ? ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS : ParseResult::TOO_MUCH_DATA_FOR_OBJECTS;
		}
	}

	ParseResult AuthRequestParser::ParseTwoOctetFreeFormat(const HeaderRecord& record, openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler, openpal::Logger* pLogger)
	{
		if (objects.Size() < 2)
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
						return ParseFreeFormat(ParseAuthChallenge, record, SIZE, objects, handler, pLogger);

					case(GroupVariation::Group120Var2) :
						return ParseFreeFormat(ParseAuthReply, record, SIZE, objects, handler, pLogger);

					case(GroupVariation::Group120Var6) :
						return ParseFreeFormat(ParseSessionKeyChange, record, SIZE, objects, handler, pLogger);

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

	bool AuthRequestParser::ParseAuthChallenge(openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler)
	{		
			Group120Var1 challenge;
			auto success = Group120Var1::Read(objects, challenge);
			if (success)
			{
				handler.OnAuthChallenge(challenge);			
			}
			return success;
	}

	bool AuthRequestParser::ParseAuthReply(openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler)
	{		
		Group120Var2 reply;
		auto success = Group120Var2::Read(objects, reply);
		if (success)
		{
			handler.OnAuthReply(reply);			
		}
		return success;
	}

	bool AuthRequestParser::ParseSessionKeyChange(openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler)
	{
		Group120Var6 keyChange;
		auto success = Group120Var6::Read(objects, keyChange);
		if (success)
		{
			handler.OnChangeSessionKeys(keyChange);
		}
		return success;
	}

	ParseResult AuthRequestParser::ParseFreeFormat(FreeFormatHandler parser, const HeaderRecord& record, uint16_t size, openpal::ReadBufferView& objects, IAuthRequestParserHandler& handler, openpal::Logger* pLogger)
	{				
		if (parser(objects, handler))
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