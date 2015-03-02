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

using namespace std;
using namespace openpal;
using namespace opendnp3;

namespace secauthv5
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

}
