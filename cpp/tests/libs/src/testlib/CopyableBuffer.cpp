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
#include "CopyableBuffer.h"

#include "HexConversions.h"

#include <openpal/container/WSlice.h>
#include <openpal/util/ToHex.h>

#include <memory.h>

using namespace testlib;

namespace testlib
{

std::ostream& operator<<(std::ostream& output, const CopyableBuffer& arBuff)
{
    output << "[" << ToHex(arBuff.ToRSlice(), true) << "]";
    return output;
}

CopyableBuffer::CopyableBuffer() : buffer(nullptr), size(0) {}

CopyableBuffer::CopyableBuffer(uint32_t size) : buffer(new uint8_t[size]), size(size)
{
    this->Zero();
}

CopyableBuffer::CopyableBuffer(const openpal::RSlice& data) : CopyableBuffer(data, data.Size()) {}

CopyableBuffer::CopyableBuffer(const uint8_t* data, uint32_t size) : buffer(new uint8_t[size]), size(size)
{
    memcpy(buffer, data, size);
}

CopyableBuffer::CopyableBuffer(const CopyableBuffer& other) : CopyableBuffer(other.buffer, other.size) {}

void CopyableBuffer::Zero()
{
    memset(this->buffer, 0, size);
}

CopyableBuffer& CopyableBuffer::operator=(const CopyableBuffer& other)
{
    // check for assignment to self
    if (this == &other)
        return *this;

    if (other.Size() != this->size)
    {
        this->size = other.Size();
        delete[] this->buffer;
        this->buffer = new uint8_t[this->size];
    }

    memcpy(this->buffer, other.buffer, this->size);

    return *this;
}

CopyableBuffer::~CopyableBuffer()
{
    delete[] buffer;
}

bool CopyableBuffer::operator==(const CopyableBuffer& other) const
{
    if (other.Size() != this->Size())
        return false;

    for (size_t i = 0; i < this->Size(); ++i)
    {
        if (this->buffer[i] != other.buffer[i])
            return false;
    }

    return true;
}

} // namespace testlib
