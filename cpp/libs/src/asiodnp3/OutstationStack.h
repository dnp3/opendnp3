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
#ifndef ASIODNP3_OUTSTATIONSTACK_H
#define ASIODNP3_OUTSTATIONSTACK_H

#include "asiopal/Executor.h"

#include "opendnp3/outstation/OutstationContext.h"
#include "opendnp3/transport/TransportStack.h"

#include "asiodnp3/IOHandler.h"
#include "asiodnp3/IOutstation.h"
#include "asiodnp3/OutstationStackConfig.h"
#include "asiodnp3/StackBase.h"

namespace asiodnp3
{

/**
 * A stack object for an outstation
 */
class OutstationStack final : public IOutstation,
                              public opendnp3::ILinkSession,
                              public opendnp3::ILinkTx,
                              public std::enable_shared_from_this<OutstationStack>,
                              public StackBase
{
public:
    OutstationStack(const openpal::Logger& logger,
                    const std::shared_ptr<asiopal::Executor>& executor,
                    const std::shared_ptr<opendnp3::ICommandHandler>& commandHandler,
                    const std::shared_ptr<opendnp3::IOutstationApplication>& application,
                    const std::shared_ptr<IOHandler>& iohandler,
                    const std::shared_ptr<asiopal::IResourceManager>& manager,
                    const OutstationStackConfig& config);

    static std::shared_ptr<OutstationStack> Create(const openpal::Logger& logger,
                                                   const std::shared_ptr<asiopal::Executor>& executor,
                                                   const std::shared_ptr<opendnp3::ICommandHandler>& commandHandler,
                                                   const std::shared_ptr<opendnp3::IOutstationApplication>& application,
                                                   const std::shared_ptr<IOHandler>& iohandler,
                                                   const std::shared_ptr<asiopal::IResourceManager>& manager,
                                                   const OutstationStackConfig& config)
    {
        auto ret = std::make_shared<OutstationStack>(logger, executor, commandHandler, application, iohandler, manager,
                                                     config);

        ret->tstack.link->SetRouter(*ret);

        return ret;
    }

    // --------- Implement IStack ---------

    bool Enable() override;

    bool Disable() override;

    void Shutdown() override;

    opendnp3::StackStatistics GetStackStatistics() override;

    // --------- Implement ILinkSession ---------

    bool OnTxReady() override
    {
        return this->tstack.link->OnTxReady();
    }

    bool OnLowerLayerUp() override
    {
        return this->tstack.link->OnLowerLayerUp();
    }

    bool OnLowerLayerDown() override
    {
        return this->tstack.link->OnLowerLayerDown();
    }

    bool OnFrame(const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata) override
    {
        return this->tstack.link->OnFrame(header, userdata);
    }

    void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& context) override
    {
        this->iohandler->BeginTransmit(shared_from_this(), buffer);
    }

    // --------- Implement IOutstation ---------

    void SetLogFilters(const openpal::LogFilters& filters) override;

    void SetRestartIIN() override;

    void Apply(const Updates& updates) override;

private:
    opendnp3::OContext ocontext;
};

} // namespace asiodnp3

#endif
