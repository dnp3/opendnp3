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
#include "utils/BufferHelpers.h"

#include <ser4cpp/util/HexConversions.h>

#include <algorithm>
#include <cassert>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>

ByteStr::ByteStr(size_t length, uint8_t seed) : CopyableBuffer(length)
{
    for (size_t i = 0; i < length; ++i)
        this->buffer[i] = static_cast<uint8_t>((i + seed) % 256);
}

ByteStr::ByteStr(const uint8_t* data, size_t length) : CopyableBuffer(data, length) {}

bool ByteStr::operator==(const ByteStr& other) const
{
    if (Size() != other.Size())
        return false;

    for (size_t i = 0; i < Size(); ++i)
        if (this->buffer[i] != other[i])
            return false;

    return true;
}

std::string ByteStr::ToHex() const
{
    return ser4cpp::HexConversions::to_hex(ToRSeq());
}

HexSequence::HexSequence(const std::string& hex) : ByteStr(Validate(RemoveSpaces(hex)))
{
    std::string s = RemoveSpaces(hex);

    size_t size = s.size();
    for (size_t index = 0, pos = 0; pos < size; ++index, pos += 2)
    {
        uint32_t val;
        std::stringstream ss;
        ss << std::hex << s.substr(pos, 2);
        if ((ss >> val).fail())
        {
            throw std::invalid_argument(hex);
        }
        this->buffer[index] = static_cast<uint8_t>(val);
    }
}

std::string HexSequence::RemoveSpaces(const std::string& hex)
{
    std::string copy(hex);
    RemoveSpacesInPlace(copy);
    return copy;
}

void HexSequence::RemoveSpacesInPlace(std::string& s)
{
    size_t pos = s.find_first_of(' ');
    if (pos != std::string::npos)
    {
        s.replace(pos, 1, "");
        RemoveSpacesInPlace(s);
    }
}

size_t HexSequence::Validate(const std::string& s)
{
    // annoying when you accidentally put an 'O' instead of zero '0'
    if (s.find_first_of("oO") != std::string::npos)
    {
        throw std::invalid_argument("Sequence contains 'o' or 'O'");
    }

    if (s.size() % 2 != 0)
    {
        throw std::invalid_argument(s);
    }

    return s.size() / 2;
}
