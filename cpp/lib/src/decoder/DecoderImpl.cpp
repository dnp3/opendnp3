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

#include "decoder/DecoderImpl.h"

#include "app/APDULogging.h"
#include "app/parsing/APDUHeaderParser.h"
#include "app/parsing/APDUParser.h"
#include "decoder/Indent.h"
#include "decoder/LoggingHandler.h"
#include "logging/LogMacros.h"

namespace opendnp3
{

DecoderImpl::DecoderImpl(IDecoderCallbacks& callbacks, const Logger& logger)
    : callbacks(&callbacks), logger(logger), link(logger), transportRx(logger, 2048)
{
}

void DecoderImpl::DecodeLPDU(const ser4cpp::rseq_t& data)
{
    Indent i(*callbacks);

    ser4cpp::rseq_t remaining(data);

    while (remaining.is_not_empty())
    {
        auto dest = this->link.WriteBuff();

        const auto NUM = (remaining.length() > dest.length()) ? dest.length() : remaining.length();

        dest.copy_from(remaining.take(NUM));
        link.OnRead(NUM, *this);

        remaining.advance(NUM);
    }
}

void DecoderImpl::DecodeTPDU(const ser4cpp::rseq_t& data)
{
    Indent i(*callbacks);
    FORMAT_HEX_BLOCK(logger, flags::TRANSPORT_RX, data, 18, 18);

    auto result = transportRx.ProcessReceive(Message(Addresses(), data));
    if (result.payload.is_not_empty())
    {
        this->DecodeAPDU(result.payload);
    }
}

void DecoderImpl::DecodeAPDU(const ser4cpp::rseq_t& data)
{
    Indent i(*callbacks);

    FORMAT_HEX_BLOCK(this->logger, flags::APP_HEX_RX, data, 18, 18);

    if (IsResponse(data))
    {
        const auto result = APDUHeaderParser::ParseResponse(data, &logger);
        if (result.success)
        {
            logging::LogHeader(this->logger, flags::APP_HEADER_RX, result.header);

            if ((result.header.IIN.LSB & 0x01) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.0 - Broadcast");
            if ((result.header.IIN.LSB & 0x02) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.1 - Class 1 events");
            if ((result.header.IIN.LSB & 0x04) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.2 - Class 2 events");
            if ((result.header.IIN.LSB & 0x08) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.3 - Class 3 events");
            if ((result.header.IIN.LSB & 0x10) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.4 - Need time");
            if ((result.header.IIN.LSB & 0x20) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.5 - Local control");
            if ((result.header.IIN.LSB & 0x40) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.6 - Device trouble");
            if ((result.header.IIN.LSB & 0x80) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN1.7 - Device restart");
            if ((result.header.IIN.MSB & 0x01) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.0 - Function code not supported");
            if ((result.header.IIN.MSB & 0x02) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.1 - Object unknown");
            if ((result.header.IIN.MSB & 0x04) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.2 - Parameter error");
            if ((result.header.IIN.MSB & 0x08) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.3 - Event buffer overflow");
            if ((result.header.IIN.MSB & 0x10) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.4 - Already executing");
            if ((result.header.IIN.MSB & 0x20) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.5 - Configuration corrupt");
            if ((result.header.IIN.MSB & 0x40) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.6 - Reserved 1");
            if ((result.header.IIN.MSB & 0x80) != 0)
                SIMPLE_LOG_BLOCK(this->logger, flags::APP_HEADER_RX, "IIN2.7 - Reserved 2");

            Indent i(*callbacks);
            LoggingHandler handler(logger, *callbacks);
            APDUParser::ParseSinglePass(result.objects, &logger, &handler, nullptr, ParserSettings::Default());
        }
    }
    else
    {
        const auto result = APDUHeaderParser::ParseRequest(data, &logger);
        if (result.success)
        {
            logging::LogHeader(this->logger, flags::APP_HEADER_RX, result.header);

            Indent i(*callbacks);
            LoggingHandler handler(logger, *callbacks);
            auto settings = (result.header.function == FunctionCode::READ) ? ParserSettings::NoContents()
                                                                           : ParserSettings::Default();
            APDUParser::ParseSinglePass(result.objects, &logger, &handler, nullptr, settings);
        }
    }
}

bool DecoderImpl::IsResponse(const ser4cpp::rseq_t& data)
{
    if (data.length() < 2)
    {
        return false;
    }

    switch (FunctionCodeSpec::from_type(data[1]))
    {
    case (FunctionCode::RESPONSE):
    case (FunctionCode::UNSOLICITED_RESPONSE):
    case (FunctionCode::AUTH_RESPONSE):
        return true;
    default:
        return false;
    }
}

bool DecoderImpl::OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata)
{
    if (header.func == LinkFunction::PRI_CONFIRMED_USER_DATA || header.func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
    {
        this->DecodeTPDU(userdata);
    }

    return true;
}

} // namespace opendnp3
