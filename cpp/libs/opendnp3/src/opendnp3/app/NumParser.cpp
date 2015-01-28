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

#include "NumParser.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/serialization/Serialization.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/ErrorCodes.h"

using namespace openpal;

namespace opendnp3
{

NumParser::NumParser(ReadFun pReadFun_, uint8_t size_) : 
	pReadFun(pReadFun_), 
	size(size_)
{
	
}

uint8_t NumParser::NumBytes() const
{
	return size;
}

ParseResult NumParser::ParseCount(openpal::ReadBufferView& buffer, uint16_t& count, openpal::Logger* pLogger) const
{
	if (this->Read(count, buffer))
	{
		if (count == 0)
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_COUNT_OF_ZERO, "count of 0");
			return ParseResult::COUNT_OF_ZERO;
		}
		else
		{
			return ParseResult::OK;
		}
	}
	else
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_HEADER, "Not enough data for count");
		return ParseResult::NOT_ENOUGH_DATA_FOR_RANGE;
	}
}

ParseResult NumParser::ParseRange(openpal::ReadBufferView& buffer, Range& range, openpal::Logger* pLogger) const
{
	if (buffer.Size() < (2 * static_cast<uint32_t>(size)))
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_HEADER, "Not enough data for start / stop");
		return ParseResult::NOT_ENOUGH_DATA_FOR_RANGE;
	}
	else
	{		
		this->Read(range.start, buffer);
		this->Read(range.stop, buffer);

		if (range.IsValid())
		{
			return ParseResult::OK;			
		}
		else
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_START_STOP_MISMATCH, "start > stop");
			return ParseResult::BAD_START_STOP;			
		}
	}
}

bool NumParser::Read(uint16_t& num, openpal::ReadBufferView& buffer) const
{
	if (buffer.Size() < size)
	{
		return false;
	}
	else
	{
		num = pReadFun(buffer);
		return true;
	}
}

uint16_t NumParser::ReadByte(openpal::ReadBufferView& buffer)
{
	return UInt8::ReadBuffer(buffer);
}

NumParser NumParser::OneByte()
{
	return NumParser(&ReadByte, 1);
}

NumParser NumParser::TwoByte()
{
	return NumParser(&UInt16::ReadBuffer, 2);
}

}


