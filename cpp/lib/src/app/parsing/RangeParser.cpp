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
#include "RangeParser.h"

#include "app/parsing/BufferedCollection.h"
#include "gen/objects/Group1.h"
#include "gen/objects/Group10.h"
#include "gen/objects/Group20.h"
#include "gen/objects/Group21.h"
#include "gen/objects/Group3.h"
#include "gen/objects/Group30.h"
#include "gen/objects/Group40.h"
#include "gen/objects/Group50.h"
#include "logging/LogMacros.h"

namespace opendnp3
{

RangeParser::RangeParser(const Range& range, size_t requiredSize, HandleFun handler)
    : range(range), requiredSize(requiredSize), handler(handler)
{
}

ParseResult RangeParser::ParseHeader(ser4cpp::rseq_t& buffer,
                                     const NumParser& numparser,
                                     const ParserSettings& settings,
                                     const HeaderRecord& record,
                                     Logger* pLogger,
                                     IAPDUHandler* pHandler)
{
    Range range;
    auto res = numparser.ParseRange(buffer, range, pLogger);
    if (res != ParseResult::OK)
    {
        return res;
    }

    FORMAT_LOGGER_BLOCK(pLogger, settings.LoggingLevel(), "%03u,%03u %s, %s [%u, %u]", record.group, record.variation,
                        GroupVariationSpec::to_human_string(record.enumeration),
                        QualifierCodeSpec::to_human_string(record.GetQualifierCode()), range.start, range.stop);

    if (settings.ExpectsContents())
    {
        return ParseRangeOfObjects(buffer, record, range, pLogger, pHandler);
    }

    if (pHandler)
    {
        pHandler->OnHeader(RangeHeader(record, range));
    }
    return ParseResult::OK;
}

ParseResult RangeParser::Process(const HeaderRecord& record,
                                 ser4cpp::rseq_t& buffer,
                                 IAPDUHandler* pHandler,
                                 Logger* pLogger) const
{
    if (buffer.length() < requiredSize)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for specified objects");
        return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
    }

    if (pHandler)
    {
        handler(record, range, buffer, *pHandler);
    }
    buffer.advance(requiredSize);
    return ParseResult::OK;
}

#define MACRO_PARSE_OBJECTS_WITH_RANGE(descriptor)                                                                     \
    case (GroupVariation::descriptor):                                                                                 \
        return RangeParser::FromFixedSize<descriptor>(range).Process(record, buffer, pHandler, pLogger);

ParseResult RangeParser::ParseRangeOfObjects(
    ser4cpp::rseq_t& buffer, const HeaderRecord& record, const Range& range, Logger* pLogger, IAPDUHandler* pHandler)
{
    switch (record.enumeration)
    {
    case (GroupVariation::Group1Var1):
        return RangeParser::FromBitfieldType<Binary>(range).Process(record, buffer, pHandler, pLogger);

        MACRO_PARSE_OBJECTS_WITH_RANGE(Group1Var2);

    case (GroupVariation::Group3Var1):
        return RangeParser::FromDoubleBitfieldType<DoubleBitBinary>(range).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group10Var1):
        return RangeParser::FromBitfieldType<BinaryOutputStatus>(range).Process(record, buffer, pHandler, pLogger);

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

    case (GroupVariation::Group80Var1):
        return RangeParser::FromBitfieldType<IINValue>(range).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group110Var0):
        return ParseRangeOfOctetData(buffer, record, range, pLogger, pHandler);

    default:
        FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unsupported qualifier/object - %s - %i / %i",
                            QualifierCodeSpec::to_human_string(record.GetQualifierCode()), record.group,
                            record.variation);

        return ParseResult::INVALID_OBJECT_QUALIFIER;
    }
}

ParseResult RangeParser::ParseRangeOfOctetData(
    ser4cpp::rseq_t& buffer, const HeaderRecord& record, const Range& range, Logger* pLogger, IAPDUHandler* pHandler)
{
    if (record.variation > 0)
    {
        const auto COUNT = range.Count();
        auto size = record.variation * COUNT;
        if (buffer.length() < size)
        {
            SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for specified octet objects");
            return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
        }

        if (pHandler)
        {
            auto read = [range, record](ser4cpp::rseq_t& buffer, uint32_t pos) -> Indexed<OctetString> {
                const auto octetData = buffer.take(record.variation);
                OctetString octets(Buffer(octetData, octetData.length()));
                buffer.advance(record.variation);
                return WithIndex(octets, range.start + pos);
            };

            auto collection = CreateBufferedCollection<Indexed<OctetString>>(buffer, COUNT, read);

            pHandler->OnHeader(RangeHeader(record, range), collection);
        }

        buffer.advance(size);
        return ParseResult::OK;
    }
    else
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Octet string variation 0 may only be used in requests");
        return ParseResult::INVALID_OBJECT;
    }
}

} // namespace opendnp3
