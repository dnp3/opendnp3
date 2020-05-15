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
#ifndef OPENDNP3_MASTERSCAN_H
#define OPENDNP3_MASTERSCAN_H

#include <exe4cpp/IExecutor.h>

#include "master/IMasterScheduler.h"
#include "master/IMasterTask.h"
#include "opendnp3/master/IMasterScan.h"

#include <memory>

namespace opendnp3
{

/**
 * Provides access to a permanently bound scan
 */
class MasterScan final : public IMasterScan
{
public:
    MasterScan() = default;

    MasterScan(std::shared_ptr<IMasterTask> task, std::shared_ptr<IMasterScheduler> scheduler);

    static std::shared_ptr<MasterScan> Create(const std::shared_ptr<IMasterTask>& task,
                                              const std::shared_ptr<IMasterScheduler>& scheduler)
    {
        return std::make_shared<MasterScan>(task, scheduler);
    }

    // Request that the scan be performed as soon as possible
    void Demand() final;

private:
    const std::shared_ptr<IMasterTask> task;
    const std::shared_ptr<IMasterScheduler> scheduler;
};

} // namespace opendnp3

#endif
