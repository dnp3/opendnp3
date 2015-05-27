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

#include "AggressiveModeParser.h"

#include <opendnp3/app/parsing/ObjectHeaderParser.h>
#include <opendnp3/app/GroupVariationRecord.h>
#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>
#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;

namespace secauth
{
	AggModeResult::AggModeResult(opendnp3::ParseResult result_) :
		result(result_),
		isAggMode(false)
	{
	
	}
	
	AggModeResult::AggModeResult(const opendnp3::Group120Var3& request_, const openpal::ReadBufferView& remainder_) :
		result(ParseResult::OK),
		isAggMode(true),
		request(request_),
		remainder(remainder_)
	{
	
	}

	// failure constructor
	AggModeHMACResult::AggModeHMACResult(opendnp3::ParseResult result_) : result(result_)
	{
	
	}

	// success constructor
	AggModeHMACResult::AggModeHMACResult(const opendnp3::Group120Var9& hmac_, const openpal::ReadBufferView& objects_) :
		result(ParseResult::OK),
		hmac(hmac_),
		objects(objects_)
	{
	
	}

	AggModeResult AggressiveModeParser::IsAggressiveMode(openpal::ReadBufferView objects, openpal::Logger* pLogger)
	{		
		ObjectHeader header;
		auto result = ObjectHeaderParser::ParseObjectHeader(header, objects, pLogger);
		if (result != ParseResult::OK)
		{
			return AggModeResult(result);
		}
		
		auto record = GroupVariationRecord::GetRecord(header.group, header.variation);

		if (record.enumeration != GroupVariation::Group120Var3)
		{
			return AggModeResult(result);
		}

		if (QualifierCodeFromType(header.qualifier) != QualifierCode::UINT8_CNT)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Aggressive mode request contains bad qualifier (%u)", header.qualifier);
			return AggModeResult(ParseResult::INVALID_OBJECT_QUALIFIER);
		}

		if (objects.Size() < UInt8::SIZE)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Insufficient data for count");
			return AggModeResult(ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
		}
			
		uint8_t count = UInt8::ReadBuffer(objects);

		if (count != 1)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Aggressive mode request contains bad count (%u)", count);
			return AggModeResult(ParseResult::NOT_ON_WHITELIST);
		}							

		Group120Var3 value;
		if (Group120Var3::Read(objects, value))
		{
			return AggModeResult(value, objects);
		}
		else
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Insufficient data for g120v3");
			return AggModeResult(ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
		}	
	}

	AggModeHMACResult AggressiveModeParser::ParseHMAC(openpal::ReadBufferView remainder, uint32_t hmacSize, openpal::Logger* pLogger)
	{
		// given the hmac size, how many bytes would be required for the header + HMAC
		// 0x 78 09 5F 01 SS SS [HMACSize]
		const uint32_t TRAILER_SIZE = 6 + hmacSize;
		if (remainder.Size() < TRAILER_SIZE)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for aggressive mode hmac with expected length of (%u)", hmacSize);
			return AggModeHMACResult(ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
		}

		const uint32_t OBJECTS_SIZE = remainder.Size() - TRAILER_SIZE;

		// partition the remainder into the two pieces
		auto objects = remainder.Take(OBJECTS_SIZE);
		auto trailer = remainder.Skip(OBJECTS_SIZE);

		ObjectHeader header;
		auto result = ObjectHeaderParser::ParseObjectHeader(header, trailer, pLogger);
		if (result != ParseResult::OK)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for aggressive mode hmac header");
			return AggModeHMACResult(result);
		}

		auto record = GroupVariationRecord::GetRecord(header.group, header.variation);
		if (record.enumeration != GroupVariation::Group120Var9)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Aggressive mode request doesn't contain g120v9 at expected position");
			return AggModeHMACResult(ParseResult::UNKNOWN_OBJECT);
		}

		if (QualifierCodeFromType(header.qualifier) != QualifierCode::UINT16_FREE_FORMAT)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Aggressive mode hmac contains unexpected qualifier (%u)", header.qualifier);
			return AggModeHMACResult(ParseResult::UNKNOWN_QUALIFIER);
		}

		uint8_t count = 0;
		uint16_t size = 0;
		if (!openpal::Parse::Many(trailer, count, size))
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for free-format count and/or size");
			return AggModeHMACResult(ParseResult::NOT_ENOUGH_DATA_FOR_HEADER);
		}

		if (size != trailer.Size())
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Agg mode free-format header doesn't contain expected data");
			return AggModeHMACResult(ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
		}

		if (size == hmacSize)
		{
			Group120Var9 hmac(trailer);
			return AggModeHMACResult(hmac, objects);
		}
		else
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Actual length of hmac (%u) doesn't match expected length of (%u)", size, hmacSize);
			return AggModeHMACResult(ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS);
		}
	}

}
