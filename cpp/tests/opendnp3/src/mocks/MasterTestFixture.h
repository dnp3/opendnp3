/*
 * Copyright 2013-2019 Automatak, LLC
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
#ifndef OPENDNP3_MASTERTESTFIXTURE_H
#define OPENDNP3_MASTERTESTFIXTURE_H

#include <opendnp3/LogLevels.h>
#include <opendnp3/master/MasterContext.h>
#include <opendnp3/master/MasterSchedulerBackend.h>

#include <dnp3mocks/MockLowerLayer.h>
#include <dnp3mocks/MockMasterApplication.h>
#include <dnp3mocks/MockSOEHandler.h>

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

namespace opendnp3
{

MasterParams NoStartupTasks();

class MasterTestFixture
{
public:
    MasterTestFixture(const MasterParams& param,
                      const Addresses& = Addresses(1, 1024),
                      const std::string& id = "test",
                      const std::shared_ptr<openpal::ILogHandler>& log = nullptr,
                      const std::shared_ptr<testlib::MockExecutor>& executor = nullptr,
                      const std::shared_ptr<IMasterScheduler>& scheduler = nullptr);
    virtual ~MasterTestFixture();

    bool SendToMaster(const std::string& hex);

    const Addresses addresses;

    const std::shared_ptr<openpal::ILogHandler> log;
    const std::shared_ptr<testlib::MockExecutor> exe;
    const std::shared_ptr<MockSOEHandler> meas;
    const std::shared_ptr<MockLowerLayer> lower;
    const std::shared_ptr<MockMasterApplication> application;
    const std::shared_ptr<IMasterScheduler> scheduler;
    const std::shared_ptr<MContext> context;
};

} // namespace opendnp3

#endif
