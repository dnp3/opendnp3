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
#include "MasterSchedulerBackend.h"

#include <algorithm>

namespace opendnp3
{

MasterSchedulerBackend::MasterSchedulerBackend(const std::shared_ptr<exe4cpp::IExecutor>& executor) : executor(executor)
{
}

void MasterSchedulerBackend::Shutdown()
{
    this->isShutdown = true;
    this->tasks.clear();
    this->current.Clear();
    this->taskTimer.cancel();
    this->taskStartTimeout.cancel();
    this->executor.reset();
}

void MasterSchedulerBackend::Add(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner)
{
    if (this->isShutdown)
        return;

    this->tasks.emplace_back(task, runner);
    this->PostCheckForTaskRun();
}

void MasterSchedulerBackend::SetRunnerOffline(const IMasterTaskRunner& runner)
{
    if (this->isShutdown)
        return;

    const auto now = Timestamp(this->executor->get_time());

    auto checkForOwnership = [now, &runner](const Record& record) -> bool {
        if (record.BelongsTo(runner))
        {
            if (!record.task->IsRecurring())
            {
                record.task->OnLowerLayerClose(now);
            }

            return true;
        }

        return false;
    };

    if (this->current && checkForOwnership(this->current))
        this->current.Clear();

    // move erase idiom
    this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), checkForOwnership), this->tasks.end());

    this->PostCheckForTaskRun();
}

bool MasterSchedulerBackend::CompleteCurrentFor(const IMasterTaskRunner& runner)
{
    // no active task
    if (!this->current)
        return false;

    // active task not for this runner
    if (!this->current.BelongsTo(runner))
        return false;

    if (this->current.task->IsRecurring())
    {
        this->Add(this->current.task, *this->current.runner);
    }

    this->current.Clear();

    this->PostCheckForTaskRun();

    return true;
}

void MasterSchedulerBackend::Demand(const std::shared_ptr<IMasterTask>& task)
{
    auto callback = [this, task, self = shared_from_this()]() {
        task->SetMinExpiration();
        this->CheckForTaskRun();
    };

    this->executor->post(callback);
}

void MasterSchedulerBackend::Evaluate()
{
    this->PostCheckForTaskRun();
}

void MasterSchedulerBackend::PostCheckForTaskRun()
{
    if (!this->taskCheckPending)
    {
        this->taskCheckPending = true;
        this->executor->post([this, self = shared_from_this()]() { this->CheckForTaskRun(); });
    }
}

bool MasterSchedulerBackend::CheckForTaskRun()
{
    if (this->isShutdown)
        return false;

    this->taskCheckPending = false;

    this->RestartTimeoutTimer();

    if (this->current)
        return false;

    const auto now = Timestamp(this->executor->get_time());

    // try to find a task that can run
    auto current = this->tasks.begin();
    auto best_task = current;
    if (current == this->tasks.end())
        return false;
    ++current;

    while (current != this->tasks.end())
    {
        if (GetBestTaskToRun(now, *best_task, *current) == Comparison::RIGHT)
        {
            best_task = current;
        }

        ++current;
    }

    // is the task runnable now?
    const auto is_expired = now >= best_task->task->ExpirationTime();
    if (is_expired)
    {
        this->current = *best_task;
        this->tasks.erase(best_task);
        this->current.runner->Run(this->current.task);

        return true;
    }

    auto callback = [this, self = shared_from_this()]() { this->CheckForTaskRun(); };

    this->taskTimer.cancel();
    this->taskTimer = this->executor->start(best_task->task->ExpirationTime().value, callback);

    return false;
}

void MasterSchedulerBackend::RestartTimeoutTimer()
{
    if (this->isShutdown)
        return;

    auto min = Timestamp::Max();

    for (auto& record : this->tasks)
    {
        if (!record.task->IsRecurring() && (record.task->StartExpirationTime() < min))
        {
            min = record.task->StartExpirationTime();
        }
    }

    this->taskStartTimeout.cancel();
    if (min != Timestamp::Max())
    {
        this->taskStartTimeout.cancel();
        this->taskStartTimeout
            = this->executor->start(min.value, [this, self = shared_from_this()]() { this->TimeoutTasks(); });
    }
}

void MasterSchedulerBackend::TimeoutTasks()
{
    if (this->isShutdown)
        return;

    // find the minimum start timeout value
    auto isTimedOut = [now = Timestamp(this->executor->get_time())](const Record& record) -> bool {
        if (record.task->IsRecurring() || record.task->StartExpirationTime() > now)
        {
            return false;
        }

        record.task->OnStartTimeout(now);

        return true;
    };

    // erase-remove idion (https://en.wikipedia.org/wiki/Erase-remove_idiom)
    this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), isTimedOut), this->tasks.end());

    this->RestartTimeoutTimer();
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::GetBestTaskToRun(const Timestamp& now,
                                                                            const Record& left,
                                                                            const Record& right)
{
    const auto BEST_ENABLED_STATUS = CompareEnabledStatus(left, right);

    if (BEST_ENABLED_STATUS != Comparison::SAME)
    {
        // if one task is disabled, return the other task
        return BEST_ENABLED_STATUS;
    }

    const auto BEST_BLOCKED_STATUS = CompareBlockedStatus(left, right);

    if (BEST_BLOCKED_STATUS != Comparison::SAME)
    {
        // if one task is blocked and the other isn't, return the unblocked task
        return BEST_BLOCKED_STATUS;
    }

    const auto EARLIEST_EXPIRATION = CompareTime(now, left, right);
    const auto BEST_PRIORITY = ComparePriority(left, right);

    // if the expiration times are the same, break based on priority, otherwise go with the expiration time
    return (EARLIEST_EXPIRATION == Comparison::SAME) ? BEST_PRIORITY : EARLIEST_EXPIRATION;
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareTime(const Timestamp& now,
                                                                       const Record& left,
                                                                       const Record& right)
{
    // if tasks are already expired, the effective expiration time is NOW
    const auto leftTime = left.task->IsExpired(now) ? now : left.task->ExpirationTime();
    const auto rightTime = right.task->IsExpired(now) ? now : right.task->ExpirationTime();

    if (leftTime < rightTime)
    {
        return Comparison::LEFT;
    }
    if (rightTime < leftTime)
    {
        return Comparison::RIGHT;
    }
    else
    {
        return Comparison::SAME;
    }
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareEnabledStatus(const Record& left, const Record& right)
{
    if (left.task->ExpirationTime() == Timestamp::Max()) // left is disabled, check the right
    {
        return right.task->ExpirationTime() == Timestamp::Max() ? Comparison::SAME : Comparison::RIGHT;
    }
    if (right.task->ExpirationTime() == Timestamp::Max()) // left is enabled, right is disabled
    {
        return Comparison::LEFT;
    }
    else
    {
        // both tasks are enabled
        return Comparison::SAME;
    }
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareBlockedStatus(const Record& left, const Record& right)
{
    if (left.task->IsBlocked())
    {
        return right.task->IsBlocked() ? Comparison::SAME : Comparison::RIGHT;
    }

    return right.task->IsBlocked() ? Comparison::LEFT : Comparison::SAME;
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::ComparePriority(const Record& left, const Record& right)
{
    if (left.task->Priority() < right.task->Priority())
    {
        return Comparison::LEFT;
    }
    if (right.task->Priority() < left.task->Priority())
    {
        return Comparison::RIGHT;
    }
    else
    {
        return Comparison::SAME;
    }
}

} // namespace opendnp3
