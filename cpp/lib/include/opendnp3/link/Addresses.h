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
#ifndef OPENDNP3_ADDRESSES_H
#define OPENDNP3_ADDRESSES_H

#include <cstdint>

namespace opendnp3
{

struct Addresses
{
    Addresses() = default;

    Addresses(uint16_t source, uint16_t destination) : source(source), destination(destination) {}

    Addresses Reverse() const
    {
        return Addresses(this->destination, this->source);
    }

    inline bool operator==(const Addresses& other) const
    {
        return (this->source == other.source) && (this->destination == other.destination);
    }

    inline bool operator!=(const Addresses& other) const
    {
        return !((*this) == other);
    }

    bool IsBroadcast() const;

    uint16_t source = 0;
    uint16_t destination = 0;
};

} // namespace opendnp3

#endif
