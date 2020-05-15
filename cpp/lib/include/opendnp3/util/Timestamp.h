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
#ifndef OPENDNP3_TIMESTAMP_H
#define OPENDNP3_TIMESTAMP_H

#include "TimeDuration.h"

namespace opendnp3
{

/**
 *  Strong typing for millisecond-based monotonic timestamps
 */
class Timestamp
{

public:
    static Timestamp Max();
    static Timestamp Min();

    Timestamp();
    explicit Timestamp(std::chrono::steady_clock::time_point value);

    bool IsMax() const;
    bool IsMin() const;

    // overflow capped to maximum value
    Timestamp operator+(const TimeDuration& duration) const;
    Timestamp& operator+=(const TimeDuration& duration);
    Timestamp operator-(const TimeDuration& duration) const;
    Timestamp& operator-=(const TimeDuration& duration);

    TimeDuration operator-(const Timestamp& timestamp) const;

    bool operator==(const Timestamp& other) const;
    bool operator!=(const Timestamp& other) const;
    bool operator<(const Timestamp& other) const;
    bool operator<=(const Timestamp& other) const;
    bool operator>(const Timestamp& other) const;
    bool operator>=(const Timestamp& other) const;

    std::chrono::steady_clock::time_point value;
};

} // namespace opendnp3

#endif
