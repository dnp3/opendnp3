/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "NumParser.h"

#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

NumParser::NumParser(ReadFun pReadFun, uint8_t size) : pReadFun(pReadFun), size(size) {}

uint8_t NumParser::NumBytes() const
{
    return size;
}

ParseResult NumParser::ParseCount(ser4cpp::rseq_t& buffer, uint16_t& count, Logger* pLogger) const
{
    if (this->Read(count, buffer))
    {
        if (count == 0)
        {
            SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "count of 0");
            return ParseResult::COUNT_OF_ZERO;
        }

        return ParseResult::OK;
    }
    else
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for count");
        return ParseResult::NOT_ENOUGH_DATA_FOR_RANGE;
    }
}

ParseResult NumParser::ParseRange(ser4cpp::rseq_t& buffer, Range& range, Logger* pLogger) const
{
    if (buffer.length() < (2 * static_cast<size_t>(size)))
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for start / stop");
        return ParseResult::NOT_ENOUGH_DATA_FOR_RANGE;
    }

    range.start = this->ReadNum(buffer);
    range.stop = this->ReadNum(buffer);

    if (range.IsValid())
    {
        return ParseResult::OK;
    }

    FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "start (%u) > stop (%u)", range.start, range.stop);
    return ParseResult::BAD_START_STOP;
}

uint16_t NumParser::ReadNum(ser4cpp::rseq_t& buffer) const
{
    return pReadFun(buffer);
}

bool NumParser::Read(uint16_t& num, ser4cpp::rseq_t& buffer) const
{
    if (buffer.length() < size)
    {
        return false;
    }

    num = pReadFun(buffer);
    return true;
}

uint16_t NumParser::ReadOneByte(ser4cpp::rseq_t& buffer)
{
    uint8_t result = 0;
    ser4cpp::UInt8::read_from(buffer, result);
    return result;
}

uint16_t NumParser::ReadTwoBytes(ser4cpp::rseq_t& buffer)
{
    uint16_t result = 0;
    ser4cpp::UInt16::read_from(buffer, result);
    return result;
}

NumParser NumParser::OneByte()
{
    return NumParser(&ReadOneByte, 1);
}

NumParser NumParser::TwoByte()
{
    return NumParser(&ReadTwoBytes, 2);
}

} // namespace opendnp3
