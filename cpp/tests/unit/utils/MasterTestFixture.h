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
#ifndef OPENDNP3_UNITTESTS_MASTERTESTFIXTURE_H
#define OPENDNP3_UNITTESTS_MASTERTESTFIXTURE_H

#include <opendnp3/logging/LogLevels.h>

#include <exe4cpp/MockExecutor.h>

#include "dnp3mocks/MockLogHandler.h"
#include "dnp3mocks/MockLowerLayer.h"
#include "dnp3mocks/MockMasterApplication.h"
#include "dnp3mocks/MockSOEHandler.h"

#include <master/MasterContext.h>
#include <master/MasterSchedulerBackend.h>

opendnp3::MasterParams NoStartupTasks();

class MasterTestFixture
{
public:
    MasterTestFixture(const opendnp3::MasterParams& param,
                      const opendnp3::Addresses& = opendnp3::Addresses(1, 1024),
                      const std::string& id = "test",
                      const std::shared_ptr<opendnp3::ILogHandler>& log = nullptr,
                      const std::shared_ptr<exe4cpp::MockExecutor>& executor = nullptr,
                      const std::shared_ptr<opendnp3::IMasterScheduler>& scheduler = nullptr);
    virtual ~MasterTestFixture();

    bool SendToMaster(const std::string& hex);

    const opendnp3::Addresses addresses;

    const std::shared_ptr<opendnp3::ILogHandler> log;
    const std::shared_ptr<exe4cpp::MockExecutor> exe;
    const std::shared_ptr<MockSOEHandler> meas;
    const std::shared_ptr<MockLowerLayer> lower;
    const std::shared_ptr<MockMasterApplication> application;
    const std::shared_ptr<opendnp3::IMasterScheduler> scheduler;
    const std::shared_ptr<opendnp3::MContext> context;
};

#endif
