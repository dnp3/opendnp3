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
#include "CommandTask.h"

#include "app/parsing/APDUParser.h"
#include "logging/LogMacros.h"
#include "master/CommandSetOps.h"
#include "master/ICommandHeader.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

std::shared_ptr<IMasterTask> CommandTask::CreateDirectOperate(const std::shared_ptr<TaskContext>& context,
                                                              CommandSet&& set,
                                                              IndexQualifierMode mode,
                                                              IMasterApplication& app,
                                                              const CommandResultCallbackT& callback,
                                                              const Timestamp& startExpiration,
                                                              const TaskConfig& config,
                                                              Logger logger)
{
    auto task
        = std::make_shared<CommandTask>(context, std::move(set), mode, app, callback, startExpiration, config, logger);
    task->LoadDirectOperate();
    return task;
}

std::shared_ptr<IMasterTask> CommandTask::CreateSelectAndOperate(const std::shared_ptr<TaskContext>& context,
                                                                 CommandSet&& set,
                                                                 IndexQualifierMode mode,
                                                                 IMasterApplication& app,
                                                                 const CommandResultCallbackT& callback,
                                                                 const Timestamp& startExpiration,
                                                                 const TaskConfig& config,
                                                                 Logger logger)
{
    auto task
        = std::make_shared<CommandTask>(context, std::move(set), mode, app, callback, startExpiration, config, logger);
    task->LoadSelectAndOperate();
    return task;
}

CommandTask::CommandTask(const std::shared_ptr<TaskContext>& context,
                         CommandSet&& commands,
                         IndexQualifierMode mode,
                         IMasterApplication& app,
                         CommandResultCallbackT callback,
                         const Timestamp& startExpiration,
                         const TaskConfig& config,
                         const Logger& logger)
    : IMasterTask(context, app, TaskBehavior::SingleExecutionNoRetry(startExpiration), logger, config),
      statusResult(CommandStatus::UNDEFINED),
      commandCallback(std::move(callback)),
      commands(std::move(commands)),
      mode(mode)
{
}

void CommandTask::LoadSelectAndOperate()
{
    functionCodes.clear();
    functionCodes.push_back(FunctionCode::SELECT);
    functionCodes.push_back(FunctionCode::OPERATE);
}

void CommandTask::LoadDirectOperate()
{
    functionCodes.clear();
    functionCodes.push_back(FunctionCode::DIRECT_OPERATE);
}

bool CommandTask::BuildRequest(APDURequest& request, uint8_t seq)
{
    if (!functionCodes.empty())
    {
        request.SetFunction(functionCodes.front());
        functionCodes.pop_front();
        request.SetControl(AppControlField::Request(seq));
        auto writer = request.GetWriter();
        return CommandSetOps::Write(commands, writer, this->mode);
    }

    return false;
}

IMasterTask::ResponseResult CommandTask::ProcessResponse(const APDUResponseHeader& header,
                                                         const ser4cpp::rseq_t& objects)
{
    return ValidateSingleResponse(header) ? ProcessResponse(objects) : ResponseResult::ERROR_BAD_RESPONSE;
}

void CommandTask::OnTaskComplete(TaskCompletion result, Timestamp /*now*/)
{
    CommandSetOps::InvokeCallback(commands, result, this->commandCallback);
}

void CommandTask::Initialize()
{
    statusResult = CommandStatus::UNDEFINED;
}

IMasterTask::ResponseResult CommandTask::ProcessResponse(const ser4cpp::rseq_t& objects)
{
    if (functionCodes.empty())
    {
        auto result = CommandSetOps::ProcessOperateResponse(commands, objects, &logger);
        return (result == CommandSetOps::OperateResult::FAIL_PARSE) ? ResponseResult::ERROR_BAD_RESPONSE
                                                                    : ResponseResult::OK_FINAL;
    }

    auto result = CommandSetOps::ProcessSelectResponse(commands, objects, &logger);

    switch (result)
    {
    case (CommandSetOps::SelectResult::OK):
        return ResponseResult::OK_REPEAT; // proceed to OPERATE
    case (CommandSetOps::SelectResult::FAIL_SELECT):
        return ResponseResult::OK_FINAL; // end the task, let the user examine each command point
    default:
        return ResponseResult::ERROR_BAD_RESPONSE;
    }
}

} // namespace opendnp3
