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
#include "TransportTx.h"

#include "TransportHeader.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/LogLevels.h"

#include <cassert>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportTx::TransportTx(const openpal::Logger& logger) : logger(logger) {}

void TransportTx::Configure(const Message& message)
{
    assert(message.payload.IsNotEmpty());
    txSegment.Clear();
    this->message = message;
    this->tpduCount = 0;
}

bool TransportTx::HasValue() const
{
    return this->message.payload.Size() > 0;
}

openpal::RSlice TransportTx::GetSegment()
{
    if (txSegment.IsSet())
    {
        return txSegment.Get();
    }

    const uint32_t numToSend
        = (this->message.payload.Size() < MAX_TPDU_PAYLOAD) ? this->message.payload.Size() : MAX_TPDU_PAYLOAD;

    auto dest = tpduBuffer.GetWSlice().Skip(1);
    this->message.payload.Take(numToSend).CopyTo(dest);

    bool fir = (tpduCount == 0);
    bool fin = (numToSend == this->message.payload.Size());
    tpduBuffer()[0] = TransportHeader::ToByte(fir, fin, sequence);

    FORMAT_LOG_BLOCK(logger, flags::TRANSPORT_TX, "FIR: %d FIN: %d SEQ: %u LEN: %u", fir, fin, sequence.Get(),
                     numToSend);

    ++statistics.numTransportTx;

    auto segment = tpduBuffer.ToRSlice(numToSend + 1);
    txSegment.Set(segment);
    return segment;
}

bool TransportTx::Advance()
{
    txSegment.Clear();
    uint32_t numToSend
        = this->message.payload.Size() < MAX_TPDU_PAYLOAD ? this->message.payload.Size() : MAX_TPDU_PAYLOAD;
    this->message.payload.Advance(numToSend);
    ++tpduCount;
    sequence.Increment();
    return this->message.payload.IsNotEmpty();
}

} // namespace opendnp3
