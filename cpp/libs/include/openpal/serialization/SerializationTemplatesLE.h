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
#ifndef OPENPAL_SERIALIZATIONTEMPLATESLE_H
#define OPENPAL_SERIALIZATIONTEMPLATESLE_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"
#include "openpal/util/Limits.h"

#include <cstdint>
#include <cstring>

namespace openpal
{

template<class T> class Bit16LE
{
public:
    static T Read(const uint8_t* data)
    {
        return (static_cast<T>(data[0]) << 0) | (static_cast<T>(data[1]) << 8);
    }

    static void Write(uint8_t* data, T value)
    {
        data[0] = static_cast<uint8_t>(value & 0xFF);
        data[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    }

    static void WriteBuffer(WSlice& buffer, T aValue)
    {
        Write(buffer, aValue);
        buffer.Advance(SIZE);
    }

    inline static T ReadBuffer(RSlice& arBuffer)
    {
        auto ret = Read(arBuffer);
        arBuffer.Advance(SIZE);
        return ret;
    }

    typedef T Type;

    const static size_t SIZE = sizeof(T);
    const static T Max;
    const static T Min;
};

template<class T> const T Bit16LE<T>::Max = openpal::MaxValue<T>();

template<class T> const T Bit16LE<T>::Min = openpal::MinValue<T>();

template<class T> class Bit32LE
{
public:
    // Endianness doesn't apply to everything. If you do bitwise or bitshift operations on an int, you don't notice the
    // endianness. The machine arranges the multiple bytes, so the least significant byte is still the least significant
    // byte, and the most significant byte is still the most significant byte

    // This is endian independent of the machine order
    static T Read(const uint8_t* data)
    {
        return (static_cast<T>(data[0]) << 0) | (static_cast<T>(data[1]) << 8) | (static_cast<T>(data[2]) << 16)
            | (static_cast<T>(data[3]) << 24);
    }

    static void Write(uint8_t* data, T value)
    {
        data[0] = static_cast<uint8_t>(value & 0xFF);
        data[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        data[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
        data[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
    }

    static void WriteBuffer(WSlice& buffer, T aValue)
    {
        Write(buffer, aValue);
        buffer.Advance(SIZE);
    }

    inline static T ReadBuffer(RSlice& buffer)
    {
        auto ret = Read(buffer);
        buffer.Advance(SIZE);
        return ret;
    }

    typedef T Type;

    const static size_t SIZE = sizeof(T);
    const static T Max;
    const static T Min;
};

template<class T> const T Bit32LE<T>::Max = openpal::MaxValue<T>();

template<class T> const T Bit32LE<T>::Min = openpal::MinValue<T>();

} // namespace openpal

#endif
