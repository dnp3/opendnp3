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

#include "TransportHeader.h"

namespace opendnp3
{

TransportHeader::TransportHeader(uint8_t byte)
    : fir((byte & FIR_MASK) != 0), fin((byte & FIN_MASK) != 0), seq((byte & SEQ_MASK))
{
}

uint8_t TransportHeader::ToByte(bool fir, bool fin, uint8_t seq)
{
    uint8_t hdr = 0;

    if (fir)
    {
        hdr |= FIR_MASK;
    }

    if (fin)
    {
        hdr |= FIN_MASK;
    }

    // Only the lower 6 bits of the sequence number
    hdr |= (SEQ_MASK & seq);

    return hdr;
}

} // namespace opendnp3
