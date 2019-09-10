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

#include "master/MasterSessionStack.h"

#include "link/LinkSession.h"
#include "master/HeaderConversions.h"

#include <utility>

namespace opendnp3
{
std::shared_ptr<MasterSessionStack> MasterSessionStack::Create(const log4cpp::Logger& logger,
                                                               const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                               const std::shared_ptr<ISOEHandler>& SOEHandler,
                                                               const std::shared_ptr<IMasterApplication>& application,
                                                               const std::shared_ptr<IMasterScheduler>& scheduler,
                                                               const std::shared_ptr<LinkSession>& session,
                                                               ILinkTx& linktx,
                                                               const MasterStackConfig& config)
{
    return std::make_shared<MasterSessionStack>(logger, executor, SOEHandler, application, scheduler, session, linktx,
                                                config);
}

MasterSessionStack::MasterSessionStack(const log4cpp::Logger& logger,
                                       const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                       const std::shared_ptr<ISOEHandler>& SOEHandler,
                                       const std::shared_ptr<IMasterApplication>& application,
                                       const std::shared_ptr<IMasterScheduler>& scheduler,
                                       std::shared_ptr<LinkSession> session,
                                       ILinkTx& linktx,
                                       const MasterStackConfig& config)
    : executor(executor),
      scheduler(scheduler),
      session(std::move(session)),
      stack(logger, executor, application, config.master.maxRxFragSize, LinkLayerConfig(config.link, false)),
      context(Addresses(config.link.LocalAddr, config.link.RemoteAddr),
              logger,
              executor,
              stack.transport,
              SOEHandler,
              application,
              scheduler,
              config.master)
{
    stack.link->SetRouter(linktx);
    stack.transport->SetAppLayer(context);
}

void MasterSessionStack::OnLowerLayerUp()
{
    stack.link->OnLowerLayerUp();
}

void MasterSessionStack::OnLowerLayerDown()
{
    stack.link->OnLowerLayerDown();
}

bool MasterSessionStack::OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata)
{
    return stack.link->OnFrame(header, userdata);
}

void MasterSessionStack::OnTxReady()
{
    this->stack.link->OnTxReady();
}

void MasterSessionStack::SetLogFilters(const log4cpp::LogLevels& filters)
{
    auto set = [this, filters]() { this->session->SetLogFilters(filters); };

    this->executor->post(set);
}

void MasterSessionStack::BeginShutdown()
{
    auto shutdown = [this]() {
        if (scheduler)
        {
            scheduler->Shutdown();
            scheduler.reset();
        }

        // now we can release the socket session
        if (session)
        {
            session->Shutdown();
            session.reset();
        }
    };

    executor->post(shutdown);
}

StackStatistics MasterSessionStack::GetStackStatistics()
{
    auto get = [self = shared_from_this()]() -> StackStatistics { return self->CreateStatistics(); };
    return executor->return_from<StackStatistics>(get);
}

std::shared_ptr<IMasterScan> MasterSessionStack::AddScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                         TimeDuration period,
                                                         const std::vector<Header>& headers,
                                                         const TaskConfig& config)
{
    auto builder = ConvertToLambda(headers);
    auto get = [self = shared_from_this(), soe_handler, period, builder, config]() {
        return self->context.AddScan(std::move(soe_handler), period, builder, config);
    };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(get), this->scheduler);
}

std::shared_ptr<IMasterScan> MasterSessionStack::AddAllObjectsScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                                   GroupVariationID gvId,
                                                                   TimeDuration period,
                                                                   const TaskConfig& config)
{
    auto get = [self = shared_from_this(), soe_handler, gvId, period, config] {
        return self->context.AddAllObjectsScan(std::move(soe_handler), gvId, period, config);
    };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(get), this->scheduler);
}

std::shared_ptr<IMasterScan> MasterSessionStack::AddClassScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                              const ClassField& field,
                                                              TimeDuration period,
                                                              const TaskConfig& config)
{
    auto get = [self = shared_from_this(), soe_handler, field, period, config] {
        return self->context.AddClassScan(std::move(soe_handler), field, period, config);
    };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(get), this->scheduler);
}

