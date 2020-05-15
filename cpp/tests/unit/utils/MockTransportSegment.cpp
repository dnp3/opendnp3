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
#include "utils/MockTransportSegment.h"

#include <ser4cpp/util/HexConversions.h>

#include <algorithm>
#include <cassert>
#include <exception>
#include <memory>
#include <sstream>

using namespace std;
using namespace opendnp3;
using namespace ser4cpp;

MockTransportSegment::MockTransportSegment(size_t segmentSize, const std::string& hex, const Addresses& addresses)
    : addresses(addresses), segmentSize(segmentSize), hs(hex), remainder(hs.ToRSeq())
{
    assert(segmentSize > 0);
}

void MockTransportSegment::Reset()
{
    remainder = hs.ToRSeq();
}

bool MockTransportSegment::HasValue() const
{
    return remainder.length() > 0;
}

ser4cpp::rseq_t MockTransportSegment::GetSegment()
{
    auto size = std::min(segmentSize, remainder.length());
    auto chunk = remainder.take(size);
    return chunk;
}

bool MockTransportSegment::Advance()
{
    auto size = std::min(segmentSize, remainder.length());
    remainder.advance(size);
    return remainder.is_not_empty();
}
