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
#include "LinkFrame.h"

#include "link/CRC.h"
#include "link/LinkHeader.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

void LinkFrame::ReadUserData(const uint8_t* pSrc, uint8_t* pDest, size_t len)
{
    size_t length = len;
    uint8_t const* pRead = pSrc;
    uint8_t* pWrite = pDest;

    while (length > 0)
    {
        size_t max = LPDU_DATA_BLOCK_SIZE;
        size_t num = (length <= max) ? length : max;
        size_t num_with_crc = num + 2;
        memmove(pWrite, pRead, num);
        pRead += num_with_crc;
        pWrite += num;
        length -= num;
    }
}

bool LinkFrame::ValidateBodyCRC(const uint8_t* pBody, size_t length)
{
    while (length > 0)
    {
        size_t max = LPDU_DATA_BLOCK_SIZE;
        size_t num = (length <= max) ? length : max;

        if (CRC::IsCorrectCRC(pBody, num))
        {
            pBody += (num + 2);
            length -= num;
        }
        else
        {
            return false;
        }
    }
    return true;
}

size_t LinkFrame::CalcFrameSize(size_t dataLength)
{
    return LPDU_HEADER_SIZE + CalcUserDataSize(dataLength);
}

size_t LinkFrame::CalcUserDataSize(size_t dataLength)
{
    if (dataLength > 0)
    {
        size_t mod16 = dataLength % LPDU_DATA_BLOCK_SIZE;
        size_t size = (dataLength / LPDU_DATA_BLOCK_SIZE) * LPDU_DATA_PLUS_CRC_SIZE; // complete blocks
        return (mod16 > 0) ? (size + mod16 + LPDU_CRC_SIZE) : size;                  // possible partial block
    }

    return 0;
}

////////////////////////////////////////////////
//
//	Outgoing frame formatting functions for Sec to Pri transactions
//
////////////////////////////////////////////////

ser4cpp::rseq_t LinkFrame::FormatAck(
    ser4cpp::wseq_t& buffer, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, false, aIsRcvBuffFull, LinkFunction::SEC_ACK, aDest, aSrc, pLogger);
}

ser4cpp::rseq_t LinkFrame::FormatNack(
    ser4cpp::wseq_t& buffer, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, false, aIsRcvBuffFull, LinkFunction::SEC_NACK, aDest, aSrc, pLogger);
}

ser4cpp::rseq_t LinkFrame::FormatLinkStatus(
    ser4cpp::wseq_t& buffer, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, false, aIsRcvBuffFull, LinkFunction::SEC_LINK_STATUS, aDest, aSrc,
                        pLogger);
}

ser4cpp::rseq_t LinkFrame::FormatNotSupported(
    ser4cpp::wseq_t& buffer, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, false, aIsRcvBuffFull, LinkFunction::SEC_NOT_SUPPORTED, aDest, aSrc,
                        pLogger);
}

////////////////////////////////////////////////
//
//	Outgoing frame formatting functions for Pri to Sec transactions
//
////////////////////////////////////////////////

ser4cpp::rseq_t LinkFrame::FormatResetLinkStates(
    ser4cpp::wseq_t& buffer, bool aIsMaster, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, false, false, LinkFunction::PRI_RESET_LINK_STATES, aDest, aSrc, pLogger);
}

ser4cpp::rseq_t LinkFrame::FormatRequestLinkStatus(
    ser4cpp::wseq_t& buffer, bool aIsMaster, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, false, false, LinkFunction::PRI_REQUEST_LINK_STATUS, aDest, aSrc,
                        pLogger);
}

ser4cpp::rseq_t LinkFrame::FormatTestLinkStatus(
    ser4cpp::wseq_t& buffer, bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, Logger* pLogger)
{
    return FormatHeader(buffer, 0, aIsMaster, aFcb, true, LinkFunction::PRI_TEST_LINK_STATES, aDest, aSrc, pLogger);
}

ser4cpp::rseq_t LinkFrame::FormatConfirmedUserData(ser4cpp::wseq_t& buffer,
                                                   bool aIsMaster,
                                                   bool aFcb,
                                                   uint16_t aDest,
                                                   uint16_t aSrc,
                                                   ser4cpp::rseq_t user_data,
                                                   Logger* pLogger)
{
    if (user_data.length() > LPDU_MAX_USER_DATA_SIZE)
    {
        return ser4cpp::rseq_t::empty();
    }

    auto userDataSize = CalcUserDataSize(user_data.length());
    auto ret = buffer.readonly().take(userDataSize + LPDU_HEADER_SIZE);
    FormatHeader(buffer, static_cast<uint8_t>(user_data.length()), aIsMaster, aFcb, true,
                 LinkFunction::PRI_CONFIRMED_USER_DATA, aDest, aSrc, pLogger);
    WriteUserData(user_data, buffer, user_data.length());
    buffer.advance(userDataSize);
    return ret;
}

ser4cpp::rseq_t LinkFrame::FormatUnconfirmedUserData(
    ser4cpp::wseq_t& buffer, bool aIsMaster, uint16_t aDest, uint16_t aSrc, ser4cpp::rseq_t user_data, Logger* pLogger)
{
    if (user_data.length() > LPDU_MAX_USER_DATA_SIZE)
    {
        return ser4cpp::rseq_t::empty();
    }

    auto userDataSize = CalcUserDataSize(user_data.length());
    auto ret = buffer.readonly().take(userDataSize + LPDU_HEADER_SIZE);
    FormatHeader(buffer, static_cast<uint8_t>(user_data.length()), aIsMaster, false, false,
                 LinkFunction::PRI_UNCONFIRMED_USER_DATA, aDest, aSrc, pLogger);
    WriteUserData(user_data, buffer, user_data.length());
    buffer.advance(userDataSize);
    return ret;
}

ser4cpp::rseq_t LinkFrame::FormatHeader(ser4cpp::wseq_t& buffer,
                                        uint8_t aDataLength,
                                        bool aIsMaster,
                                        bool aFcb,
                                        bool aFcvDfc,
                                        LinkFunction aFuncCode,
                                        uint16_t aDest,
                                        uint16_t aSrc,
                                        Logger* pLogger)
{
    if (buffer.length() < LPDU_HEADER_SIZE)
    {
        return ser4cpp::rseq_t::empty();
    }

    LinkHeader header(aDataLength + LPDU_MIN_LENGTH, aSrc, aDest, aIsMaster, aFcvDfc, aFcb, aFuncCode);

    FORMAT_LOGGER_BLOCK(pLogger, flags::LINK_TX, "Function: %s Dest: %u Source: %u Length: %u",
                        LinkFunctionSpec::to_human_string(aFuncCode), aDest, aSrc, aDataLength);

    header.Write(buffer);
    auto ret = buffer.readonly().take(10);
    buffer.advance(10);
    return ret;
}

void LinkFrame::WriteUserData(const uint8_t* pSrc, uint8_t* pDest, size_t length)
{
    while (length > 0)
    {
        uint8_t max = LPDU_DATA_BLOCK_SIZE;
        size_t num = length > max ? max : length;
        memcpy(pDest, pSrc, num);
        CRC::AddCrc(pDest, num);
        pSrc += num;
        pDest += (num + 2);
        length -= num;
    }
}

} // namespace opendnp3
