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
#ifndef OPENPAL_SERIALIZER_H
#define OPENPAL_SERIALIZER_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include <cstdint>

namespace openpal
{

template<class T> class Serializer
{
public:
    typedef bool (*ReadFunc)(RSlice& buffer, T& output);
    typedef bool (*WriteFunc)(const T& value, WSlice& buffer);

    Serializer() : size(0), pReadFunc(nullptr), pWriteFunc(nullptr) {}

    Serializer(uint32_t size_, ReadFunc pReadFunc_, WriteFunc pWriteFunc_)
        : size(size_), pReadFunc(pReadFunc_), pWriteFunc(pWriteFunc_)
    {
    }

    /**
     * @return The size (in bytes) required for every call to read/write
     */
    uint32_t Size() const
    {
        return size;
    }

    /**
     * reads the value and advances the read buffer
     */
    bool Read(RSlice& buffer, T& output) const
    {
        return (*pReadFunc)(buffer, output);
    }

    /**
     * writes the value and advances the write buffer
     */
    bool Write(const T& value, WSlice& buffer) const
    {
        return (*pWriteFunc)(value, buffer);
    }

private:
    uint32_t size;
    ReadFunc pReadFunc;
    WriteFunc pWriteFunc;
};

} // namespace openpal

#endif
