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
#include "openpal/executor/TimerRef.h"

namespace openpal
{

TimerRef::TimerRef(openpal::IExecutor& executor) : pExecutor(&executor), pTimer(nullptr) {}

TimerRef::~TimerRef()
{
    this->Cancel();
}

bool TimerRef::IsActive() const
{
    return (pTimer != nullptr);
}

MonotonicTimestamp TimerRef::ExpiresAt() const
{
    return pTimer != nullptr ? pTimer->ExpiresAt() : MonotonicTimestamp::Max();
}

bool TimerRef::Cancel()
{
    if (pTimer != nullptr)
    {
        pTimer->Cancel();
        pTimer = nullptr;
        return true;
    }

    return false;
}

bool TimerRef::StartAction(const TimeDuration& timeout, const action_t& action)
{
    if (pTimer != nullptr)
    {
        return false;
    }

    pTimer = pExecutor->Start(timeout, action);
    return true;
}

bool TimerRef::StartAction(const MonotonicTimestamp& expiration, const action_t& action)
{
    if (pTimer != nullptr)
    {
        return false;
    }

    pTimer = pExecutor->Start(expiration, action);
    return true;
}

void TimerRef::RestartAction(const TimeDuration& timeout, const action_t& action)
{
    if (pTimer != nullptr)
    {
        pTimer->Cancel();
    }

    pTimer = pExecutor->Start(timeout, action);
}

void TimerRef::RestartAction(const MonotonicTimestamp& expiration, const action_t& action)
{
    if (pTimer != nullptr)
    {
        pTimer->Cancel();
    }

    pTimer = pExecutor->Start(expiration, action);
}

} // namespace openpal
