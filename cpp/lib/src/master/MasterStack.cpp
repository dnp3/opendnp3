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
#include "master/MasterStack.h"

#include "app/APDUBuilders.h"
#include "master/HeaderConversions.h"
#include "master/MasterContext.h"

namespace opendnp3
{

MasterStack::MasterStack(const log4cpp::Logger& logger,
                         const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                         const std::shared_ptr<ISOEHandler>& SOEHandler,
                         const std::shared_ptr<IMasterApplication>& application,
                         const std::shared_ptr<IMasterScheduler>& scheduler,
                         const std::shared_ptr<IOHandler>& iohandler,
                         const std::shared_ptr<IResourceManager>& manager,
                         const MasterStackConfig& config)
    : StackBase(logger,
                executor,
                application,
                iohandler,
                manager,
                config.master.maxRxFragSize,
                LinkLayerConfig(config.link, false)),
      mcontext(Addresses(config.link.LocalAddr, config.link.RemoteAddr),
               logger,
               executor,
               tstack.transport,
               SOEHandler,
               application,
               scheduler,
               config.master)
{
    tstack.transport->SetAppLayer(mcontext);
}

bool MasterStack::Enable()
{
    auto action = [self = shared_from_this()] { return self->iohandler->Enable(self); };
    return this->executor->return_from<bool>(action);
}

bool MasterStack::Disable()
{
    auto action = [self = shared_from_this()] { return self->iohandler->Disable(self); };
    return this->executor->return_from<bool>(action);
}

void MasterStack::Shutdown()
{
    this->PerformShutdown(shared_from_this());
}

StackStatistics MasterStack::GetStackStatistics()
{
    auto get = [self = shared_from_this()]() -> StackStatistics { return self->CreateStatistics(); };
    return this->executor->return_from<StackStatistics>(get);
}

void MasterStack::SetLogFilters(const log4cpp::LogLevels& filters)
{
    auto set = [self = this->shared_from_this(), filters]() { self->logger.set_levels(filters); };

    this->executor->post(set);
}

std::shared_ptr<IMasterScan> MasterStack::AddScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                  TimeDuration period,
                                                  const std::vector<Header>& headers,
                                                  const TaskConfig& config)
{
    auto builder = ConvertToLambda(headers);
    auto self = shared_from_this();
    auto add = [self, soe_handler, builder, period, config]() { return self->mcontext.AddScan(std::move(soe_handler), period, builder, config); };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(add), mcontext.scheduler);
}

std::shared_ptr<IMasterScan> MasterStack::AddAllObjectsScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                            GroupVariationID gvId,
                                                            TimeDuration period,
                                                            const TaskConfig& config)
{
    auto self = shared_from_this();
    auto add = [self, soe_handler, gvId, period, config]() { return self->mcontext.AddAllObjectsScan(std::move(soe_handler), gvId, period, config); };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(add), mcontext.scheduler);
}

std::shared_ptr<IMasterScan> MasterStack::AddClassScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                       const ClassField& field,
                                                       TimeDuration period,
                                                       const TaskConfig& config)
{
    auto self = shared_from_this();
    auto add = [self, soe_handler, field, period, config]() { return self->mcontext.AddClassScan(std::move(soe_handler), field, period, config); };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(add), mcontext.scheduler);
}

std::shared_ptr<IMasterScan> MasterStack::AddRangeScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                       GroupVariationID gvId,
                                                       uint16_t start,
                                                       uint16_t stop,
                                                       TimeDuration period,
                                                       const TaskConfig& config)
{
    auto add = [self = this->shared_from_this(), soe_handler, gvId, start, stop, period, config]() {
        return self->mcontext.AddRangeScan(std::move(soe_handler), gvId, start, stop, period, config);
    };
    return MasterScan::Create(executor->return_from<std::shared_ptr<IMasterTask>>(add), mcontext.scheduler);
}

void MasterStack::Scan(std::shared_ptr<ISOEHandler> soe_handler,
                       const std::vector<Header>& headers,
                       const TaskConfig& config)
{
    auto add = [self = this->shared_from_this(), soe_handler, builder = ConvertToLambda(headers), config]() {
        return self->mcontext.Scan(std::move(soe_handler), builder, config);
    };
    return this->executor->post(add);
}

void MasterStack::ScanAllObjects(std::shared_ptr<ISOEHandler> soe_handler,
                                 GroupVariationID gvId,
                                 const TaskConfig& config)
{
    auto add
        = [self = this->shared_from_this(), soe_handler, gvId, config]() { return self->mcontext.ScanAllObjects(std::move(soe_handler), gvId, config); };
    return this->executor->post(add);
}

void MasterStack::ScanClasses(std::shared_ptr<ISOEHandler> soe_handler,
                              const ClassField& field,
                              const TaskConfig& config)
{
    auto add = [self = this->shared_from_this(), soe_handler, field, config]() { return self->mcontext.ScanClasses(std::move(soe_handler), field, config); };
    return this->executor->post(add);
}

void MasterStack::ScanRange(std::shared_ptr<ISOEHandler> soe_handler,
                            GroupVariationID gvId,
                            uint16_t start,
                            uint16_t stop,
                            const TaskConfig& config)
{
    auto add = [self = this->shared_from_this(), soe_handler, gvId, start, stop, config]() {
        return self->mcontext.ScanRange(std::move(soe_handler), gvId, start, stop, config);
    };
    return this->executor->post(add);
}

void MasterStack::Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config)
{
    auto add = [self = this->shared_from_this(), value, index, config]() {
        return self->mcontext.Write(value, index, config);
    };
    return this->executor->post(add);
}

void MasterStack::Restart(RestartType op, const RestartOperationCallbackT& callback, TaskConfig config)
{
    auto add = [self = this->shared_from_this(), op, callback, config]() {
        return self->mcontext.Restart(op, callback, config);
    };
    return this->executor->post(add);
}

void MasterStack::PerformFunction(const std::string& name,
                                  FunctionCode func,
                                  const std::vector<Header>& headers,
                                  const TaskConfig& config)
{
    auto add = [self = this->shared_from_this(), name, func, builder = ConvertToLambda(headers), config]() {
        return self->mcontext.PerformFunction(name, func, builder, config);
    };
    return this->executor->post(add);
}

void MasterStack::SelectAndOperate(CommandSet&& commands,
                                   const CommandResultCallbackT& callback,
                                   const TaskConfig& config)
{
    /// this is to work around the fact that c++11 doesn't have generic move capture
    auto set = std::make_shared<CommandSet>(std::move(commands));

    auto action = [self = this->shared_from_this(), set, config, callback]() {
        self->mcontext.SelectAndOperate(std::move(*set), callback, config);
    };

    this->executor->post(action);
}

void MasterStack::DirectOperate(CommandSet&& commands, const CommandResultCallbackT& callback, const TaskConfig& config)
{
    /// this is to work around the fact that c++11 doesn't have generic move capture
    auto set = std::make_shared<CommandSet>(std::move(commands));

    auto action = [self = this->shared_from_this(), set, config, callback]() {
        self->mcontext.DirectOperate(std::move(*set), callback, config);
    };

    this->executor->post(action);
}

} // namespace opendnp3
