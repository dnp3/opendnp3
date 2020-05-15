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

#ifndef OPENDNP3_TASKBEHAVIOR_H
#define OPENDNP3_TASKBEHAVIOR_H

#include "opendnp3/util/TimeDuration.h"
#include "opendnp3/util/Timestamp.h"

namespace opendnp3
{

/**
 *   All of the configuration parameters that control how the task wil behave
 */
class TaskBehavior
{

public:
    static TaskBehavior SingleExecutionNoRetry();

    static TaskBehavior SingleExecutionNoRetry(const Timestamp& startExpiration);

    static TaskBehavior ImmediatePeriodic(const TimeDuration& period,
                                          const TimeDuration& minRetryDelay,
                                          const TimeDuration& maxRetryDelay);

    static TaskBehavior SingleImmediateExecutionWithRetry(const TimeDuration& minRetryDelay,
                                                          const TimeDuration& maxRetryDelay);

    static TaskBehavior ReactsToIINOnly();

    /**
     * Called when the task succeeds. Resets the retry timeout to the minimum, and returns the new expiration time
     */
    void OnSuccess(const Timestamp& now);

    /**
     * Called when the task fails due to a response timeout
     */
    void OnResponseTimeout(const Timestamp& now);

    /**
     * return the current expiration time
     */
    Timestamp GetExpiration() const
    {
        return expiration;
    }

    /**
     * return the time after which the task should fail if it hasn't start running
     */
    Timestamp GetStartExpiration() const
    {
        return startExpiration;
    }

    /**
     * reset to the initial state
     */
    void Reset();

    /**
     * Disable the task
     */
    void Disable();

private:
    TimeDuration CalcNextRetryTimeout() const;

    TaskBehavior() = delete;

    TaskBehavior(const TimeDuration& period,
                 const Timestamp& expiration,
                 const TimeDuration& minRetryDelay,
                 const TimeDuration& maxRetryDelay,
                 const Timestamp& startExpiration);

    const TimeDuration period;
    const TimeDuration minRetryDelay;
    const TimeDuration maxRetryDelay;
    const Timestamp startExpiration;

    // permanently disable the task
    bool disabled = false;

    // The tasks current expiration time
    Timestamp expiration;

    // The current retry delay
    TimeDuration currentRetryDelay;
};

} // namespace opendnp3

#endif
