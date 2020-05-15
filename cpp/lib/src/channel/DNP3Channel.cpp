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
#include "channel/DNP3Channel.h"

#include "logging/LogMacros.h"
#include "master/MasterSchedulerBackend.h"
#include "master/MasterStack.h"
#include "outstation/OutstationStack.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

DNP3Channel::DNP3Channel(const Logger& logger,
                         const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                         std::shared_ptr<IOHandler> iohandler,
                         std::shared_ptr<IResourceManager> manager)
    :

      logger(logger),
      executor(executor),
      scheduler(std::make_shared<MasterSchedulerBackend>(executor)),
      iohandler(std::move(iohandler)),
      manager(std::move(manager)),
      resources(ResourceManager::Create())
{
}

DNP3Channel::~DNP3Channel()
{
    this->ShutdownImpl();
}

// comes from the outside, so we need to post
void DNP3Channel::Shutdown()
{
    auto shutdown = [self = shared_from_this()]() { self->ShutdownImpl(); };

    this->executor->block_until_and_flush(shutdown);
}

void DNP3Channel::ShutdownImpl()
{
    if (!this->resources)
        return;

    // shutdown the IO handler
    this->iohandler->Shutdown();
    this->iohandler.reset();

    this->scheduler->Shutdown();
    this->scheduler.reset();

    // shutdown any remaining stacks
    this->resources->Shutdown();
    this->resources.reset();

    // posting ensures that we run this after
    // and callbacks created by calls above
    auto detach = [self = shared_from_this()] {
        self->manager->Detach(self);
        self->manager.reset();
    };

    this->executor->post(detach);
}

LinkStatistics DNP3Channel::GetStatistics()
{
    auto get = [this]() { return this->iohandler->Statistics(); };
    return this->executor->return_from<LinkStatistics>(get);
}

LogLevels DNP3Channel::GetLogFilters() const
{
    auto get = [this]() { return this->logger.get_levels(); };
    return this->executor->return_from<LogLevels>(get);
}

void DNP3Channel::SetLogFilters(const LogLevels& filters)
{
    auto set = [self = this->shared_from_this(), filters]() { self->logger.set_levels(filters); };
    this->executor->post(set);
}

std::shared_ptr<IMaster> DNP3Channel::AddMaster(const std::string& id,
                                                std::shared_ptr<ISOEHandler> SOEHandler,
                                                std::shared_ptr<IMasterApplication> application,
                                                const MasterStackConfig& config)
{
    auto stack = MasterStack::Create(this->logger.detach(id), this->executor, SOEHandler, application, this->scheduler,
                                     this->iohandler, this->resources, config);

    return this->AddStack(config.link, stack);
}

std::shared_ptr<IOutstation> DNP3Channel::AddOutstation(const std::string& id,
                                                        std::shared_ptr<ICommandHandler> commandHandler,
                                                        std::shared_ptr<IOutstationApplication> application,
                                                        const OutstationStackConfig& config)
{
    auto stack = OutstationStack::Create(this->logger.detach(id), this->executor, commandHandler, application,
                                         this->iohandler, this->resources, config);

    return this->AddStack(config.link, stack);
}

template<class T> std::shared_ptr<T> DNP3Channel::AddStack(const LinkConfig& link, const std::shared_ptr<T>& stack)
{

    auto create = [stack, route = Addresses(link.RemoteAddr, link.LocalAddr), self = this->shared_from_this()]() {
        auto add = [stack, route, self]() -> bool { return self->iohandler->AddContext(stack, route); };

        return self->executor->return_from<bool>(add) ? stack : nullptr;
    };

    return this->resources->Bind<T>(create);
}

} // namespace opendnp3
