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
#ifndef OPENDNP3_OUTSTATIONSTACK_H
#define OPENDNP3_OUTSTATIONSTACK_H

#include "StackBase.h"
#include "channel/IOHandler.h"
#include "outstation/OutstationContext.h"
#include "transport/TransportStack.h"

#include "opendnp3/outstation/IOutstation.h"
#include "opendnp3/outstation/OutstationStackConfig.h"

#include <exe4cpp/IExecutor.h>

namespace opendnp3
{

/**
 * A stack object for an outstation
 */
class OutstationStack final : public IOutstation,
                              public ILinkSession,
                              public ILinkTx,
                              public std::enable_shared_from_this<OutstationStack>,
                              public StackBase
{
public:
    OutstationStack(const Logger& logger,
                    const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                    const std::shared_ptr<ICommandHandler>& commandHandler,
                    const std::shared_ptr<IOutstationApplication>& application,
                    const std::shared_ptr<IOHandler>& iohandler,
                    const std::shared_ptr<IResourceManager>& manager,
                    const OutstationStackConfig& config);

    static std::shared_ptr<OutstationStack> Create(const Logger& logger,
                                                   const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                   const std::shared_ptr<ICommandHandler>& commandHandler,
                                                   const std::shared_ptr<IOutstationApplication>& application,
                                                   const std::shared_ptr<IOHandler>& iohandler,
                                                   const std::shared_ptr<IResourceManager>& manager,
                                                   const OutstationStackConfig& config)
    {
        auto ret = std::make_shared<OutstationStack>(logger, executor, commandHandler, application, iohandler, manager,
                                                     config);

        ret->tstack.link->SetRouter(*ret);

        return ret;
    }

    // --------- Implement IStack ---------

    bool Enable() final;

    bool Disable() final;

    void Shutdown() final;

    StackStatistics GetStackStatistics() final;

    // --------- Implement ILinkSession ---------

    bool OnTxReady() final
    {
        return this->tstack.link->OnTxReady();
    }

    bool OnLowerLayerUp() final
    {
        return this->tstack.link->OnLowerLayerUp();
    }

    bool OnLowerLayerDown() final
    {
        return this->tstack.link->OnLowerLayerDown();
    }

    bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata) final
    {
        return this->tstack.link->OnFrame(header, userdata);
    }

    void BeginTransmit(const ser4cpp::rseq_t& buffer, ILinkSession& context) final
    {
        this->iohandler->BeginTransmit(shared_from_this(), buffer);
    }

    // --------- Implement IOutstation ---------

    void SetLogFilters(const opendnp3::LogLevels& filters) final;

    void SetRestartIIN() final;

    void Apply(const Updates& updates) final;

private:
    OContext ocontext;
};

} // namespace opendnp3

#endif
