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
#include "ShiftableBuffer.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace opendnp3
{

ShiftableBuffer::ShiftableBuffer(uint8_t* pBuffer_, size_t size)
    : pBuffer(pBuffer_), M_SIZE(size), writePos(0), readPos(0)
{
}

void ShiftableBuffer::Shift()
{
    auto numRead = this->NumBytesRead();

    // copy all unread data to the front of the buffer
    memmove(pBuffer, pBuffer + readPos, numRead);

    readPos = 0;
    writePos = numRead;
}

void ShiftableBuffer::Reset()
{
    writePos = 0;
    readPos = 0;
}

void ShiftableBuffer::AdvanceRead(size_t numBytes)
{
    assert(numBytes <= this->NumBytesRead());
    readPos += numBytes;
}

void ShiftableBuffer::AdvanceWrite(size_t aNumBytes)
{
    assert(aNumBytes <= this->NumWriteBytes());
    writePos += aNumBytes;
}

bool ShiftableBuffer::Sync(size_t& skipCount)
{
    while (this->NumBytesRead() > 1) // at least 2 bytes
    {
        if (this->ReadBuffer()[0] == 0x05 && this->ReadBuffer()[1] == 0x64)
        {
            return true;
        }

        this->AdvanceRead(1); // skip the first byte
        ++skipCount;
    }

    return false;
}

} // namespace opendnp3
