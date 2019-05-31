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
#ifndef OPENDNP3_FREEFORMATPARSER_H
#define OPENDNP3_FREEFORMATPARSER_H

#include "app/parsing/IAPDUHandler.h"
#include "app/parsing/ObjectHeaderParser.h"
#include "app/parsing/ParseResult.h"
#include "app/parsing/ParserSettings.h"

#include "opendnp3/StaticOnly.h"

#include <ser4cpp/container/SequenceTypes.h>

#include <log4cpp/Logger.h>

namespace opendnp3
{

/**
    Parser for free format (0x5B)
*/
class FreeFormatParser : private StaticOnly
{
public:
    static ParseResult ParseHeader(ser4cpp::rseq_t& buffer,
                                   const ParserSettings& settings,
                                   const HeaderRecord& record,
                                   log4cpp::Logger* pLogger,
                                   IAPDUHandler* pHandler);

private:
    typedef bool (&FreeFormatHandler)(const FreeFormatHeader& header,
                                      const ser4cpp::rseq_t& objects,
                                      IAPDUHandler* pHandler);

    static ParseResult ParseFreeFormat(FreeFormatHandler parser,
                                       const FreeFormatHeader& header,
                                       uint16_t size,
                                       ser4cpp::rseq_t& objects,
                                       IAPDUHandler* pHandler,
                                       log4cpp::Logger* pLogger);

    // Free format handlers

    template<class T>
    static bool ParseAny(const FreeFormatHeader& header, const ser4cpp::rseq_t& object, IAPDUHandler* pHandler)
    {
        T value;
        auto success = value.Read(object);
        if (success && pHandler)
        {
            pHandler->OnHeader(header, value, object);
        }
        return success;
    }
};

} // namespace opendnp3

#endif
