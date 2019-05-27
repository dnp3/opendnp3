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
#include "opendnp3/TimeDuration.h"

namespace opendnp3
{

TimeDuration TimeDuration::Min()
{
    return TimeDuration(exe4cpp::duration_t::min());
}

TimeDuration TimeDuration::Max()
{
    return TimeDuration(exe4cpp::duration_t::max());
}

TimeDuration TimeDuration::Zero()
{
    return TimeDuration(std::chrono::milliseconds(0));
}

TimeDuration TimeDuration::Milliseconds(int64_t milliseconds)
{
    return TimeDuration(std::chrono::milliseconds(milliseconds));
}

TimeDuration TimeDuration::Seconds(int64_t seconds)
{
    return TimeDuration(std::chrono::seconds(seconds));
}

TimeDuration TimeDuration::Minutes(int64_t minutes)
{
    return TimeDuration(std::chrono::minutes(minutes));
}

TimeDuration TimeDuration::Hours(int64_t hours)
{
    return TimeDuration(std::chrono::hours(hours));
}

TimeDuration TimeDuration::Days(int64_t days)
{
    return TimeDuration(std::chrono::hours(24) * days);
}

TimeDuration TimeDuration::Double() const
{
    const bool doubling_would_cause_mult_overflow
        = this->value >= exe4cpp::duration_t::max() / 2;

    return doubling_would_cause_mult_overflow
        ? TimeDuration::Max()
        : TimeDuration(this->value + this->value);
}

bool TimeDuration::IsNegative() const
{
    return *this < TimeDuration::Zero();
}

bool TimeDuration::operator==(const TimeDuration& other) const
{
    return this->value == other.value;
}

bool TimeDuration::operator<(const TimeDuration& other) const
{
    return this->value < other.value;
}

bool TimeDuration::operator<=(const TimeDuration& other) const
{
    return this->value <= other.value;
}

bool TimeDuration::operator>(const TimeDuration& other) const
{
    return this->value > other.value;
}

bool TimeDuration::operator>=(const TimeDuration& other) const
{
    return this->value >= other.value;
}

TimeDuration::TimeDuration(exe4cpp::duration_t value)
    : value(value)
{

}

} // namespace opendnp3
