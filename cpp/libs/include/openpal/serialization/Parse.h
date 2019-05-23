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
#ifndef OPENPAL_PARSE_H
#define OPENPAL_PARSE_H

#include "openpal/container/RSlice.h"
#include "openpal/serialization/UInt48Type.h"
#include "openpal/util/Uncopyable.h"

namespace openpal
{
class Parse : private StaticOnly
{
public:
    static bool Read(RSlice& input, uint8_t& output);
    static bool Read(RSlice& input, uint16_t& output);

    static bool Read(RSlice& input, uint32_t& output);
    static bool Read(RSlice& input, UInt48Type& output);

    static bool Read(RSlice& input, int16_t& output);
    static bool Read(RSlice& input, int32_t& output);

    static bool Read(RSlice& input, double& output);
    static bool Read(RSlice& input, float& output);

    template<typename T, typename... Args> static bool Many(RSlice& input, T& output, Args&... args)
    {
        return Read(input, output) && Many(input, args...);
    }

private:
    static bool Many(RSlice& input)
    {
        return true;
    }
};

} // namespace openpal

#endif
