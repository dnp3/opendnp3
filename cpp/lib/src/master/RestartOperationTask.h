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
#ifndef OPENDNP3_RESTART_OPERATION_TASK_H
#define OPENDNP3_RESTART_OPERATION_TASK_H

#include "app/parsing/IAPDUHandler.h"
#include "master/IMasterTask.h"
#include "master/TaskPriority.h"

#include "opendnp3/gen/RestartType.h"
#include "opendnp3/master/RestartOperationResult.h"

namespace opendnp3
{

class RestartOperationTask final : public IMasterTask, private IAPDUHandler
{

public:
    RestartOperationTask(const std::shared_ptr<TaskContext>& context,
                         IMasterApplication& app,
                         const Timestamp& startTimeout,
                         RestartType operationType,
                         RestartOperationCallbackT callback,
                         const Logger& logger,
                         const TaskConfig& config);

    bool BuildRequest(APDURequest& request, uint8_t seq) override;

    int Priority() const override
    {
        return priority::USER_REQUEST;
    }

    bool BlocksLowerPriority() const override
    {
        return false;
    }

    bool IsRecurring() const override
    {
        return false;
    }

    char const* Name() const override;

    bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override;

private:
    MasterTaskType GetTaskType() const override;

    IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var1>& values) override;
    IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var2>& values) override;

    static FunctionCode ToFunctionCode(RestartType op);

    const FunctionCode function;
    RestartOperationCallbackT callback;
    TimeDuration duration;

    IMasterTask::ResponseResult ProcessResponse(const APDUResponseHeader& header,
                                                const ser4cpp::rseq_t& objects) override;

    void OnTaskComplete(TaskCompletion result, Timestamp now) override;
};

} // namespace opendnp3

#endif