std::shared_ptr<IMasterScan> MasterSessionStack::AddRangeScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                              GroupVariationID gvId,
                                                              uint16_t start,
                                                              uint16_t stop,
                                                              TimeDuration period,
                                                              const TaskConfig& config)
{
    auto get = [self = shared_from_this(), soe_handler, gvId, start, stop, period, config] {
        return self->context.AddRangeScan(std::move(soe_handler), gvId, start, stop, period, config);
    };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(get), this->scheduler);
}

void MasterSessionStack::Scan(std::shared_ptr<ISOEHandler> soe_handler,
                              const std::vector<Header>& headers,
                              const TaskConfig& config)
{
    auto builder = ConvertToLambda(headers);
    auto action = [self = shared_from_this(), soe_handler, builder, config]() -> void { self->context.Scan(std::move(soe_handler), builder, config); };
    return executor->post(action);
}

void MasterSessionStack::ScanAllObjects(std::shared_ptr<ISOEHandler> soe_handler,
                                        GroupVariationID gvId,
                                        const TaskConfig& config)
{
    auto action = [self = shared_from_this(), soe_handler, gvId, config]() -> void { self->context.ScanAllObjects(std::move(soe_handler), gvId, config); };
    return executor->post(action);
}

void MasterSessionStack::ScanClasses(std::shared_ptr<ISOEHandler> soe_handler,
                                     const ClassField& field,
                                     const TaskConfig& config)
{
    auto action = [self = shared_from_this(), soe_handler, field, config]() -> void { self->context.ScanClasses(std::move(soe_handler), field, config); };
    return executor->post(action);
}

void MasterSessionStack::ScanRange(std::shared_ptr<ISOEHandler> soe_handler,
                                   GroupVariationID gvId,
                                   uint16_t start,
                                   uint16_t stop,
                                   const TaskConfig& config)
{
    auto action = [self = shared_from_this(), soe_handler, gvId, start, stop, config]() -> void {
        self->context.ScanRange(std::move(soe_handler), gvId, start, stop, config);
    };
    return executor->post(action);
}

void MasterSessionStack::Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config)
{
    auto action
        = [self = shared_from_this(), value, index, config]() -> void { self->context.Write(value, index, config); };
    return executor->post(action);
}

void MasterSessionStack::Restart(RestartType op, const RestartOperationCallbackT& callback, TaskConfig config)
{
    auto action
        = [self = shared_from_this(), op, callback, config]() -> void { self->context.Restart(op, callback, config); };
    return executor->post(action);
}

void MasterSessionStack::PerformFunction(const std::string& name,
                                         FunctionCode func,
                                         const std::vector<Header>& headers,
                                         const TaskConfig& config)
{
    auto builder = ConvertToLambda(headers);
    auto action = [self = shared_from_this(), name, func, builder, config]() -> void {
        self->context.PerformFunction(name, func, builder, config);
    };
    return executor->post(action);
}

/// --- ICommandProcessor ---

void MasterSessionStack::SelectAndOperate(CommandSet&& commands,
                                          const CommandResultCallbackT& callback,
                                          const TaskConfig& config)
{
    // this is required b/c move capture not supported in C++11
    auto set = std::make_shared<CommandSet>(std::move(commands));

    auto action = [self = shared_from_this(), set, config, callback]() -> void {
        self->context.SelectAndOperate(std::move(*set), callback, config);
    };
    executor->post(action);
}

void MasterSessionStack::DirectOperate(CommandSet&& commands,
                                       const CommandResultCallbackT& callback,
                                       const TaskConfig& config)
{
    // this is required b/c move capture not supported in C++11
    auto set = std::make_shared<CommandSet>(std::move(commands));

    auto action = [self = shared_from_this(), set, config, callback]() -> void {
        self->context.DirectOperate(std::move(*set), callback, config);
    };
    executor->post(action);
}

StackStatistics MasterSessionStack::CreateStatistics() const
{
    return StackStatistics(this->stack.link->GetStatistics(), this->stack.transport->GetStatistics());
}

} // namespace opendnp3
