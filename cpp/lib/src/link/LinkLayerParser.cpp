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
#include "LinkLayerParser.h"

#include "link/CRC.h"
#include "link/IFrameSink.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

LinkLayerParser::LinkLayerParser(const Logger& logger)
    : logger(logger), state(State::FindSync), frameSize(0), buffer(rxBuffer, LPDU_MAX_FRAME_SIZE)
{
}

void LinkLayerParser::Reset()
{
    state = State::FindSync;
    frameSize = 0;
    buffer.Reset();
}

ser4cpp::wseq_t LinkLayerParser::WriteBuff() const
{
    return ser4cpp::wseq_t(buffer.WriteBuff(), buffer.NumWriteBytes());
}

void LinkLayerParser::OnRead(size_t numBytes, IFrameSink& sink)
{
    buffer.AdvanceWrite(numBytes);

    while (ParseUntilComplete() == State::Complete)
    {
        ++statistics.numLinkFrameRx;
        this->PushFrame(sink);
        state = State::FindSync;
    }

    buffer.Shift();
}

LinkLayerParser::State LinkLayerParser::ParseUntilComplete()
{
    auto lastState = this->state;
    // continue as long as we're making progress, i.e. a state change
    while ((this->state = ParseOneStep()) != lastState)
    {
        lastState = state;
    }
    return state;
}

LinkLayerParser::State LinkLayerParser::ParseOneStep()
{
    switch (state)
    {
    case (State::FindSync):
        return ParseSync();
    case (State::ReadHeader):
        return ParseHeader();
    case (State::ReadBody):
        return ParseBody();
    default:
        return state;
    }
}

LinkLayerParser::State LinkLayerParser::ParseSync()
{
    if (this->buffer.NumBytesRead() >= 10) // && buffer.Sync())
    {
        size_t skipCount = 0;
        const auto synced = buffer.Sync(skipCount);
        if (skipCount > 0)
        {
            FORMAT_LOG_BLOCK(logger, flags::WARN, "Skipped %zu bytes seaching for start bytes", skipCount);
        }

        return synced ? State::ReadHeader : State::FindSync;
    }

    return State::FindSync;
}

LinkLayerParser::State LinkLayerParser::ParseHeader()
{
    if (this->buffer.NumBytesRead() >= 10)
    {
        if (this->ReadHeader())
        {
            return State::ReadBody;
        }

        this->FailFrame();
        return State::FindSync;
    }
    else
    {
        return State::ReadHeader;
    }
}

LinkLayerParser::State LinkLayerParser::ParseBody()
{
    if (buffer.NumBytesRead() < this->frameSize)
    {
        return State::ReadBody;
    }

    if (this->ValidateBody())
    {
        this->TransferUserData();
        return State::Complete;
    }

    this->FailFrame();
    return State::FindSync;
}

void LinkLayerParser::PushFrame(IFrameSink& sink)
{
    LinkHeaderFields fields(header.GetFuncEnum(), header.IsFromMaster(), header.IsFcbSet(), header.IsFcvDfcSet(),
                            Addresses(header.GetSrc(), header.GetDest()));

    sink.OnFrame(fields, userData);

    buffer.AdvanceRead(frameSize);
}

void LinkLayerParser::TransferUserData()
{
    uint32_t len = header.GetLength() - LPDU_MIN_LENGTH;
    LinkFrame::ReadUserData(buffer.ReadBuffer() + LPDU_HEADER_SIZE, rxBuffer, len);
    userData = ser4cpp::rseq_t(rxBuffer, len);
}

bool LinkLayerParser::ReadHeader()
{
    header.Read(buffer.ReadBuffer());
    if (CRC::IsCorrectCRC(buffer.ReadBuffer(), LI_CRC))
    {
        return ValidateHeaderParameters();
    }
    else
    {
        ++statistics.numHeaderCrcError;
        SIMPLE_LOG_BLOCK(logger, flags::WARN, "CRC failure in header");
        return false;
    }
}

bool LinkLayerParser::ValidateBody()
{
    uint32_t len = header.GetLength() - LPDU_MIN_LENGTH;
    if (LinkFrame::ValidateBodyCRC(buffer.ReadBuffer() + LPDU_HEADER_SIZE, len))
    {
        FORMAT_LOG_BLOCK(logger, flags::LINK_RX, "Function: %s Dest: %u Source: %u Length: %u",
                         LinkFunctionSpec::to_human_string(header.GetFuncEnum()), header.GetDest(), header.GetSrc(),
                         header.GetLength());

        FORMAT_HEX_BLOCK(logger, flags::LINK_RX_HEX, buffer.ReadBuffer().take(frameSize), 10, 18);

        return true;
    }

    ++this->statistics.numBodyCrcError;
    SIMPLE_LOG_BLOCK(logger, flags::ERR, "CRC failure in body");
    return false;
}

