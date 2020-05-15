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
#ifndef OPENDNP3_MASTERSESSIONSTACK_H
#define OPENDNP3_MASTERSESSIONSTACK_H

#include "master/MasterContext.h"
#include "master/MasterScan.h"
#include "transport/TransportStack.h"

#include "opendnp3/master/IMasterSession.h"
#include "opendnp3/master/MasterStackConfig.h"

#include <exe4cpp/asio/StrandExecutor.h>

namespace opendnp3
{

class LinkSession;

/**
 * Interface that represents an ephemeral master session
 */
class MasterSessionStack final : public IMasterSession, public std::enable_shared_from_this<MasterSessionStack>
{
public:
    static std::shared_ptr<MasterSessionStack> Create(const Logger& logger,
                                                      const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                      const std::shared_ptr<ISOEHandler>& SOEHandler,
                                                      const std::shared_ptr<IMasterApplication>& application,
                                                      const std::shared_ptr<IMasterScheduler>& scheduler,
                                                      const std::shared_ptr<LinkSession>& session,
                                                      ILinkTx& linktx,
                                                      const MasterStackConfig& config);

    void OnLowerLayerUp();

    void OnLowerLayerDown();

    bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata);

    void OnTxReady();

    void SetLogFilters(const opendnp3::LogLevels& filters) final;

    /// --- IGPRSMaster ---

    void BeginShutdown() final;

    /// --- ICommandOperations ---

    StackStatistics GetStackStatistics() final;
    std::shared_ptr<IMasterScan> AddScan(TimeDuration period,
                                         const std::vector<Header>& headers,
                                         std::shared_ptr<ISOEHandler> soe_handler,
                                         const TaskConfig& config) final;
    std::shared_ptr<IMasterScan> AddAllObjectsScan(GroupVariationID gvId,
                                                   TimeDuration period,
                                                   std::shared_ptr<ISOEHandler> soe_handler,
                                                   const TaskConfig& config) final;
    std::shared_ptr<IMasterScan> AddClassScan(const ClassField& field,
                                              TimeDuration period,
                                              std::shared_ptr<ISOEHandler> soe_handler,
                                              const TaskConfig& config) final;
    std::shared_ptr<IMasterScan> AddRangeScan(GroupVariationID gvId,
                                              uint16_t start,
                                              uint16_t stop,
                                              TimeDuration period,
                                              std::shared_ptr<ISOEHandler> soe_handler,
                                              const TaskConfig& config) final;
    void Scan(const std::vector<Header>& headers,
              std::shared_ptr<ISOEHandler> soe_handler,
              const TaskConfig& config) final;
    void ScanAllObjects(GroupVariationID gvId,
                        std::shared_ptr<ISOEHandler> soe_handler,
                        const TaskConfig& config) final;
    void ScanClasses(const ClassField& field, std::shared_ptr<ISOEHandler> soe_handler, const TaskConfig& config) final;
    void ScanRange(GroupVariationID gvId,
                   uint16_t start,
                   uint16_t stop,
                   std::shared_ptr<ISOEHandler> soe_handler,
                   const TaskConfig& config) final;
    void Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config) final;
    void Restart(RestartType op, const RestartOperationCallbackT& callback, TaskConfig config) final;
    void PerformFunction(const std::string& name,
                         FunctionCode func,
                         const std::vector<Header>& headers,
                         const TaskConfig& config) final;

    /// --- ICommandProcessor ---

    void SelectAndOperate(CommandSet&& commands,
                          const CommandResultCallbackT& callback,
                          const TaskConfig& config) final;
    void DirectOperate(CommandSet&& commands, const CommandResultCallbackT& callback, const TaskConfig& config) final;

    MasterSessionStack(const Logger& logger,
                       const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                       const std::shared_ptr<ISOEHandler>& SOEHandler,
                       const std::shared_ptr<IMasterApplication>& application,
                       const std::shared_ptr<IMasterScheduler>& scheduler,
                       std::shared_ptr<LinkSession> session,
                       ILinkTx& linktx,
                       const MasterStackConfig& config);

private:
    StackStatistics CreateStatistics() const;

    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    std::shared_ptr<IMasterScheduler> scheduler;

    std::shared_ptr<LinkSession> session;

    TransportStack stack;
    MContext context;
};

} // namespace opendnp3

#endif
