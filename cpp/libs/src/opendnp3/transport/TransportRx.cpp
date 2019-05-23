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
#include "TransportRx.h"

#include "TransportConstants.h"
#include "TransportHeader.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/util/ToHex.h>

#include "opendnp3/LogLevels.h"

#include <cstring>

using namespace std;
using namespace openpal;

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

openpal::WSlice TransportRx::GetAvailable()
{
    return rxBuffer.GetWSlice().Skip(numBytesRead);
}

Message TransportRx::ProcessReceive(const Message& segment)
{
    ++statistics.numTransportRx;

    if (segment.payload.IsEmpty())
    {
        FORMAT_LOG_BLOCK(logger, flags::WARN, "Received tpdu with no header");
        ++statistics.numTransportErrorRx;
        return Message();
    }

    const TransportHeader header(segment.payload[0]);

    const auto payload = segment.payload.Skip(1);

    FORMAT_LOG_BLOCK(logger, flags::TRANSPORT_RX, "FIR: %d FIN: %d SEQ: %u LEN: %u", header.fir, header.fin, header.seq,
                     payload.Size());

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

    if (payload.Size() > available.Size())
    {
        // transport buffer overflow
        ++statistics.numTransportBufferOverflow;
        SIMPLE_LOG_BLOCK(logger, flags::WARN, "Exceeded the buffer size before a complete fragment was read");
        this->numBytesRead = 0;
        return Message();
    }

    payload.CopyTo(available);

    this->numBytesRead += payload.Size();
    this->lastAddresses = segment.addresses;
    this->expectedSeq = header.seq;
    this->expectedSeq.Increment();

    if (header.fin)
    {
        const auto ret = rxBuffer.ToRSlice().Take(numBytesRead);
        this->numBytesRead = 0;
        return Message(segment.addresses, ret);
    }

    return Message();
}

} // namespace opendnp3
