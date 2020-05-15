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

#include "master/RestartOperationTask.h"

#include "app/APDUBuilders.h"
#include "app/parsing/APDUParser.h"
#include "master/TaskPriority.h"

#include <utility>

namespace opendnp3
{

RestartOperationTask::RestartOperationTask(const std::shared_ptr<TaskContext>& context,
                                           IMasterApplication& app,
                                           const Timestamp& startTimeout,
                                           RestartType operationType,
                                           RestartOperationCallbackT callback,
                                           const Logger& logger,
                                           const TaskConfig& config)
    : IMasterTask(context, app, TaskBehavior::SingleExecutionNoRetry(startTimeout), logger, config),
      function((operationType == RestartType::COLD) ? FunctionCode::COLD_RESTART : FunctionCode::WARM_RESTART),
      callback(std::move(callback))
{
}

bool RestartOperationTask::BuildRequest(APDURequest& request, uint8_t seq)
{
    request.SetControl(AppControlField(true, true, false, false, seq));
    request.SetFunction(this->function);
    return true;
}

bool RestartOperationTask::IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode /*qc*/)
{
    if (headerCount != 0)
    {
        return false;
    }

    switch (gv)
    {
    case (GroupVariation::Group52Var1):
    case (GroupVariation::Group52Var2):
        return true;
    default:
        return false;
    }
}

MasterTaskType RestartOperationTask::GetTaskType() const
{
    return MasterTaskType::USER_TASK;
}

char const* RestartOperationTask::Name() const
{
    return FunctionCodeSpec::to_human_string(this->function);
}

IMasterTask::ResponseResult RestartOperationTask::ProcessResponse(const APDUResponseHeader& header,
                                                                  const ser4cpp::rseq_t& objects)
{
    if (!(ValidateSingleResponse(header) && ValidateInternalIndications(header)))
    {
        return ResponseResult::ERROR_BAD_RESPONSE;
    }

    if (objects.is_empty())
    {
        return ResponseResult::ERROR_BAD_RESPONSE;
    }

    auto result = APDUParser::Parse(objects, *this, &logger);

    return (result == ParseResult::OK) ? ResponseResult::OK_FINAL : ResponseResult::ERROR_BAD_RESPONSE;
}

IINField RestartOperationTask::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group52Var1>& values)
{
    Group52Var1 value;
    if (values.ReadOnlyValue(value))
    {
        this->duration = TimeDuration::Milliseconds(value.time);
        return IINField::Empty();
    }

    return IINBit::PARAM_ERROR;
}

IINField RestartOperationTask::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group52Var2>& values)
{
    Group52Var2 value;
    if (values.ReadOnlyValue(value))
    {
        this->duration = TimeDuration::Milliseconds(value.time);
        return IINField::Empty();
    }

    return IINBit::PARAM_ERROR;
}

FunctionCode RestartOperationTask::ToFunctionCode(RestartType op)
{
    return (op == RestartType::COLD) ? FunctionCode::COLD_RESTART : FunctionCode::WARM_RESTART;
}

void RestartOperationTask::OnTaskComplete(TaskCompletion result, Timestamp /*now*/)
{
    if (this->Errors().Any())
    {
        this->callback(RestartOperationResult(TaskCompletion::FAILURE_BAD_RESPONSE, this->duration));
    }
    else
    {
        this->callback(RestartOperationResult(result, this->duration));
    }
}

} // namespace opendnp3
