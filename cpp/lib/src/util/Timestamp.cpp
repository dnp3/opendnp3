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
#include "opendnp3/util/Timestamp.h"

#include <exe4cpp/Typedefs.h>

namespace opendnp3
{

Timestamp Timestamp::Max()
{
    return Timestamp(exe4cpp::steady_time_t::max());
}

Timestamp Timestamp::Min()
{
    return Timestamp(exe4cpp::steady_time_t::min());
}

bool Timestamp::IsMax() const
{
    return value == exe4cpp::steady_time_t::max();
}

bool Timestamp::IsMin() const
{
    return value == exe4cpp::steady_time_t::min();
}

Timestamp::Timestamp() : value(exe4cpp::steady_time_t::min()) {}

Timestamp::Timestamp(exe4cpp::steady_time_t value) : value(value) {}

Timestamp Timestamp::operator+(const TimeDuration& duration) const
{
    const auto maximum = exe4cpp::steady_time_t::max() - value;

    return duration.value >= maximum ? Timestamp::Max() : Timestamp(value + duration.value);
}

Timestamp& Timestamp::operator+=(const TimeDuration& duration)
{
    const auto result = *this + duration;
    this->value = result.value;
    return *this;
}

Timestamp Timestamp::operator-(const TimeDuration& duration) const
{
    const auto maximum = value - exe4cpp::steady_time_t::min();

    return duration.value >= maximum ? Timestamp::Min() : Timestamp(value - duration.value);
}

Timestamp& Timestamp::operator-=(const TimeDuration& duration)
{
    const auto result = *this - duration;
    this->value = result.value;
    return *this;
}

TimeDuration Timestamp::operator-(const Timestamp& timestamp) const
{
    return TimeDuration(this->value - timestamp.value);
}

bool Timestamp::operator==(const Timestamp& other) const
{
    return this->value == other.value;
}

bool Timestamp::operator!=(const Timestamp& other) const
{
    return this->value != other.value;
}

bool Timestamp::operator<(const Timestamp& other) const
{
    return this->value < other.value;
}

bool Timestamp::operator<=(const Timestamp& other) const
{
    return this->value <= other.value;
}

bool Timestamp::operator>(const Timestamp& other) const
{
    return this->value > other.value;
}

bool Timestamp::operator>=(const Timestamp& other) const
{
    return this->value >= other.value;
}

} // namespace opendnp3
