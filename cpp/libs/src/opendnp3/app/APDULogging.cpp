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

#include "APDULogging.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/app/parsing/APDUParser.h"

namespace opendnp3
{
namespace logging
{

    void LogHeader(openpal::Logger& logger, int32_t flags, const APDUHeader& header)
    {
#ifndef OPENPAL_STRIP_LOGGING

        FORMAT_LOG_BLOCK(logger, flags, "FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s", header.control.FIR,
                         header.control.FIN, header.control.CON, header.control.UNS, header.control.SEQ,
                         FunctionCodeToString(header.function));

#endif
    }

    void LogHeader(openpal::Logger& logger, int32_t flags, const APDUResponseHeader& header)
    {
#ifndef OPENPAL_STRIP_LOGGING
        FORMAT_LOG_BLOCK(logger, flags, "FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
                         header.control.FIR, header.control.FIN, header.control.CON, header.control.UNS,
                         header.control.SEQ, FunctionCodeToString(header.function), header.IIN.LSB, header.IIN.MSB);
#endif
    }

    void ParseAndLogRequestTx(openpal::Logger& logger, const openpal::RSlice& apdu)
    {

#ifndef OPENPAL_STRIP_LOGGING

        FORMAT_HEX_BLOCK(logger, flags::APP_HEX_TX, apdu, 18, 18);

        if (logger.IsEnabled(flags::APP_HEADER_TX))
        {
            const auto result = APDUHeaderParser::ParseRequest(apdu, &logger);
            if (result.success)
            {
                LogHeader(logger, flags::APP_HEADER_TX, result.header);

                if (logger.IsEnabled(flags::APP_OBJECT_TX))
                {
                    auto expectsContents = result.header.function != FunctionCode::READ;
                    APDUParser::ParseAndLogAll(result.objects, &logger,
                                               ParserSettings::Create(expectsContents, flags::APP_OBJECT_TX));
                }
            }
        }

#endif
    }

    void ParseAndLogResponseTx(openpal::Logger& logger, const openpal::RSlice& apdu)
    {

#ifndef OPENPAL_STRIP_LOGGING

        FORMAT_HEX_BLOCK(logger, flags::APP_HEX_TX, apdu, 18, 18);

        if (logger.IsEnabled(flags::APP_HEADER_TX))
        {
            const auto result = APDUHeaderParser::ParseResponse(apdu, &logger);
            if (result.success)
            {
                LogHeader(logger, flags::APP_HEADER_TX, result.header);

                if (logger.IsEnabled(flags::APP_OBJECT_TX))
                {
                    APDUParser::ParseAndLogAll(result.objects, &logger,
                                               ParserSettings::Create(true, flags::APP_OBJECT_TX));
                }
            }
        }

#endif
    }

} // namespace logging
} // namespace opendnp3
