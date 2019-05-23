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
#ifndef OPENPAL_STATICBUFFER_H
#define OPENPAL_STATICBUFFER_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"
#include "openpal/util/Comparisons.h"

#include <cstdint>

namespace openpal
{

template<uint32_t SIZE> class StaticBuffer
{

public:
    StaticBuffer() {}

    virtual ~StaticBuffer() {}

    RSlice ToRSlice() const
    {
        return RSlice(buffer, SIZE);
    }

    RSlice ToRSlice(uint32_t maxSize) const
    {
        return RSlice(buffer, openpal::Min(SIZE, maxSize));
    }

    WSlice GetWSlice()
    {
        return WSlice(buffer, SIZE);
    }

    WSlice GetWSlice(uint32_t maxSize)
    {
        return WSlice(buffer, openpal::Min(SIZE, maxSize));
    }

    const uint8_t* operator()() const
    {
        return buffer;
    }

    uint8_t* operator()()
    {
        return buffer;
    }

    uint32_t Size() const
    {
        return SIZE;
    }

private:
    uint8_t buffer[SIZE];
};

} // namespace openpal

#endif
