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
#include <opendnp3/LogLevels.h>
#include <opendnp3/master/MasterContext.h>
#include <opendnp3/master/MasterSchedulerBackend.h>

#include <asiodnp3/DefaultMasterApplication.h>

#include <dnp3mocks/MockLowerLayer.h>
#include <dnp3mocks/MockMasterApplication.h>
#include <dnp3mocks/MockSOEHandler.h>

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

class MasterTestObject
{
public:
    MasterTestObject(const opendnp3::MasterParams& params)
        : addresses(),
          log(),
          exe(std::make_shared<testlib::MockExecutor>()),
          meas(std::make_shared<opendnp3::MockSOEHandler>()),
          lower(std::make_shared<opendnp3::MockLowerLayer>()),
          application(std::make_shared<opendnp3::MockMasterApplication>()),
          scheduler(std::make_shared<opendnp3::MasterSchedulerBackend>(exe)),
          context(addresses, log.logger, exe, lower, meas, application, this->scheduler, params)
    {
        lower->SetUpperLayer(context);
    }

    ~MasterTestObject()
    {
        this->scheduler->Shutdown();
    }

    size_t SendToMaster(const openpal::RSlice& buffer)
    {
        context.OnReceive(opendnp3::Message(this->addresses.Reverse(), buffer));
        return exe->RunMany();
    }

    size_t LowerLayerUp()
    {
        context.OnLowerLayerUp();
        return exe->RunMany();
    }

private:
    const opendnp3::Addresses addresses;

    testlib::MockLogHandler log;
    const std::shared_ptr<testlib::MockExecutor> exe;
    const std::shared_ptr<opendnp3::MockSOEHandler> meas;
    const std::shared_ptr<opendnp3::MockLowerLayer> lower;
    const std::shared_ptr<opendnp3::MockMasterApplication> application;
    const std::shared_ptr<opendnp3::IMasterScheduler> scheduler;
    opendnp3::MContext context;
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size)
{
    openpal::RSlice buffer(Data, Size);

    opendnp3::MasterParams config;
    MasterTestObject t(config);
    t.LowerLayerUp();
    t.SendToMaster(buffer);
    return 0;
}
