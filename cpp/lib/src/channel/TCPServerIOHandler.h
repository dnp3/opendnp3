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

#ifndef OPENDNP3_TCPSERVERIOHANDLER_H
#define OPENDNP3_TCPSERVERIOHANDLER_H

#include "channel/IOHandler.h"
#include "channel/TCPServer.h"

#include "opendnp3/channel/ChannelRetry.h"
#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/gen/ServerAcceptMode.h"

#include <exe4cpp/Timer.h>

namespace opendnp3
{

class TCPServerIOHandler final : public IOHandler
{
    class Server final : public TCPServer
    {
    public:
        typedef std::function<void(const std::shared_ptr<exe4cpp::StrandExecutor>& executor, asio::ip::tcp::socket)>
            callback_t;

        Server(const Logger& logger,
               const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
               const IPEndpoint& endpoint,
               std::error_code& ec)
            : TCPServer(logger, executor, endpoint, ec)
        {
        }

        void StartAcceptingConnection(const callback_t& callback)
        {
            this->callback = callback;
            this->StartAccept();
        }

    private:
        callback_t callback;

        void OnShutdown() final {}

        void AcceptConnection(uint64_t sessionid,
                              const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                              asio::ip::tcp::socket) final;
    };

public:
    static std::shared_ptr<TCPServerIOHandler> Create(const Logger& logger,
                                                      ServerAcceptMode accept_mode,
                                                      const std::shared_ptr<IChannelListener>& listener,
                                                      const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                      const IPEndpoint& endpoint,
                                                      std::error_code& ec)
    {
        return std::make_shared<TCPServerIOHandler>(logger, accept_mode, listener, executor, endpoint, ec);
    }

    TCPServerIOHandler(const Logger& logger,
                       ServerAcceptMode accept_mode,
                       const std::shared_ptr<IChannelListener>& listener,
                       std::shared_ptr<exe4cpp::StrandExecutor> executor,
                       IPEndpoint endpoint,
                       std::error_code& ec);

protected:
    void ShutdownImpl() final;
    void BeginChannelAccept() final;
    void SuspendChannelAccept() final;
    void OnChannelShutdown() final {} // do nothing, always accepting new connections

private:
    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    const IPEndpoint endpoint;
    std::shared_ptr<Server> server;
};

} // namespace opendnp3

#endif
