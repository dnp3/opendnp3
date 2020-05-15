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
#ifndef OPENDNP3_MASTERSTACK_H
#define OPENDNP3_MASTERSTACK_H

#include "StackBase.h"
#include "master/MasterContext.h"
#include "master/MasterScan.h"

#include "opendnp3/master/IMaster.h"
#include "opendnp3/master/MasterStackConfig.h"

namespace opendnp3
{

class MasterStack : public IMaster,
                    public ILinkSession,
                    public ILinkTx,
                    public std::enable_shared_from_this<MasterStack>,
                    public StackBase
{
public:
    MasterStack(const Logger& logger,
                const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                const std::shared_ptr<ISOEHandler>& SOEHandler,
                const std::shared_ptr<IMasterApplication>& application,
                const std::shared_ptr<IMasterScheduler>& scheduler,
                const std::shared_ptr<IOHandler>& iohandler,
                const std::shared_ptr<IResourceManager>& manager,
                const MasterStackConfig& config);

    static std::shared_ptr<MasterStack> Create(const Logger& logger,
                                               const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                               const std::shared_ptr<ISOEHandler>& SOEHandler,
                                               const std::shared_ptr<IMasterApplication>& application,
                                               const std::shared_ptr<IMasterScheduler>& scheduler,
                                               const std::shared_ptr<IOHandler>& iohandler,
                                               const std::shared_ptr<IResourceManager>& manager,
                                               const MasterStackConfig& config)
    {
        auto ret = std::make_shared<MasterStack>(logger, executor, SOEHandler, application, scheduler, iohandler,
                                                 manager, config);

        ret->tstack.link->SetRouter(*ret);

        return ret;
    }

    // --------- Implement IStack ---------

    bool Enable() override;

    bool Disable() override;

    void Shutdown() override;

    StackStatistics GetStackStatistics() override;

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

    bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata) override
    {
        return this->tstack.link->OnFrame(header, userdata);
    }

    void BeginTransmit(const ser4cpp::rseq_t& buffer, ILinkSession& context) override
    {
        this->iohandler->BeginTransmit(shared_from_this(), buffer);
    }

    // --------- Implement IMasterOperations ---------

    void SetLogFilters(const opendnp3::LogLevels& filters) override;

    std::shared_ptr<IMasterScan> AddScan(TimeDuration period,
                                         const std::vector<Header>& headers,
                                         std::shared_ptr<ISOEHandler> soe_handler,
                                         const TaskConfig& config) override;

    std::shared_ptr<IMasterScan> AddAllObjectsScan(GroupVariationID gvId,
                                                   TimeDuration period,
                                                   std::shared_ptr<ISOEHandler> soe_handler,
                                                   const TaskConfig& config) override;

    std::shared_ptr<IMasterScan> AddClassScan(const ClassField& field,
                                              TimeDuration period,
                                              std::shared_ptr<ISOEHandler> soe_handler,
                                              const TaskConfig& config) override;

    std::shared_ptr<IMasterScan> AddRangeScan(GroupVariationID gvId,
                                              uint16_t start,
                                              uint16_t stop,
                                              TimeDuration period,
                                              std::shared_ptr<ISOEHandler> soe_handler,
                                              const TaskConfig& config) override;

    void Scan(const std::vector<Header>& headers,
              std::shared_ptr<ISOEHandler> soe_handler,
              const TaskConfig& config) override;

    void ScanAllObjects(GroupVariationID gvId,
                        std::shared_ptr<ISOEHandler> soe_handler,
                        const TaskConfig& config) override;

    void ScanClasses(const ClassField& field,
                     std::shared_ptr<ISOEHandler> soe_handler,
                     const TaskConfig& config) override;

    void ScanRange(GroupVariationID gvId,
                   uint16_t start,
                   uint16_t stop,
                   std::shared_ptr<ISOEHandler> soe_handler,
                   const TaskConfig& config) override;

    void Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config) override;

    void Restart(RestartType op, const RestartOperationCallbackT& callback, TaskConfig config) override;

    void PerformFunction(const std::string& name,
                         FunctionCode func,
                         const std::vector<Header>& headers,
                         const TaskConfig& config) override;

    // ------- implement ICommandProcessor ---------

    void SelectAndOperate(CommandSet&& commands,
                          const CommandResultCallbackT& callback,
                          const TaskConfig& config) override;

    void DirectOperate(CommandSet&& commands,
                       const CommandResultCallbackT& callback,
                       const TaskConfig& config) override;

protected:
    MContext mcontext;
};

} // namespace opendnp3

#endif
