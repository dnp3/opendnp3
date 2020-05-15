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
#include "APDUHeaderParser.h"

#include "app/APDUHeader.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"
#include "opendnp3/util/Uncopyable.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

APDUHeaderParser::Result<APDUHeader> APDUHeaderParser::ParseRequest(const ser4cpp::rseq_t& apdu, Logger* logger)
{
    if (apdu.length() < APDUHeader::REQUEST_SIZE)
    {
        FORMAT_LOGGER_BLOCK(logger, flags::WARN, "Request fragment  with insufficient size of %zu bytes",
                            apdu.length());
        return Result<APDUHeader>::Error();
    }

    return Result<APDUHeader>::Ok(APDUHeader(AppControlField(apdu[0]), FunctionCodeSpec::from_type(apdu[1])),
                                  apdu.skip(APDUHeader::REQUEST_SIZE));
}

APDUHeaderParser::Result<APDUResponseHeader> APDUHeaderParser::ParseResponse(const ser4cpp::rseq_t& apdu,
                                                                             Logger* logger)
{
    if (apdu.length() < APDUHeader::RESPONSE_SIZE)
    {
        FORMAT_LOGGER_BLOCK(logger, flags::WARN, "Response fragment  with insufficient size of %zu bytes",
                            apdu.length());
        return Result<APDUResponseHeader>::Error();
    }

    return Result<APDUResponseHeader>::Ok(
        APDUResponseHeader(AppControlField(apdu[0]), FunctionCodeSpec::from_type(apdu[1]), IINField(apdu[2], apdu[3])),
        apdu.skip(APDUHeader::RESPONSE_SIZE));
}

} // namespace opendnp3
