/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#include "CountIndexParser.h"

#include "app/parsing/BufferedCollection.h"
#include "gen/objects/Group11.h"
#include "gen/objects/Group12.h"
#include "gen/objects/Group13.h"
#include "gen/objects/Group2.h"
#include "gen/objects/Group22.h"
#include "gen/objects/Group23.h"
#include "gen/objects/Group32.h"
#include "gen/objects/Group4.h"
#include "gen/objects/Group41.h"
#include "gen/objects/Group42.h"
#include "gen/objects/Group43.h"
#include "logging/LogMacros.h"

namespace opendnp3
{

CountIndexParser::CountIndexParser(uint16_t count, size_t requiredSize, const NumParser& numparser, HandleFun handler)
    : count(count), requiredSize(requiredSize), numparser(numparser), handler(handler)
{
}

ParseResult CountIndexParser::ParseHeader(ser4cpp::rseq_t& buffer,
                                          const NumParser& numparser,
                                          const ParserSettings& settings,
                                          const HeaderRecord& record,
                                          Logger* pLogger,
                                          IAPDUHandler* pHandler)
{
    uint16_t count;
    auto res = numparser.ParseCount(buffer, count, pLogger);
    if (res == ParseResult::OK)
    {
        FORMAT_LOGGER_BLOCK(pLogger, settings.LoggingLevel(), "%03u,%03u %s, %s [%u]", record.group, record.variation,
                            GroupVariationSpec::to_human_string(record.enumeration),
                            QualifierCodeSpec::to_human_string(record.GetQualifierCode()), count);

        if (settings.ExpectsContents())
        {
            return ParseCountOfObjects(buffer, record, numparser, count, pLogger, pHandler);
        }
        else
        {
            return ParseCountOfIndices(buffer, record, numparser, count, pLogger, pHandler);
        }
    }

    return res;
}

ParseResult CountIndexParser::Process(const HeaderRecord& record,
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
        handler(record, count, numparser, buffer, *pHandler);
    }
    buffer.advance(requiredSize);
    return ParseResult::OK;
}

ParseResult CountIndexParser::ParseCountOfObjects(ser4cpp::rseq_t& buffer,
                                                  const HeaderRecord& record,
                                                  const NumParser& numparser,
                                                  uint16_t count,
                                                  Logger* pLogger,
                                                  IAPDUHandler* pHandler)
{
    switch (record.enumeration)
    {
    case (GroupVariation::Group2Var1):
        return CountIndexParser::From<Group2Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group2Var2):
        return CountIndexParser::From<Group2Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group2Var3):
        return CountIndexParser::From<Group2Var3>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group4Var1):
        return CountIndexParser::From<Group4Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group4Var2):
        return CountIndexParser::From<Group4Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group4Var3):
        return CountIndexParser::From<Group4Var3>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group11Var1):
        return CountIndexParser::From<Group11Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group11Var2):
        return CountIndexParser::From<Group11Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group12Var1):
        return CountIndexParser::From<Group12Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group13Var1):
        return CountIndexParser::From<Group13Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group13Var2):
        return CountIndexParser::From<Group13Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group22Var1):
        return CountIndexParser::From<Group22Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group22Var2):
        return CountIndexParser::From<Group22Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group22Var5):
        return CountIndexParser::From<Group22Var5>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group22Var6):
        return CountIndexParser::From<Group22Var6>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group23Var1):
        return CountIndexParser::From<Group23Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group23Var2):
        return CountIndexParser::From<Group23Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group23Var5):
        return CountIndexParser::From<Group23Var5>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group23Var6):
        return CountIndexParser::From<Group23Var6>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group32Var1):
        return CountIndexParser::From<Group32Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var2):
        return CountIndexParser::From<Group32Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var3):
        return CountIndexParser::From<Group32Var3>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var4):
        return CountIndexParser::From<Group32Var4>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var5):
        return CountIndexParser::From<Group32Var5>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var6):
        return CountIndexParser::From<Group32Var6>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var7):
        return CountIndexParser::From<Group32Var7>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group32Var8):
        return CountIndexParser::From<Group32Var8>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group41Var1):
        return CountIndexParser::From<Group41Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group41Var2):
        return CountIndexParser::From<Group41Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group41Var3):
        return CountIndexParser::From<Group41Var3>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group41Var4):
        return CountIndexParser::From<Group41Var4>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group42Var1):
        return CountIndexParser::From<Group42Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var2):
        return CountIndexParser::From<Group42Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var3):
        return CountIndexParser::From<Group42Var3>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var4):
        return CountIndexParser::From<Group42Var4>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var5):
        return CountIndexParser::From<Group42Var5>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var6):
        return CountIndexParser::From<Group42Var6>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var7):
        return CountIndexParser::From<Group42Var7>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group42Var8):
        return CountIndexParser::From<Group42Var8>(count, numparser).Process(record, buffer, pHandler, pLogger);

    case (GroupVariation::Group43Var1):
        return CountIndexParser::From<Group43Var1>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var2):
        return CountIndexParser::From<Group43Var2>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var3):
        return CountIndexParser::From<Group43Var3>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var4):
        return CountIndexParser::From<Group43Var4>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var5):
        return CountIndexParser::From<Group43Var5>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var6):
        return CountIndexParser::From<Group43Var6>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var7):
        return CountIndexParser::From<Group43Var7>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group43Var8):
        return CountIndexParser::From<Group43Var8>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group50Var4):
        return CountIndexParser::From<Group50Var4>(count, numparser).Process(record, buffer, pHandler, pLogger);
    case (GroupVariation::Group111Var0):
        return ParseIndexPrefixedOctetData(buffer, record, numparser, count, pLogger, pHandler);

    default:

        FORMAT_LOGGER_BLOCK(pLogger, flags::WARN, "Unsupported qualifier/object - %s - %i / %i",
                            QualifierCodeSpec::to_human_string(record.GetQualifierCode()), record.group,
                            record.variation);

        return ParseResult::INVALID_OBJECT_QUALIFIER;
    }
}

