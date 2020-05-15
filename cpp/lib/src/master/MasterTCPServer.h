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
#ifndef OPENDNP3_MASTERTCPSERVER_H
#define OPENDNP3_MASTERTCPSERVER_H

#include "ResourceManager.h"
#include "channel/TCPServer.h"

#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/IListenCallbacks.h"

namespace opendnp3
{
/**
 * Binds and listens on an IPv4 TCP port
 *
 * Meant to be used exclusively as a shared_ptr
 */
class MasterTCPServer final : public TCPServer
{

public:
    MasterTCPServer(const Logger& logger,
                    const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                    const IPEndpoint& endpoint,
                    std::shared_ptr<IListenCallbacks> callbacks,
                    std::shared_ptr<ResourceManager> manager,
                    std::error_code& ec);

    static std::shared_ptr<MasterTCPServer> Create(const Logger& logger,
                                                   const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                   const IPEndpoint& endpoint,
                                                   const std::shared_ptr<IListenCallbacks>& callbacks,
                                                   const std::shared_ptr<ResourceManager>& manager,
                                                   std::error_code& ec)
    {
        auto server = std::make_shared<MasterTCPServer>(logger, executor, endpoint, callbacks, manager, ec);

        if (!ec)
        {
            server->StartAccept();
        }

        return server;
    }

private:
    std::shared_ptr<IListenCallbacks> callbacks;
    std::shared_ptr<ResourceManager> manager;

    static std::string SessionIdToString(uint64_t sessionid);

    // implement the virutal methods from TCPServer

    virtual void OnShutdown() override;

    virtual void AcceptConnection(uint64_t sessionid,
                                  const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                  asio::ip::tcp::socket) override;
};

} // namespace opendnp3

#endif
