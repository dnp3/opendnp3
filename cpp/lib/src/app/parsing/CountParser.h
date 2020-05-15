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
#ifndef OPENDNP3_COUNTPARSER_H
#define OPENDNP3_COUNTPARSER_H

#include "app/parsing/BufferedCollection.h"
#include "app/parsing/Functions.h"
#include "app/parsing/IAPDUHandler.h"
#include "app/parsing/NumParser.h"
#include "app/parsing/ParseResult.h"
#include "app/parsing/ParserSettings.h"

#include "opendnp3/logging/Logger.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

class CountParser
{
    typedef void (*HandleFun)(const HeaderRecord& record,
                              uint16_t count,
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
    template<class Descriptor> static CountParser From(uint16_t count);

    static ParseResult ParseCountOfObjects(
        ser4cpp::rseq_t& buffer, const HeaderRecord& record, uint16_t count, Logger* pLogger, IAPDUHandler* pHandler);

    template<class Descriptor>
    static void InvokeCountOf(const HeaderRecord& record,
                              uint16_t count,
                              const ser4cpp::rseq_t& buffer,
                              IAPDUHandler& handler);

    CountParser(uint16_t count, size_t required_size, HandleFun handler);

    uint16_t count;
    size_t required_size;
    HandleFun handler;

    CountParser() = delete;
};

template<class Descriptor> CountParser CountParser::From(uint16_t count)
{
    const auto size = static_cast<size_t>(count) * Descriptor::Size();
    return CountParser(count, size, &InvokeCountOf<Descriptor>);
}

template<class T>
void CountParser::InvokeCountOf(const HeaderRecord& record,
                                uint16_t count,
                                const ser4cpp::rseq_t& buffer,
                                IAPDUHandler& handler)
{
    auto read = [](ser4cpp::rseq_t& buffer, uint32_t) -> T {
        T value;
        T::Read(buffer, value);
        return value;
    };

    auto collection = CreateBufferedCollection<T>(buffer, count, read);
    handler.OnHeader(CountHeader(record, count), collection);
}

} // namespace opendnp3

#endif
