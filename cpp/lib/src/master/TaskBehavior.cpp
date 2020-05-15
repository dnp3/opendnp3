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

#include "TaskBehavior.h"

#include <limits>

namespace opendnp3
{

TaskBehavior TaskBehavior::SingleExecutionNoRetry()
{
    return SingleExecutionNoRetry(Timestamp::Max()); // no start expiration
}

TaskBehavior TaskBehavior::SingleExecutionNoRetry(const Timestamp& startExpiration)
{
    return TaskBehavior(TimeDuration::Min(), // not periodic
                        Timestamp::Min(),    // run immediately
                        TimeDuration::Max(), TimeDuration::Max(), startExpiration);
}

TaskBehavior TaskBehavior::ImmediatePeriodic(const TimeDuration& period,
                                             const TimeDuration& minRetryDelay,
                                             const TimeDuration& maxRetryDelay)
{
    return TaskBehavior(period,
                        Timestamp::Min(), // run immediately
                        minRetryDelay, maxRetryDelay,
                        Timestamp::Max() // no start expiraion
    );
}

TaskBehavior TaskBehavior::SingleImmediateExecutionWithRetry(const TimeDuration& minRetryDelay,
                                                             const TimeDuration& maxRetryDelay)
{
    return TaskBehavior(TimeDuration::Min(), // not periodic
                        Timestamp::Min(),    // run immediatey
                        minRetryDelay, maxRetryDelay, Timestamp::Max());
}

TaskBehavior TaskBehavior::ReactsToIINOnly()
{
    return TaskBehavior(TimeDuration::Min(), // not periodic
                        Timestamp::Max(),    // only run when needed
                        TimeDuration::Max(), // never retry
                        TimeDuration::Max(), Timestamp::Max());
}

TaskBehavior::TaskBehavior(const TimeDuration& period,
                           const Timestamp& expiration,
                           const TimeDuration& minRetryDelay,
                           const TimeDuration& maxRetryDelay,
                           const Timestamp& startExpiration)
    : period(period),
      minRetryDelay(minRetryDelay),
      maxRetryDelay(maxRetryDelay),
      startExpiration(startExpiration),
      expiration(expiration),
      currentRetryDelay(minRetryDelay)
{
}

void TaskBehavior::OnSuccess(const Timestamp& now)
{
    this->currentRetryDelay = this->minRetryDelay;
    this->expiration = this->period.IsNegative() ? Timestamp::Max() : now + this->period;
}

void TaskBehavior::OnResponseTimeout(const Timestamp& now)
{
    this->expiration = now + this->currentRetryDelay;
    this->currentRetryDelay = this->CalcNextRetryTimeout();
}

void TaskBehavior::Reset()
{
    this->disabled = false;
    this->expiration = Timestamp::Min();
    this->currentRetryDelay = this->minRetryDelay;
}

void TaskBehavior::Disable()
{
    this->disabled = true;
    this->expiration = Timestamp::Max();
}

TimeDuration TaskBehavior::CalcNextRetryTimeout() const
{
    const auto doubled = this->currentRetryDelay.Double();
    return (doubled > this->maxRetryDelay) ? this->maxRetryDelay : doubled;
}

} // namespace opendnp3
