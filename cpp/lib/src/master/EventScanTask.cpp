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

#include "EventScanTask.h"

#include "MasterTasks.h"
#include "app/APDUBuilders.h"
#include "app/parsing/APDUParser.h"
#include "logging/LogMacros.h"
#include "master/MeasurementHandler.h"

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

EventScanTask::EventScanTask(const std::shared_ptr<TaskContext>& context,
                             IMasterApplication& application,
                             std::shared_ptr<ISOEHandler> soeHandler,
                             ClassField classes,
                             const Logger& logger)
    : PollTaskBase(
          context, application, std::move(soeHandler), TaskBehavior::ReactsToIINOnly(), logger, TaskConfig::Default()),
      classes(classes.OnlyEventClasses())
{
}

bool EventScanTask::BuildRequest(APDURequest& request, uint8_t seq)
{
    build::ClassRequest(request, FunctionCode::READ, classes, seq);
    return true;
}

bool EventScanTask::IsEnabled() const
{
    return classes.HasEventClass();
}

} // namespace opendnp3
