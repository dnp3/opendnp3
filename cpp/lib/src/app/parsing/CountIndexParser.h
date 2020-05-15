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
#ifndef OPENDNP3_COUNTINDEXPARSER_H
#define OPENDNP3_COUNTINDEXPARSER_H

#include "app/parsing/BufferedCollection.h"
#include "app/parsing/IAPDUHandler.h"
#include "app/parsing/NumParser.h"
#include "app/parsing/ParseResult.h"
#include "app/parsing/ParserSettings.h"

#include "opendnp3/logging/Logger.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

class CountIndexParser
{
    typedef void (*HandleFun)(const HeaderRecord& record,
                              uint16_t count,
                              const NumParser& numparser,
                              const ser4cpp::rseq_t& buffer,
                              IAPDUHandler& handler);

public:
    static ParseResult ParseHeader(ser4cpp::rseq_t& buffer,
                                   const NumParser& numparser,
                                   const ParserSettings& settings,
                                   const HeaderRecord& record,
                                   Logger* pLogger,
                                   IAPDUHandler* pHandler);

private:
    // Process the count handler against the buffer
    ParseResult Process(const HeaderRecord& record,
                        ser4cpp::rseq_t& buffer,
                        IAPDUHandler* pHandler,
                        Logger* pLogger) const;

    // Create a count handler from a fixed size descriptor
    template<class Descriptor> static CountIndexParser From(uint16_t count, const NumParser& numparser);

    // Create a count handler from a fixed size descriptor
    template<class Type> static CountIndexParser FromType(uint16_t count, const NumParser& numparser);

    static ParseResult ParseCountOfObjects(ser4cpp::rseq_t& buffer,
                                           const HeaderRecord& record,
                                           const NumParser& numparser,
                                           uint16_t count,
                                           Logger* pLogger,
                                           IAPDUHandler* pHandler);

    static ParseResult ParseCountOfIndices(ser4cpp::rseq_t& buffer,
                                           const HeaderRecord& record,
                                           const NumParser& numparser,
                                           uint16_t count,
                                           Logger* pLogger,
                                           IAPDUHandler* pHandler);

    static ParseResult ParseIndexPrefixedOctetData(ser4cpp::rseq_t& buffer,
                                                   const HeaderRecord& record,
                                                   const NumParser& numParser,
                                                   uint32_t count,
                                                   Logger* pLogger,
                                                   IAPDUHandler* pHandler);

    template<class Descriptor>
    static void InvokeCountOf(const HeaderRecord& record,
                              uint16_t count,
                              const NumParser& numparser,
                              const ser4cpp::rseq_t& buffer,
                              IAPDUHandler& handler);

    template<class Type>
    static void InvokeCountOfType(const HeaderRecord& record,
                                  uint16_t count,
                                  const NumParser& numparser,
                                  const ser4cpp::rseq_t& buffer,
                                  IAPDUHandler& handler);

    CountIndexParser(uint16_t count, size_t requiredSize, const NumParser& numparser, HandleFun handler);

    uint16_t count;
    size_t requiredSize;
    NumParser numparser;
    HandleFun handler;

    CountIndexParser() = delete;
};

template<class Descriptor> CountIndexParser CountIndexParser::From(uint16_t count, const NumParser& numparser)
{
    const size_t SIZE = static_cast<size_t>(count) * (Descriptor::Size() + numparser.NumBytes());
    return CountIndexParser(count, SIZE, numparser, &InvokeCountOf<Descriptor>);
}

template<class Type> CountIndexParser CountIndexParser::FromType(uint16_t count, const NumParser& numparser)
{
    const size_t SIZE = static_cast<size_t>(count) * (Type::Size() + numparser.NumBytes());
    return CountIndexParser(count, SIZE, numparser, &InvokeCountOfType<Type>);
}

template<class Descriptor>
void CountIndexParser::InvokeCountOf(const HeaderRecord& record,
                                     uint16_t count,
                                     const NumParser& numparser,
                                     const ser4cpp::rseq_t& buffer,
                                     IAPDUHandler& handler)
{
    auto read = [&numparser](ser4cpp::rseq_t & buffer, uint32_t) -> Indexed<typename Descriptor::Target>
    {
        Indexed<typename Descriptor::Target> pair;
        pair.index = numparser.ReadNum(buffer);
        Descriptor::ReadTarget(buffer, pair.value);
        return pair;
    };

    auto collection = CreateBufferedCollection<Indexed<typename Descriptor::Target>>(buffer, count, read);
    handler.OnHeader(PrefixHeader(record, count), collection);
}

template<class Type>
void CountIndexParser::InvokeCountOfType(const HeaderRecord& record,
                                         uint16_t count,
                                         const NumParser& numparser,
                                         const ser4cpp::rseq_t& buffer,
                                         IAPDUHandler& handler)
{
    auto read = [&numparser](ser4cpp::rseq_t& buffer, uint32_t) -> Indexed<Type> {
        Indexed<Type> pair;
        pair.index = numparser.ReadNum(buffer);
        Type::Read(buffer, pair.value);
        return pair;
    };

    auto collection = CreateBufferedCollection<Indexed<Type>>(buffer, count, read);
    handler.OnHeader(PrefixHeader(record, count), collection);
}

} // namespace opendnp3

#endif
