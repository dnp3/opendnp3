/*
 * Copyright 2013-2019 Automatak, LLC
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

#include "FreeFormatParser.h"

#include "app/GroupVariationRecord.h"
#include "app/parsing/ObjectHeaderParser.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

ParseResult FreeFormatParser::ParseHeader(ser4cpp::rseq_t& buffer,
                                          const ParserSettings& settings,
                                          const HeaderRecord& record,
                                          Logger* pLogger,
                                          IAPDUHandler* pHandler)
{
    if (buffer.length() < 3)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for free format count and size");
        return ParseResult::NOT_ENOUGH_DATA_FOR_HEADER;
    }

    uint8_t freeFormatCount;
    ser4cpp::UInt8::read_from(buffer, freeFormatCount);
    uint16_t freeFormatSize;
    ser4cpp::UInt16::read_from(buffer, freeFormatSize);

    FORMAT_LOGGER_BLOCK(pLogger, settings.LoggingLevel(), "%03u,%03u %s, %s, count: %u size: %u", record.group,
                        record.variation, GroupVariationSpec::to_human_string(record.enumeration),
                        QualifierCodeSpec::to_human_string(record.GetQualifierCode()), freeFormatCount, freeFormatSize);

    if (freeFormatCount != 1)
    {
        FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unsupported free-format count of %u", freeFormatCount);
        return ParseResult::UNREASONABLE_OBJECT_COUNT;
    }

    if (buffer.length() < freeFormatSize)
    {
        FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Insufficient data (%zu) for free format object of size (%u)",
                            buffer.length(), freeFormatSize);
        return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
    }

    ser4cpp::rseq_t copy(buffer.take(freeFormatSize));
    buffer.advance(freeFormatSize);

    FreeFormatHeader header(record, freeFormatCount);

    switch (record.enumeration)
    {
    case (GroupVariation::Group120Var1):
        return ParseFreeFormat(ParseAny<Group120Var1>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var2):
        return ParseFreeFormat(ParseAny<Group120Var2>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var5):
        return ParseFreeFormat(ParseAny<Group120Var5>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var6):
        return ParseFreeFormat(ParseAny<Group120Var6>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var7):
        return ParseFreeFormat(ParseAny<Group120Var7>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var9):
        return ParseFreeFormat(ParseAny<Group120Var9>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var8):
        return ParseFreeFormat(ParseAny<Group120Var8>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var10):
        return ParseFreeFormat(ParseAny<Group120Var10>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var11):
        return ParseFreeFormat(ParseAny<Group120Var11>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var12):
        return ParseFreeFormat(ParseAny<Group120Var12>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var13):
        return ParseFreeFormat(ParseAny<Group120Var13>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var14):
        return ParseFreeFormat(ParseAny<Group120Var14>, header, freeFormatSize, copy, pHandler, pLogger);

    case (GroupVariation::Group120Var15):
        return ParseFreeFormat(ParseAny<Group120Var15>, header, freeFormatSize, copy, pHandler, pLogger);

    default:
        FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unsupported qualifier/object - %s - %i / %i",
                            QualifierCodeSpec::to_human_string(record.GetQualifierCode()), record.group,
                            record.variation);

        return ParseResult::INVALID_OBJECT_QUALIFIER;
    }
}

ParseResult FreeFormatParser::ParseFreeFormat(FreeFormatHandler parser,
                                              const FreeFormatHeader& header,
                                              uint16_t /*size*/,
                                              ser4cpp::rseq_t& objects,
                                              IAPDUHandler* pHandler,
                                              Logger* pLogger)
{
    if (parser(header, objects, pHandler))
    {
        return ParseResult::OK;
    }

    FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Insufficient data for free-format object: (%i, %i)", header.group,
                        header.variation);
    return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
}

} // namespace opendnp3
