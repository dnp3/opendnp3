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
#include "UserPollTask.h"

#include <utility>

namespace opendnp3
{

UserPollTask::UserPollTask(const std::shared_ptr<TaskContext>& context,
                           HeaderBuilderT builder,
                           const TaskBehavior& behavior,
                           bool recurring,
                           IMasterApplication& app,
                           std::shared_ptr<ISOEHandler> soeHandler,
                           const Logger& logger,
                           TaskConfig config)
    : PollTaskBase(context, app, std::move(soeHandler), behavior, logger, config),
      builder(std::move(builder)),
      recurring(recurring)
{
}

bool UserPollTask::BuildRequest(APDURequest& request, uint8_t seq)
{
    this->rxCount = 0;
    request.SetFunction(FunctionCode::READ);
    request.SetControl(AppControlField::Request(seq));
    auto writer = request.GetWriter();
    return builder(writer);
}

} // namespace opendnp3
