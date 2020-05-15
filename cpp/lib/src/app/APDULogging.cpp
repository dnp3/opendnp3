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

#include "APDULogging.h"

#include "app/parsing/APDUHeaderParser.h"
#include "app/parsing/APDUParser.h"
#include "logging/LogMacros.h"

namespace opendnp3
{
namespace logging
{

    void LogHeader(Logger& logger, const LogLevel& flags, const APDUHeader& header)
    {
        FORMAT_LOG_BLOCK(logger, flags, "FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s", header.control.FIR,
                         header.control.FIN, header.control.CON, header.control.UNS, header.control.SEQ,
                         FunctionCodeSpec::to_human_string(header.function));
    }

    void LogHeader(Logger& logger, const LogLevel& flags, const APDUResponseHeader& header)
    {
        FORMAT_LOG_BLOCK(logger, flags, "FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
                         header.control.FIR, header.control.FIN, header.control.CON, header.control.UNS,
                         header.control.SEQ, FunctionCodeSpec::to_human_string(header.function), header.IIN.LSB,
                         header.IIN.MSB);
    }

    void ParseAndLogRequestTx(Logger& logger, const ser4cpp::rseq_t& apdu)
    {
        FORMAT_HEX_BLOCK(logger, flags::APP_HEX_TX, apdu, 18, 18);

        if (logger.is_enabled(flags::APP_HEADER_TX))
        {
            const auto result = APDUHeaderParser::ParseRequest(apdu, &logger);
            if (result.success)
            {
                LogHeader(logger, flags::APP_HEADER_TX, result.header);

                if (logger.is_enabled(flags::APP_OBJECT_TX))
                {
                    auto expectsContents = result.header.function != FunctionCode::READ;
                    APDUParser::ParseAndLogAll(result.objects, &logger,
                                               ParserSettings::Create(expectsContents, flags::APP_OBJECT_TX));
                }
            }
        }
    }

    void ParseAndLogResponseTx(Logger& logger, const ser4cpp::rseq_t& apdu)
    {
        FORMAT_HEX_BLOCK(logger, flags::APP_HEX_TX, apdu, 18, 18);

        if (logger.is_enabled(flags::APP_HEADER_TX))
        {
            const auto result = APDUHeaderParser::ParseResponse(apdu, &logger);
            if (result.success)
            {
                LogHeader(logger, flags::APP_HEADER_TX, result.header);

                if (logger.is_enabled(flags::APP_OBJECT_TX))
                {
                    APDUParser::ParseAndLogAll(result.objects, &logger,
                                               ParserSettings::Create(true, flags::APP_OBJECT_TX));
                }
            }
        }
    }

} // namespace logging
} // namespace opendnp3
