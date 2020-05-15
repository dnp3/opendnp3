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
#ifndef OPENDNP3_DNP3CHANNEL_H
#define OPENDNP3_DNP3CHANNEL_H

#include "ResourceManager.h"
#include "channel/IOHandler.h"
#include "master/IMasterScheduler.h"

#include "opendnp3/channel/IChannel.h"

namespace opendnp3
{

class DNP3Channel final : public IChannel, public std::enable_shared_from_this<DNP3Channel>
{

public:
    DNP3Channel(const Logger& logger,
                const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                std::shared_ptr<IOHandler> iohandler,
                std::shared_ptr<IResourceManager> manager);

    static std::shared_ptr<DNP3Channel> Create(const Logger& logger,
                                               const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                               const std::shared_ptr<IOHandler>& iohandler,
                                               const std::shared_ptr<IResourceManager>& manager)
    {
        return std::make_shared<DNP3Channel>(logger, executor, iohandler, manager);
    }

    ~DNP3Channel();

    // ----------------------- Implement IChannel -----------------------

    void Shutdown() final;

    LinkStatistics GetStatistics() final;

    opendnp3::LogLevels GetLogFilters() const final;

    void SetLogFilters(const opendnp3::LogLevels& filters) final;

    std::shared_ptr<IMaster> AddMaster(const std::string& id,
                                       std::shared_ptr<ISOEHandler> SOEHandler,
                                       std::shared_ptr<IMasterApplication> application,
                                       const MasterStackConfig& config) final;

    std::shared_ptr<IOutstation> AddOutstation(const std::string& id,
                                               std::shared_ptr<ICommandHandler> commandHandler,
                                               std::shared_ptr<IOutstationApplication> application,
                                               const OutstationStackConfig& config) final;

private:
    void ShutdownImpl();

    // ----- generic method for adding a stack ------
    template<class T> std::shared_ptr<T> AddStack(const LinkConfig& link, const std::shared_ptr<T>& stack);

    Logger logger;
    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    std::shared_ptr<IMasterScheduler> scheduler;

    std::shared_ptr<IOHandler> iohandler;
    std::shared_ptr<IResourceManager> manager;
    std::shared_ptr<ResourceManager> resources;
};

} // namespace opendnp3

#endif
