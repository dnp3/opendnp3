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
#ifndef OPENDNP3_IOPENDELAYSTRATEGY_H
#define OPENDNP3_IOPENDELAYSTRATEGY_H

#include "opendnp3/util/TimeDuration.h"
#include "opendnp3/util/Uncopyable.h"

namespace opendnp3
{

/**
 * A strategy interface for controlling how connection are retried
 */
class IOpenDelayStrategy
{

public:
    virtual ~IOpenDelayStrategy() {}

    /**
     * The the next delay based on the current and the maximum.
     */
    virtual TimeDuration GetNextDelay(const TimeDuration& current, const TimeDuration& max) const = 0;
};

/**
 * Implements IOpenDelayStrategy using exponential-backoff.
 */
class ExponentialBackoffStrategy final : public IOpenDelayStrategy, private Uncopyable
{
    static ExponentialBackoffStrategy instance;

public:
    static IOpenDelayStrategy& Instance();

    TimeDuration GetNextDelay(const TimeDuration& current, const TimeDuration& max) const final;
};

} // namespace opendnp3

#endif
