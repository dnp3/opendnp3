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
#ifndef __BUFFER_SEGMENT_H_
#define __BUFFER_SEGMENT_H_

#include <opendnp3/link/ITransportSegment.h>

#include <testlib/BufferHelpers.h>

#include <string>

namespace opendnp3
{

class BufferSegment final : public ITransportSegment
{
public:
    BufferSegment(uint32_t segmentSize, const std::string& hex, const Addresses& addresses);

    const Addresses& GetAddresses() const override
    {
        return this->addresses;
    }

    bool HasValue() const override;

    openpal::RSlice GetSegment() override;

    bool Advance() override;

    void Reset();

private:
    const Addresses addresses;
    uint32_t segmentSize;
    testlib::HexSequence hs;
    openpal::RSlice remainder;
};

} // namespace opendnp3

#endif
