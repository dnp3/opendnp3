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
#ifndef OPENDNP3_TASKCONTEXT_H
#define OPENDNP3_TASKCONTEXT_H

#include "opendnp3/util/Uncopyable.h"

#include <set>

namespace opendnp3
{

class IMasterTask; // break circular dependency

/**
 *
 * Allows tasks requiring sequenced execution order to block lower priority tasks
 *
 * Every master session will initialize its tasks with a shared_ptr to a TaskContext
 *
 */
class TaskContext : private Uncopyable
{
    std::set<const IMasterTask*> blocking_tasks;

public:
    void AddBlock(const IMasterTask& task);

    void RemoveBlock(const IMasterTask& task);

    bool IsBlocked(const IMasterTask& task) const;
};

} // namespace opendnp3

#endif
