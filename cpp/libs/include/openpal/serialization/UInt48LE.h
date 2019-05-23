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
#ifndef OPENPAL_UINT48LE_H
#define OPENPAL_UINT48LE_H

#include "UInt48Type.h"

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include <cstdint>
#include <cstring>

namespace openpal
{

class UInt48LE
{
public:
    static UInt48Type Read(const uint8_t* data);

    static void Write(uint8_t* data, UInt48Type value);

    inline static UInt48Type ReadBuffer(RSlice& buffer)
    {
        auto ret = Read(buffer);
        buffer.Advance(SIZE);
        return ret;
    }

    static void WriteBuffer(WSlice& buffer, UInt48Type value)
    {
        Write(buffer, value);
        buffer.Advance(SIZE);
    }

    const static int64_t MAX = 281474976710655ULL; // 2^48 -1
    const static size_t SIZE = 6;
    typedef UInt48Type Type;
};

} // namespace openpal

#endif
