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
#include "TransportTx.h"

#include "TransportHeader.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <ser4cpp/serialization/LittleEndian.h>

#include <cassert>

namespace opendnp3
{

TransportTx::TransportTx(const Logger& logger) : logger(logger) {}

void TransportTx::Configure(const Message& message)
{
    assert(message.payload.is_not_empty());
    txSegment.clear();
    this->message = message;
    this->tpduCount = 0;
}

bool TransportTx::HasValue() const
{
    return this->message.payload.length() > 0;
}

ser4cpp::rseq_t TransportTx::GetSegment()
{
    if (txSegment.is_set())
    {
        return txSegment.get();
    }

    const size_t numToSend
        = (this->message.payload.length() < MAX_TPDU_PAYLOAD) ? this->message.payload.length() : MAX_TPDU_PAYLOAD;

    auto dest = tpduBuffer.as_wseq().skip(1);
    dest.copy_from(this->message.payload.take(numToSend));

    bool fir = (tpduCount == 0);
    bool fin = (numToSend == this->message.payload.length());
    auto destHeader = tpduBuffer.as_wseq();
    ser4cpp::UInt8::write_to(destHeader, TransportHeader::ToByte(fir, fin, sequence));

    FORMAT_LOG_BLOCK(logger, flags::TRANSPORT_TX, "FIR: %d FIN: %d SEQ: %u LEN: %zu", fir, fin, sequence.Get(),
                     numToSend);

    ++statistics.numTransportTx;

    auto segment = tpduBuffer.as_seq(numToSend + 1);
    txSegment.set(segment);
    return segment;
}

bool TransportTx::Advance()
{
    txSegment.clear();
    size_t numToSend
        = this->message.payload.length() < MAX_TPDU_PAYLOAD ? this->message.payload.length() : MAX_TPDU_PAYLOAD;
    this->message.payload.advance(numToSend);
    ++tpduCount;
    sequence.Increment();
    return this->message.payload.is_not_empty();
}

} // namespace opendnp3
