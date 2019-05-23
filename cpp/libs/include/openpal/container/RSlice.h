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
#ifndef OPENPAL_RSLICE_H
#define OPENPAL_RSLICE_H

#include "HasSize.h"

#include <cstdint>

namespace openpal
{

class WSlice;

/**
 *	Represents a readonly slice of a buffer located elsewhere. Mediates reading from the buffer
 *	to prevent overreads and other errors.
 */
class RSlice : public HasSize<uint32_t>
{

public:
    static RSlice Empty();

    RSlice();

    RSlice(uint8_t const* pBuffer, uint32_t size);

    void Clear();

    RSlice CopyTo(WSlice&) const;

    RSlice Take(uint32_t count) const;

    RSlice Skip(uint32_t count) const;

    bool Equals(const RSlice& rhs) const;

    void Advance(uint32_t count);

    operator uint8_t const*() const
    {
        return pBuffer;
    }

private:
    uint8_t const* pBuffer;
};

} // namespace openpal

#endif
