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
#ifndef TESTLIB_COPYABLEBUFFER_H
#define TESTLIB_COPYABLEBUFFER_H

#include <openpal/container/RSlice.h>

#include <stddef.h>

#include <memory>
#include <sstream>

namespace testlib
{

/**
    Implements a dynamic buffer with a safe
    copy constructor. This makes it easier to compose with
    classes without requiring an explicit copy constructor
*/
class CopyableBuffer
{

public:
    // Construct null buffer
    CopyableBuffer();
    // Construct based on starting size of buffer
    CopyableBuffer(uint32_t size);
    CopyableBuffer(const openpal::RSlice&);
    CopyableBuffer(const uint8_t* data, uint32_t size);
    CopyableBuffer(const CopyableBuffer&);
    CopyableBuffer& operator=(const CopyableBuffer&);
    ~CopyableBuffer();

    bool operator==(const CopyableBuffer& other) const;
    bool operator!=(const CopyableBuffer& other) const
    {
        return !(*this == other);
    }

    openpal::RSlice ToRSlice() const
    {
        return openpal::RSlice(buffer, size);
    }

    operator const uint8_t*() const
    {
        return buffer;
    }

    operator uint8_t*()
    {
        return buffer;
    }

    uint32_t Size() const
    {
        return size;
    }

    void Zero();

protected:
    uint8_t* buffer;

private:
    uint32_t size;
};

} // namespace testlib

#endif