bool LinkLayerParser::ValidateHeaderParameters()
{
    if (!header.ValidLength())
    {
        ++statistics.numBadLength;
        FORMAT_LOG_BLOCK(logger, flags::ERR, "LENGTH out of range [5,255]: %i", header.GetLength());
        return false;
    }

    // Now make sure that the function code is known and that the FCV is appropriate
    if (!this->ValidateFunctionCode())
    {
        return false;
    }

    uint8_t user_data_length = header.GetLength() - LPDU_MIN_LENGTH;
    frameSize = LinkFrame::CalcFrameSize(user_data_length);
    LinkFunction func = header.GetFuncEnum();

    const bool has_payload = user_data_length > 0;
    const bool should_have_payload
        = (func == LinkFunction::PRI_CONFIRMED_USER_DATA || func == LinkFunction::PRI_UNCONFIRMED_USER_DATA);

    // make sure that the presence/absence of user data matches the function code
    if (should_have_payload && !has_payload)
    {
        ++statistics.numBadLength;
        FORMAT_LOG_BLOCK(logger, flags::ERR, "User data with no payload. FUNCTION: %s",
                         LinkFunctionSpec::to_human_string(func));
        return false;
    }

    if (!should_have_payload && has_payload)
    {
        ++statistics.numBadLength;
        FORMAT_LOG_BLOCK(logger, flags::ERR, "Unexpected LENGTH in frame: %i with FUNCTION: %s", user_data_length,
                         LinkFunctionSpec::to_human_string(func));
        return false;
    }

    // calculate the total frame size
    frameSize = LinkFrame::CalcFrameSize(user_data_length);

    return true;
}

void LinkLayerParser::FailFrame()
{
    // All you have to do is advance the reader by one, when the resync happens the data will disappear
    buffer.AdvanceRead(1);
}

bool LinkLayerParser::ValidateFunctionCode()
{
    // Now make sure that the function code is known and that the FCV is appropriate
    if (header.IsPriToSec())
    {
        bool fcv_set = false;

        switch (header.GetFuncEnum())
        {
        case (LinkFunction::PRI_CONFIRMED_USER_DATA):
        case (LinkFunction::PRI_TEST_LINK_STATES):
            fcv_set = true;
            break;
        case (LinkFunction::PRI_REQUEST_LINK_STATUS):
        case (LinkFunction::PRI_RESET_LINK_STATES):
        case (LinkFunction::PRI_UNCONFIRMED_USER_DATA):
            break;
        default:
        {
            ++statistics.numBadFunctionCode;
            FORMAT_LOG_BLOCK(logger, flags::WARN, "Unknown PriToSec FUNCTION: %s",
                             LinkFunctionSpec::to_human_string(header.GetFuncEnum()));
            return false;
        }
        }

        // now check the fcv
        if (fcv_set != header.IsFcvDfcSet())
        {
            ++statistics.numBadFCV;
            FORMAT_LOG_BLOCK(logger, flags::WARN, "Bad FCV for FUNCTION: %s",
                             LinkFunctionSpec::to_human_string(header.GetFuncEnum()));
            return false;
        }

        // if fcv isn't expected to be set, fcb can be either 1 or 0, doesn't matter
    }
    else // SecToPri - just validate the function codes and that FCB is 0
    {
        switch (header.GetFuncEnum())
        {
        case (LinkFunction::SEC_ACK):
        case (LinkFunction::SEC_NACK):
        case (LinkFunction::SEC_LINK_STATUS):
        case (LinkFunction::SEC_NOT_SUPPORTED):
            break;
        default:
        {
            ++statistics.numBadFunctionCode;
            FORMAT_LOG_BLOCK(logger, flags::ERR, "Unknown SecToPri FUNCTION: %s",
                             LinkFunctionSpec::to_human_string(header.GetFuncEnum()));
            return false;
        }
        }

        // now check the fcb, it should always be zero
        if (header.IsFcbSet())
        {
            ++statistics.numBadFCB;
            FORMAT_LOG_BLOCK(logger, flags::ERR, "FCB set for SecToPri FUNCTION: %s",
                             LinkFunctionSpec::to_human_string(header.GetFuncEnum()));
            return false;
        }
    }

    return true; // valid!
}

} // namespace opendnp3
