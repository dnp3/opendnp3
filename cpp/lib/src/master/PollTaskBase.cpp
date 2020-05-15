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
#include "PollTaskBase.h"

#include "logging/LogMacros.h"
#include "master/MeasurementHandler.h"

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

PollTaskBase::PollTaskBase(const std::shared_ptr<TaskContext>& context,
                           IMasterApplication& application,
                           std::shared_ptr<ISOEHandler> handler,
                           const TaskBehavior& behavior,
                           const Logger& logger,
                           TaskConfig config)
    : IMasterTask(context, application, behavior, logger, config), handler(std::move(handler))
{
}

void PollTaskBase::Initialize()
{
    this->rxCount = 0;
}

IMasterTask::ResponseResult PollTaskBase::ProcessResponse(const APDUResponseHeader& header,
                                                          const ser4cpp::rseq_t& objects)
{
    if (header.control.FIR)
    {
        if (this->rxCount > 0)
        {
            SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected FIR frame");
            return ResponseResult::ERROR_BAD_RESPONSE;
        }

        return ProcessMeasurements(header, objects);
    }
    else
    {
        if (this->rxCount > 0)
        {
            return ProcessMeasurements(header, objects);
        }

        SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected non-FIR frame");
        return ResponseResult::ERROR_BAD_RESPONSE;
    }
}

IMasterTask::ResponseResult PollTaskBase::ProcessMeasurements(const APDUResponseHeader& header,
                                                              const ser4cpp::rseq_t& objects)
{
    ++rxCount;

    if (MeasurementHandler::ProcessMeasurements(header.as_response_info(), objects, logger, handler.get())
        == ParseResult::OK)
    {
        return header.control.FIN ? ResponseResult::OK_FINAL : ResponseResult::OK_CONTINUE;
    }

    return ResponseResult::ERROR_BAD_RESPONSE;
}

} // namespace opendnp3
