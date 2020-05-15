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
#ifndef OPENDNP3_RESTART_OPERATION_RESULT_H
#define OPENDNP3_RESTART_OPERATION_RESULT_H

#include "opendnp3/gen/TaskCompletion.h"
#include "opendnp3/util/TimeDuration.h"

#include <cstdint>
#include <functional>

namespace opendnp3
{

class RestartOperationResult
{
public:
    RestartOperationResult() : summary(TaskCompletion::FAILURE_NO_COMMS) {}

    RestartOperationResult(TaskCompletion summary_, TimeDuration restartTime_)
        : summary(summary_), restartTime(restartTime_)
    {
    }

    /// The result of the task as a whole.
    TaskCompletion summary;

    /// Time delay until restart
    TimeDuration restartTime;
};

typedef std::function<void(const RestartOperationResult&)> RestartOperationCallbackT;

} // namespace opendnp3

#endif
