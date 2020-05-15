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
#include "TransportRx.h"

#include "TransportConstants.h"
#include "TransportHeader.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <ser4cpp/util/HexConversions.h>

#include <cstring>

namespace opendnp3
{

TransportRx::TransportRx(const Logger& logger, uint32_t maxRxFragSize)
    : logger(logger), rxBuffer(maxRxFragSize), numBytesRead(0)
{
}

void TransportRx::Reset()
{
    this->ClearRxBuffer();
}

void TransportRx::ClearRxBuffer()
{
    numBytesRead = 0;
}

ser4cpp::wseq_t TransportRx::GetAvailable()
{
    return rxBuffer.as_wslice().skip(numBytesRead);
}

Message TransportRx::ProcessReceive(const Message& segment)
{
    ++statistics.numTransportRx;

    if (segment.payload.is_empty())
    {
        FORMAT_LOG_BLOCK(logger, flags::WARN, "Received tpdu with no header");
        ++statistics.numTransportErrorRx;
        return Message();
    }

    const TransportHeader header(segment.payload[0]);

    const auto payload = segment.payload.skip(1);

    FORMAT_LOG_BLOCK(logger, flags::TRANSPORT_RX, "FIR: %d FIN: %d SEQ: %u LEN: %zu", header.fir, header.fin,
                     header.seq, payload.length());

    if (header.fir && this->numBytesRead > 0)
    {
        ++statistics.numTransportDiscard;
        SIMPLE_LOG_BLOCK(logger, flags::WARN, "FIR received mid-fragment, discarding previous bytes");
        this->numBytesRead = 0;
        // continue processing
    }

    // there are special checks we must perform if it isn't the first packet
    if (!header.fir)
    {
        if (this->numBytesRead == 0)
        {
            ++statistics.numTransportIgnore;
            SIMPLE_LOG_BLOCK(logger, flags::WARN, "non-FIR packet with 0 prior bytes");
            return Message(); // drop the data
        }

        if (header.seq != this->expectedSeq)
        {
            ++statistics.numTransportIgnore;
            FORMAT_LOG_BLOCK(logger, flags::WARN, "Received segment w/ seq: %u, expected: %u", header.seq,
                             this->expectedSeq.Get());
            return Message(); // drop the data
        }

        if (segment.addresses != this->lastAddresses)
        {
            ++statistics.numTransportIgnore;
            FORMAT_LOG_BLOCK(logger, flags::WARN,
                             "Bad addressing: last { src: %u, dest: %u } received { src: %u, dest: %u}",
                             this->lastAddresses.source, this->lastAddresses.destination, segment.addresses.source,
                             segment.addresses.destination);
            return Message(); // drop the data
        }
    }

    auto available = this->GetAvailable();

    if (payload.length() > available.length())
    {
        // transport buffer overflow
        ++statistics.numTransportBufferOverflow;
        SIMPLE_LOG_BLOCK(logger, flags::WARN, "Exceeded the buffer size before a complete fragment was read");
        this->numBytesRead = 0;
        return Message();
    }

    available.copy_from(payload);

    this->numBytesRead += payload.length();
    this->lastAddresses = segment.addresses;
    this->expectedSeq = header.seq;
    this->expectedSeq.Increment();

    if (header.fin)
    {
        const auto ret = rxBuffer.as_rslice().take(numBytesRead);
        this->numBytesRead = 0;
        return Message(segment.addresses, ret);
    }

    return Message();
}

} // namespace opendnp3
