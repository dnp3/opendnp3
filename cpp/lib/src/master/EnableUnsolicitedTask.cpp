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
#include "EnableUnsolicitedTask.h"

#include "MasterTasks.h"
#include "app/APDUBuilders.h"

#include <exe4cpp/IExecutor.h>

namespace opendnp3
{

EnableUnsolicitedTask::EnableUnsolicitedTask(const std::shared_ptr<TaskContext>& context,
                                             IMasterApplication& app,
                                             const TaskBehavior& behavior,
                                             ClassField enabledClasses,
                                             const Logger& logger)
    : IMasterTask(context, app, behavior, logger, TaskConfig::Default()), enabledClasses(enabledClasses)
{
}

bool EnableUnsolicitedTask::BuildRequest(APDURequest& request, uint8_t seq)
{
    build::EnableUnsolicited(request, enabledClasses.OnlyEventClasses(), seq);
    return true;
}

bool EnableUnsolicitedTask::IsEnabled() const
{
    return enabledClasses.HasEventClass();
}

IMasterTask::ResponseResult EnableUnsolicitedTask::ProcessResponse(const APDUResponseHeader& header,
                                                                   const ser4cpp::rseq_t& objects)
{
    return ValidateNullResponse(header, objects) ? ResponseResult::OK_FINAL : ResponseResult::ERROR_BAD_RESPONSE;
}

} // namespace opendnp3
