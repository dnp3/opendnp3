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
#ifndef OPENDNP3_OCTETDATA_H
#define OPENDNP3_OCTETDATA_H

#include "opendnp3/util/Buffer.h"

#include <array>
#include <cstdint>

namespace opendnp3
{

/**
 * A base-class for bitstrings containing up to 255 bytes
 */
class OctetData
{
public:
    const static uint8_t MAX_SIZE = 255;

    /**
     * Construct with a default value of [0x00] (length == 1)
     */
    OctetData();

    /**
     * Construct from a c-style string
     *
     * strlen() is used internally to determine the length
     *
     * If the length is 0, the default value of [0x00] is assigned
     * If the length is > 255, only the first 255 bytes are copied.
     *
     * The null terminator is NOT copied as part of buffer
     */
    OctetData(const char* input);

    /**
     * Construct from read-only buffer slice
     *
     *
     * If the length is 0, the default value of [0x00] is assigned
     * If the length is > 255, only the first 255 bytes are copied.
     *
     * The null terminator is NOT copied as part of buffer
     */
    OctetData(const Buffer& input);

    inline uint8_t Size() const
    {
        return size;
    }

    /**
     * Set the octet data to the input buffer
     *
     * If the length is 0, the default value of [0x00] is assigned
     * If the length is > 255, only the first 255 bytes are copied
     *
     * @param input the input data to copy into this object
     *
     * @return true if the input meets the length requirements, false otherwise
     */
    bool Set(const Buffer& input);

    /**
     * Set the buffer equal to the supplied c-string
     *
     * If the length is 0, the default value of [0x00] is assigned
     * If the length is > 255, only the first 255 bytes are copied
     *
     * @param input c-style string to copy into this object
     *
     * @return true if the input meets the length requirements, false otherwise
     */
    bool Set(const char* input);

    /**
     * @return a view of the current data
     */
    const Buffer ToBuffer() const;

private:
    static const Buffer ToSlice(const char* input);

    std::array<uint8_t, MAX_SIZE> buffer = {0x00};
    uint8_t size;
};

} // namespace opendnp3

#endif
