/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "TCPServerIOHandler.h"

#include "channel/TCPSocketChannel.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

void TCPServerIOHandler::Server::AcceptConnection(uint64_t /*sessionid*/,
                                                  const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                  asio::ip::tcp::socket socket)
{
    this->callback(executor, std::move(socket));
}

TCPServerIOHandler::TCPServerIOHandler(const Logger& logger,
                                       ServerAcceptMode mode,
                                       const std::shared_ptr<IChannelListener>& listener,
                                       std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                       IPEndpoint endpoint,
                                       std::error_code& ec)
    : IOHandler(logger, mode == ServerAcceptMode::CloseExisting, listener),
      executor(std::move(executor)),
      endpoint(std::move(endpoint)),
      server(std::make_shared<Server>(this->logger, this->executor, this->endpoint, ec))
{
}

void TCPServerIOHandler::ShutdownImpl()
{
    if (this->server)
    {
        this->server->Shutdown();
        this->server.reset();
    }
}

void TCPServerIOHandler::BeginChannelAccept()
{
    auto callback = [self = shared_from_this(), this](const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                      asio::ip::tcp::socket socket) {
        this->OnNewChannel(TCPSocketChannel::Create(executor, std::move(socket)));
    };

    if (this->server)
    {
        this->server->StartAcceptingConnection(callback);
    }
    else
    {
        std::error_code ec;
        this->server = std::make_shared<Server>(this->logger, this->executor, this->endpoint, ec);

        if (ec)
        {
            SIMPLE_LOG_BLOCK(this->logger, flags::WARN, ec.message().c_str());

            // TODO - should we retry?
        }
        else
        {
            this->server->StartAcceptingConnection(callback);
        }
    }
}

void TCPServerIOHandler::SuspendChannelAccept()
{
    if (this->server)
    {
        this->server->Shutdown();
        this->server.reset();
    }
}

} // namespace opendnp3
