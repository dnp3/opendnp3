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
#ifndef OPENDNP3_PREFIXFIELDS_H
#define OPENDNP3_PREFIXFIELDS_H

#include "opendnp3/util/StaticOnly.h"

#include <ser4cpp/container/SequenceTypes.h>
#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

class PrefixFields : private StaticOnly
{
public:
    template<typename... Args> static bool Read(ser4cpp::rseq_t& input, Args&... fields)
    {
        if (input.length() < (sizeof...(Args) * ser4cpp::UInt16::size))
        {
            // not enough in the buffer to even read the length prefixes
            return false;
        }

        uint16_t lengths[sizeof...(Args)];

        for (size_t i = 0; i < sizeof...(Args); ++i)
        {
            ser4cpp::UInt16::read_from(input, lengths[i]);
        }

        size_t sum = 0;

        for (size_t i = 0; i < sizeof...(Args); ++i)
        {
            sum += lengths[i];
        }

        if (input.length() < sum)
        {
            // not enough for the defined fields
            return false;
        }

        ReadFields(input, lengths, fields...);

        return true;
    }

    template<typename... Args> static bool Write(ser4cpp::wseq_t& dest, const Args&... fields)
    {
        const auto total_size = (sizeof...(Args) * ser4cpp::UInt16::size) + SumSizes(fields...);

        if (dest.length() < total_size)
        {
            return false;
        }

        WriteLengths(dest, fields...);
        WriteFields(dest, fields...);

        return true;
    }

    template<typename... Args> static bool LengthFitsInUInt16(const ser4cpp::rseq_t& arg1, Args&... fields)
    {
        return (arg1.length() <= std::numeric_limits<uint16_t>::max()) && LengthFitsInUInt16(fields...);
    }

private:
    template<typename... Args> static size_t SumSizes(const ser4cpp::rseq_t& arg1, Args&... fields)
    {
        return arg1.length() + SumSizes(fields...);
    }

    static bool LengthFitsInUInt16()
    {
        return true;
    }

    static size_t SumSizes()
    {
        return 0;
    }

    template<typename... Args>
    static void ReadFields(ser4cpp::rseq_t& input, uint16_t* pLength, ser4cpp::rseq_t& output, Args&... fields)
    {
        output = input.take(*pLength);
        input.advance(*pLength);
        ReadFields(input, ++pLength, fields...);
    }

    static void ReadFields(ser4cpp::rseq_t& input, uint16_t* pLength) {}

    template<typename... Args>
    static void WriteLengths(ser4cpp::wseq_t& dest, const ser4cpp::rseq_t& arg1, Args&... fields)
    {
        ser4cpp::UInt16::write_to(dest, static_cast<uint16_t>(arg1.length()));
        WriteLengths(dest, fields...);
    }

    static void WriteLengths(ser4cpp::wseq_t& dest) {}

    template<typename... Args>
    static void WriteFields(ser4cpp::wseq_t& dest, const ser4cpp::rseq_t& arg1, Args&... fields)
    {
        dest.copy_from(arg1);
        WriteFields(dest, fields...);
    }

    static void WriteFields(ser4cpp::wseq_t& dest) {}
};

} // namespace opendnp3

#endif
