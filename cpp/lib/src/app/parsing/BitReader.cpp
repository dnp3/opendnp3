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
#include "BitReader.h"

#include <cassert>

namespace opendnp3
{

size_t NumBytesInBits(size_t numBits)
{
    size_t numBytes = numBits / 8;
    return ((numBits % 8) == 0) ? numBytes : numBytes + 1;
}

bool GetBit(const ser4cpp::rseq_t& buffer, size_t position)
{
    size_t byte = position / 8;
    size_t bit = position % 8;
    assert(byte < buffer.length());
    return (buffer[byte] & (1 << bit)) != 0;
}

size_t NumBytesInDoubleBits(size_t numBits)
{
    size_t numBytes = numBits / 4;
    return ((numBits % 4) == 0) ? numBytes : numBytes + 1;
}

DoubleBit GetDoubleBit(const ser4cpp::rseq_t& buffer, size_t index)
{
    size_t byteNumber = index / 4;
    assert(byteNumber < buffer.length());
    uint8_t byte = buffer[byteNumber];
    size_t bitshift = 2 * (index % 4);
    return DoubleBitSpec::from_type((byte >> bitshift) & 0x03);
}

} // namespace opendnp3
