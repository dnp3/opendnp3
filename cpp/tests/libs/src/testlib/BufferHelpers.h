/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef TESTLIB_BUFFERHELPERS_H
#define TESTLIB_BUFFERHELPERS_H

#include "CopyableBuffer.h"

#include <string>

namespace testlib
{

class ByteStr : public CopyableBuffer
{

public:
    ByteStr(uint32_t length, uint8_t seed = 0);
    ByteStr(const uint8_t* data, uint32_t length);
    bool operator==(const ByteStr& other) const;
    std::string ToHex() const;
};

/**
 * A sequence of hex values in the form "01 02 03 04" that are stored as a ByteStr.
 */
class HexSequence : public ByteStr
{
public:
    HexSequence(const std::string& hex);

    operator openpal::RSlice()
    {
        return this->ToRSlice();
    }

private:
    std::string RemoveSpaces(const std::string& hex);
    void RemoveSpacesInPlace(std::string& hex);
    static uint32_t Validate(const std::string& hex);
};

} // namespace testlib

#endif
