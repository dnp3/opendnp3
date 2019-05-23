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
#ifndef OPENPAL_IEXECUTOR_H
#define OPENPAL_IEXECUTOR_H

#include "IMonotonicTimeSource.h"
#include "ITimer.h"
#include "TimeDuration.h"

#include <functional>

namespace openpal
{

typedef std::function<void()> action_t;

/**
 * Interface for posting events to a queue.  Events can be posted for
 * immediate consumption or some time in the future.  Events are processed
 * in the order they are received.
 *
 */
class IExecutor : public IMonotonicTimeSource
{

public:
    virtual ~IExecutor() {}

    /// @return a new timer based on a relative time duration
    virtual ITimer* Start(const TimeDuration& duration, const action_t& action) = 0;

    /// @return a new timer based on an absolute timestamp of the monotonic clock
    virtual ITimer* Start(const MonotonicTimestamp& expiration, const action_t& action) = 0;

    /// @return Thread-safe way to post an event to be handled asynchronously
    virtual void Post(const action_t& action) = 0;
};

} // namespace openpal

#endif
