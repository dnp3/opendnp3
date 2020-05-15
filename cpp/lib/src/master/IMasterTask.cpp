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
#include "IMasterTask.h"

#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

IMasterTask::IMasterTask(std::shared_ptr<TaskContext> context,
                         IMasterApplication& app,
                         TaskBehavior behavior,
                         const Logger& logger,
                         TaskConfig config)
    : context(std::move(context)), application(&app), logger(logger), config(config), behavior(std::move(behavior))
{
}

IMasterTask::~IMasterTask()
{
    context->RemoveBlock(*this);

    if (config.pCallback)
    {
        config.pCallback->OnDestroyed();
    }
}

Timestamp IMasterTask::ExpirationTime() const
{
    return this->IsEnabled() ? this->behavior.GetExpiration() : Timestamp::Max();
}

Timestamp IMasterTask::StartExpirationTime() const
{
    return this->behavior.GetStartExpiration();
}

IMasterTask::ResponseResult IMasterTask::OnResponse(const APDUResponseHeader& response,
                                                    const ser4cpp::rseq_t& objects,
                                                    Timestamp now)
{
    auto result = this->ProcessResponse(response, objects);

    switch (result)
    {
    case (ResponseResult::ERROR_BAD_RESPONSE):
        this->CompleteTask(TaskCompletion::FAILURE_BAD_RESPONSE, now);
        break;
    case (ResponseResult::OK_FINAL):
        this->CompleteTask(TaskCompletion::SUCCESS, now);
        break;
    default:
        break;
    }

    return result;
}

void IMasterTask::CompleteTask(TaskCompletion result, Timestamp now)
{
    switch (result)
    {

    // retry immediately when the comms come back online
    case (TaskCompletion::FAILURE_NO_COMMS):
        this->behavior.Reset();
        break;

    // back-off exponentially using the task retry
    case (TaskCompletion::FAILURE_RESPONSE_TIMEOUT):
    {
        this->behavior.OnResponseTimeout(now);
        if (this->BlocksLowerPriority())
            this->context->AddBlock(*this);
        break;
    }

    case (TaskCompletion::SUCCESS):
        this->behavior.OnSuccess(now);
        this->context->RemoveBlock(*this);
        break;

    /**
    FAILURE_BAD_RESPONSE
    FAILURE_START_TIMEOUT
    FAILURE_MESSAGE_FORMAT_ERROR
    */
    default:
    {
        this->behavior.Disable();
        if (this->BlocksLowerPriority())
            this->context->AddBlock(*this);
    }
    }

    if (config.pCallback)
    {
        config.pCallback->OnComplete(result);
    }

    // notify the application
    this->application->OnTaskComplete(TaskInfo(this->GetTaskType(), result, config.taskId));

    // notify any super class implementations
    this->OnTaskComplete(result, now);
}

void IMasterTask::OnResponseTimeout(Timestamp now)
{
    this->CompleteTask(TaskCompletion::FAILURE_RESPONSE_TIMEOUT, now);
}

void IMasterTask::OnLowerLayerClose(Timestamp now)
{
    this->CompleteTask(TaskCompletion::FAILURE_NO_COMMS, now);
}

void IMasterTask::OnStartTimeout(Timestamp now)
{
    this->CompleteTask(TaskCompletion::FAILURE_START_TIMEOUT, now);
}

void IMasterTask::OnMessageFormatError(Timestamp now)
{
    this->CompleteTask(TaskCompletion::FAILURE_MESSAGE_FORMAT_ERROR, now);
}

void IMasterTask::OnStart()
{
    if (config.pCallback)
    {
        config.pCallback->OnStart();
    }

    this->application->OnTaskStart(this->GetTaskType(), config.taskId);

    this->Initialize();
}

void IMasterTask::SetMinExpiration()
{
    this->behavior.Reset();
}

bool IMasterTask::ValidateSingleResponse(const APDUResponseHeader& header)
{
    if (header.control.FIR && header.control.FIN)
    {
        return true;
    }

    SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected response FIR/FIN not set");
    return false;
}

bool IMasterTask::ValidateNullResponse(const APDUResponseHeader& header, const ser4cpp::rseq_t& objects)
{
    return ValidateSingleResponse(header) && ValidateNoObjects(objects) && ValidateInternalIndications(header);
}

bool IMasterTask::ValidateInternalIndications(const APDUResponseHeader& header)
{
    if (header.IIN.HasRequestError())
    {
        FORMAT_LOG_BLOCK(logger, flags::WARN, "Task was explicitly rejected via response with error IIN bit(s): %s",
                         this->Name());
        return false;
    }

    return true;
}

bool IMasterTask::ValidateNoObjects(const ser4cpp::rseq_t& objects)
{
    if (objects.is_empty())
    {
        return true;
    }

    FORMAT_LOG_BLOCK(logger, flags::WARN, "Received unexpected response object headers for task: %s", this->Name());
    return false;
}

} // namespace opendnp3
