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
#ifndef OPENDNP3_COMMANDTASK_H
#define OPENDNP3_COMMANDTASK_H

#include "master/IMasterTask.h"
#include "master/TaskPriority.h"

#include "opendnp3/gen/FunctionCode.h"
#include "opendnp3/gen/IndexQualifierMode.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/CommandSet.h"
#include "opendnp3/master/ICommandProcessor.h"
#include "opendnp3/master/ITaskCallback.h"

#include <assert.h>

#include <deque>
#include <memory>

namespace opendnp3
{

// Base class with machinery for performing command operations
class CommandTask : public IMasterTask
{

public:
    CommandTask(const std::shared_ptr<TaskContext>& context,
                CommandSet&& commands,
                IndexQualifierMode mode,
                IMasterApplication& app,
                CommandResultCallbackT callback,
                const Timestamp& startExpiration,
                const TaskConfig& config,
                const Logger& logger);

    static std::shared_ptr<IMasterTask> CreateDirectOperate(const std::shared_ptr<TaskContext>& context,
                                                            CommandSet&& set,
                                                            IndexQualifierMode mode,
                                                            IMasterApplication& app,
                                                            const CommandResultCallbackT& callback,
                                                            const Timestamp& startExpiration,
                                                            const TaskConfig& config,
                                                            Logger logger);
    static std::shared_ptr<IMasterTask> CreateSelectAndOperate(const std::shared_ptr<TaskContext>& context,
                                                               CommandSet&& set,
                                                               IndexQualifierMode mode,
                                                               IMasterApplication& app,
                                                               const CommandResultCallbackT& callback,
                                                               const Timestamp& startExpiration,
                                                               const TaskConfig& config,
                                                               Logger logger);

    virtual char const* Name() const override final
    {
        return "Command Task";
    }

    virtual int Priority() const override final
    {
        return priority::COMMAND;
    }

    virtual bool BlocksLowerPriority() const override final
    {
        return false;
    }

    virtual bool IsRecurring() const override final
    {
        return false;
    }

    virtual bool BuildRequest(APDURequest& request, uint8_t seq) override final;

private:
    virtual bool IsEnabled() const override final
    {
        return true;
    }

    virtual MasterTaskType GetTaskType() const override final
    {
        return MasterTaskType::USER_TASK;
    }

    virtual void Initialize() override final;

    virtual ResponseResult ProcessResponse(const APDUResponseHeader& header,
                                           const ser4cpp::rseq_t& objects) override final;

    virtual void OnTaskComplete(TaskCompletion result, Timestamp now) override final;

    ResponseResult ProcessResponse(const ser4cpp::rseq_t& objects);

    void LoadSelectAndOperate();
    void LoadDirectOperate();

    std::deque<FunctionCode> functionCodes;

    CommandStatus statusResult;
    const CommandResultCallbackT commandCallback;
    CommandSet commands;
    const IndexQualifierMode mode;
};

} // namespace opendnp3

#endif
