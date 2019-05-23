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
#include "openpal/util/Limits.h"

#include <cfloat>
#include <cstdint>

/// these are implemented in terms of cstdint
/// since AVR doesn't have std::limits
namespace openpal
{

template<> uint8_t MinValue<uint8_t>()
{
    return 0;
}

template<> uint8_t MaxValue<uint8_t>()
{
    return UINT8_MAX;
}

template<> uint16_t MinValue<uint16_t>()
{
    return 0;
}

template<> uint16_t MaxValue<uint16_t>()
{
    return UINT16_MAX;
}

template<> float MinValue<float>()
{
    return -FLT_MAX;
}

template<> float MaxValue<float>()
{
    return FLT_MAX;
}

template<> double MinValue<double>()
{
    return -DBL_MAX;
}

template<> double MaxValue<double>()
{
    return DBL_MAX;
}

template<> int16_t MinValue<int16_t>()
{
    return INT16_MIN;
}

template<> int16_t MaxValue<int16_t>()
{
    return INT16_MAX;
}

template<> int32_t MinValue<int32_t>()
{
    return INT32_MIN;
}

template<> int32_t MaxValue<int32_t>()
{
    return INT32_MAX;
}

template<> uint32_t MinValue<uint32_t>()
{
    return 0;
}

template<> uint32_t MaxValue<uint32_t>()
{
    return UINT32_MAX;
}

template<> int64_t MinValue<int64_t>()
{
    return INT64_MIN;
}

template<> int64_t MaxValue<int64_t>()
{
    return INT64_MAX;
}

} // namespace openpal