ParseResult CountIndexParser::ParseCountOfIndices(ser4cpp::rseq_t& buffer,
                                                  const HeaderRecord& record,
                                                  const NumParser& numparser,
                                                  uint16_t count,
                                                  Logger* pLogger,
                                                  IAPDUHandler* pHandler)
{
    const auto SIZE = static_cast<size_t>(count) * static_cast<size_t>(numparser.NumBytes());

    if (buffer.length() < SIZE)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for specified sequence of indices");
        return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
    }

    if (pHandler)
    {
        auto read = [&numparser, record](ser4cpp::rseq_t& buffer, uint32_t pos) -> uint16_t {
            return numparser.ReadNum(buffer);
        };

        auto collection = CreateBufferedCollection<uint16_t>(buffer, count, read);
        pHandler->OnHeader(PrefixHeader(record, count), collection);
    }

    buffer.advance(SIZE);
    return ParseResult::OK;
}

ParseResult CountIndexParser::ParseIndexPrefixedOctetData(ser4cpp::rseq_t& buffer,
                                                          const HeaderRecord& record,
                                                          const NumParser& numparser,
                                                          uint32_t count,
                                                          Logger* pLogger,
                                                          IAPDUHandler* pHandler)
{
    if (record.variation == 0)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Octet string variation 0 may only be used in requests");
        return ParseResult::INVALID_OBJECT;
    }

    const uint32_t TOTAL_SIZE = count * (numparser.NumBytes() + record.variation);

    if (buffer.length() < TOTAL_SIZE)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for specified bitfield objects");
        return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
    }

    if (pHandler)
    {
        auto read = [&numparser, record](ser4cpp::rseq_t& buffer, uint32_t pos) -> Indexed<OctetString> {
            auto index = numparser.ReadNum(buffer);
            const auto octetStringSlice = buffer.take(record.variation);
            OctetString octets(Buffer(octetStringSlice, octetStringSlice.length()));
            buffer.advance(record.variation);
            return WithIndex(octets, index);
        };

        auto collection = CreateBufferedCollection<Indexed<OctetString>>(buffer, count, read);
        pHandler->OnHeader(PrefixHeader(record, count), collection);
    }

    buffer.advance(TOTAL_SIZE);
    return ParseResult::OK;
}

} // namespace opendnp3