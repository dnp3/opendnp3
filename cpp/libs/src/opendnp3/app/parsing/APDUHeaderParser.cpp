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
#include "APDUHeaderParser.h"

#include <openpal/container/RSlice.h>
#include <openpal/logging/LogMacros.h>
#include <openpal/util/Uncopyable.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/APDUHeader.h"

namespace opendnp3
{

APDUHeaderParser::Result<APDUHeader> APDUHeaderParser::ParseRequest(const openpal::RSlice& apdu,
                                                                    openpal::Logger* logger)
{
    if (apdu.Size() < APDUHeader::REQUEST_SIZE)
    {
        FORMAT_LOGGER_BLOCK(logger, flags::WARN, "Request fragment  with insufficient size of %u bytes", apdu.Size());
        return Result<APDUHeader>::Error();
    }

    return Result<APDUHeader>::Ok(APDUHeader(AppControlField(apdu[0]), FunctionCodeFromType(apdu[1])),
                                  apdu.Skip(APDUHeader::REQUEST_SIZE));
}

APDUHeaderParser::Result<APDUResponseHeader> APDUHeaderParser::ParseResponse(const openpal::RSlice& apdu,
                                                                             openpal::Logger* logger)
{
    if (apdu.Size() < APDUHeader::RESPONSE_SIZE)
    {
        FORMAT_LOGGER_BLOCK(logger, flags::WARN, "Response fragment  with insufficient size of %u bytes", apdu.Size());
        return Result<APDUResponseHeader>::Error();
    }

    return Result<APDUResponseHeader>::Ok(
        APDUResponseHeader(AppControlField(apdu[0]), FunctionCodeFromType(apdu[1]), IINField(apdu[2], apdu[3])),
        apdu.Skip(APDUHeader::RESPONSE_SIZE));
}

} // namespace opendnp3
