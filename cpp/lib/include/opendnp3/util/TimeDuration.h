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
#ifndef OPENDNP3_TIMEDURATION_H
#define OPENDNP3_TIMEDURATION_H

#include <chrono>
#include <cstdint>
#include <string>

namespace opendnp3
{

/**
 *  Strong typing for millisecond based time durations
 */
class TimeDuration
{
    friend class Timestamp;

public:
    static TimeDuration Min();

    static TimeDuration Max();

    static TimeDuration Zero();

    static TimeDuration Milliseconds(int64_t milliseconds);

    static TimeDuration Seconds(int64_t seconds);

    static TimeDuration Minutes(int64_t minutes);    

    TimeDuration();

    TimeDuration Double() const;

    bool IsNegative() const;

    std::string ToString() const;

    bool operator==(const TimeDuration& other) const;
    bool operator<(const TimeDuration& other) const;
    bool operator<=(const TimeDuration& other) const;
    bool operator>(const TimeDuration& other) const;
    bool operator>=(const TimeDuration& other) const;

    std::chrono::steady_clock::duration value;

private:
    template<class T> static TimeDuration FromValue(int64_t value);

    explicit TimeDuration(std::chrono::steady_clock::duration value);
};

} // namespace opendnp3

#endif
