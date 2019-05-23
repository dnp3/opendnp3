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
#include "BufferSegment.h"

#include <openpal/util/ToHex.h>

#include <testlib/HexConversions.h>

#include <memory.h>

#include <algorithm>
#include <cassert>
#include <exception>
#include <sstream>

using namespace std;
using namespace openpal;
using namespace testlib;

namespace opendnp3
{

BufferSegment::BufferSegment(uint32_t segmentSize, const std::string& hex, const Addresses& addresses)
    : addresses(addresses), segmentSize(segmentSize), hs(hex), remainder(hs.ToRSlice())
{
    assert(segmentSize > 0);
}

void BufferSegment::Reset()
{
    remainder = hs.ToRSlice();
}

bool BufferSegment::HasValue() const
{
    return remainder.Size() > 0;
}

openpal::RSlice BufferSegment::GetSegment()
{
    auto size = std::min(segmentSize, remainder.Size());
    auto chunk = remainder.Take(size);
    return chunk;
}

bool BufferSegment::Advance()
{
    auto size = std::min(segmentSize, remainder.Size());
    remainder.Advance(size);
    return remainder.IsNotEmpty();
}

} // namespace opendnp3
