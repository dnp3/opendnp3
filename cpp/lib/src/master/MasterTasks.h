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
#ifndef OPENDNP3_MASTERTASKS_H
#define OPENDNP3_MASTERTASKS_H

#include "master/IMasterScheduler.h"

#include "opendnp3/master/IMasterApplication.h"
#include "opendnp3/master/ISOEHandler.h"
#include "opendnp3/master/MasterParams.h"

#include <vector>

namespace opendnp3
{

class MasterTasks
{

public:
    MasterTasks(const MasterParams& params,
                const Logger& logger,
                IMasterApplication& application,
                std::shared_ptr<ISOEHandler> SOEHandler);

    void Initialize(IMasterScheduler& scheduler, IMasterTaskRunner& runner);

    bool DemandTimeSync();
    bool DemandEventScan();
    bool DemandIntegrity();

    void OnRestartDetected();

    void BindTask(const std::shared_ptr<IMasterTask>& task);

    const std::shared_ptr<TaskContext> context;

private:
    bool Demand(const std::shared_ptr<IMasterTask>& task)
    {
        if (task)
        {
            task->SetMinExpiration();
            return true;
        }
        else
        {
            return false;
        }
    }

    inline static TaskBehavior RetryBehavior(const MasterParams& params)
    {
        return TaskBehavior::SingleImmediateExecutionWithRetry(params.taskRetryPeriod, params.maxTaskRetryPeriod);
    }

    const std::shared_ptr<IMasterTask> clearRestart;
    const std::shared_ptr<IMasterTask> assignClass;
    const std::shared_ptr<IMasterTask> startupIntegrity;
    const std::shared_ptr<IMasterTask> eventScan;
    const std::shared_ptr<IMasterTask> disableUnsol;
    const std::shared_ptr<IMasterTask> enableUnsol;
    const std::shared_ptr<IMasterTask> timeSynchronization;

    static std::shared_ptr<IMasterTask> GetTimeSyncTask(const std::shared_ptr<TaskContext>& context,
                                                        TimeSyncMode mode,
                                                        const Logger& logger,
                                                        IMasterApplication& application);
    static std::shared_ptr<IMasterTask> GetEnableUnsolTask(const std::shared_ptr<TaskContext>& context,
                                                           const MasterParams& params,
                                                           const Logger& logger,
                                                           IMasterApplication& application);
    static std::shared_ptr<IMasterTask> GetDisableUnsolTask(const std::shared_ptr<TaskContext>& context,
                                                            const MasterParams& params,
                                                            const Logger& logger,
                                                            IMasterApplication& application);

    std::vector<std::shared_ptr<IMasterTask>> boundTasks;
};

} // namespace opendnp3

#endif
