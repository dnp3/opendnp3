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
#include "opendnp3/app/OctetData.h"

#include <ser4cpp/container/SequenceTypes.h>
#include <ser4cpp/util/Comparisons.h>

#include <cstring>

namespace opendnp3
{

OctetData::OctetData() : size(1) {}

OctetData::OctetData(const char* input) : OctetData(ToSlice(input)) {}

OctetData::OctetData(const ser4cpp::rseq_t& input) : size(input.is_empty() ? 1 : ser4cpp::min<uint32_t>(MAX_SIZE, input.length()))
{
    if (input.is_not_empty())
    {
        auto dest = buffer.as_wseq();
        dest.copy_from(input.take(size));
    }
}

bool OctetData::Set(const ser4cpp::rseq_t& input)
{
    if (input.is_empty())
    {
        this->size = 0;
        this->buffer.as_wseq()[0] = 0x00;
        return false;
    }

    const bool is_oversized = input.length() > MAX_SIZE;
    const uint8_t usable_size = is_oversized ? MAX_SIZE : static_cast<uint8_t>(input.length());

    auto dest = this->buffer.as_wseq();
    dest.copy_from(input.take(usable_size));
    this->size = usable_size;
    return !is_oversized;
}

bool OctetData::Set(const char* input)
{
    const size_t length = strlen(input);
    return this->Set(ser4cpp::rseq_t(reinterpret_cast<const uint8_t*>(input),
                                     static_cast<uint32_t>(length > MAX_SIZE ? MAX_SIZE : length)));
}

ser4cpp::rseq_t OctetData::ToRSeq() const
{
    return buffer.as_seq(size);
}

ser4cpp::rseq_t OctetData::ToSlice(const char* input)
{
    const size_t length = strlen(input);
    if (length == 0)
        return ser4cpp::rseq_t::empty();
    return ser4cpp::rseq_t(reinterpret_cast<const uint8_t*>(input),
                           length > MAX_SIZE ? MAX_SIZE : static_cast<uint32_t>(length));
}

} // namespace opendnp3
