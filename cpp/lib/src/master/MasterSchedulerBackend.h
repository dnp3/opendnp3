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
#ifndef OPENDNP3_MASTERSCHEDULERBACKEND_H
#define OPENDNP3_MASTERSCHEDULERBACKEND_H

#include "master/IMasterScheduler.h"
#include "master/IMasterTaskRunner.h"

#include <exe4cpp/Timer.h>

#include <memory>
#include <vector>

namespace opendnp3
{

class MasterSchedulerBackend final : public IMasterScheduler,
                                     public std::enable_shared_from_this<MasterSchedulerBackend>
{

    // Tasks are associated with a particular runner
    struct Record
    {
        Record() = default;

        Record(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner) : task(task), runner(&runner) {}

        operator bool()
        {
            return task && runner;
        }

        void Clear()
        {
            this->task.reset();
            this->runner = nullptr;
        }

        bool BelongsTo(const IMasterTaskRunner& runner) const
        {
            return this->runner == &runner;
        }

        std::shared_ptr<IMasterTask> task;
        IMasterTaskRunner* runner = nullptr;
    };

public:
    explicit MasterSchedulerBackend(const std::shared_ptr<exe4cpp::IExecutor>& executor);

    virtual void Shutdown() override;

    // ------- implement IMasterScheduler --------

    virtual void Add(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner) override;

    virtual void SetRunnerOffline(const IMasterTaskRunner& runner) override;

    virtual bool CompleteCurrentFor(const IMasterTaskRunner& runner) override;

    virtual void Demand(const std::shared_ptr<IMasterTask>& task) override;

    virtual void Evaluate() override;

private:
    bool isShutdown = false;
    bool taskCheckPending = false;

    Record current;
    std::vector<Record> tasks;

    void PostCheckForTaskRun();

    bool CheckForTaskRun();

    void RestartTimeoutTimer();

    void TimeoutTasks();

    std::shared_ptr<exe4cpp::IExecutor> executor;
    exe4cpp::Timer taskTimer;
    exe4cpp::Timer taskStartTimeout;

    enum class Comparison : uint8_t
    {
        LEFT,
        RIGHT,
        SAME
    };

    static Comparison GetBestTaskToRun(const Timestamp& now, const Record& left, const Record& right);

    static Comparison CompareEnabledStatus(const Record& left, const Record& right);

    static Comparison CompareBlockedStatus(const Record& left, const Record& right);

    static Comparison ComparePriority(const Record& left, const Record& right);

    static Comparison CompareTime(const Timestamp& now, const Record& left, const Record& right);
};

} // namespace opendnp3

#endif
