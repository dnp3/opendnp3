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
#ifndef OPENDNP3_IMASTERSCHEDULER_H
#define OPENDNP3_IMASTERSCHEDULER_H

#include "master/IMasterTask.h"
#include "master/IMasterTaskRunner.h"

namespace opendnp3
{

/**
 * Interface used by master sessions to schedule tasks
 */
class IMasterScheduler
{

public:
    virtual ~IMasterScheduler() {}

    virtual void Shutdown() = 0;

    /**
     * Add a single task to the scheduler. The tasks will be started asynchronously,
     * i.e. not by the call to this method
     */
    virtual void Add(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner) = 0;

    /**
     * Remove all tasks associated with this context, including the running one
     */
    virtual void SetRunnerOffline(const IMasterTaskRunner& runner) = 0;

    /**
     *
     */
    virtual bool CompleteCurrentFor(const IMasterTaskRunner& runner) = 0;

    /**
     *  Called if task changes in such a way that it might be runnable sooner than scheduled
     */
    virtual void Evaluate() = 0;

    /**
     * Run a task as soon as possible
     */
    virtual void Demand(const std::shared_ptr<IMasterTask>& task) = 0;

    /**
     * Add multiple tasks in one call
     */
    void Add(std::initializer_list<std::shared_ptr<IMasterTask>> tasks, IMasterTaskRunner& runner)
    {
        for (auto& task : tasks)
            this->Add(task, runner);
    }
};

} // namespace opendnp3

#endif
