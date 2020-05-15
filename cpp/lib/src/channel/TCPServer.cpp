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

#include "channel/TCPServer.h"

#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <sstream>

namespace opendnp3
{

TCPServer::TCPServer(const Logger& logger,
                     const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                     const IPEndpoint& endpoint,
                     std::error_code& ec)
    : logger(logger),
      executor(executor),
      endpoint(asio::ip::tcp::v4(), endpoint.port),
      acceptor(*executor->get_context()),
      socket(*executor->get_context())
{
    this->Configure(endpoint.address, ec);
}

void TCPServer::Shutdown()
{
    if (this->isShutdown)
        return;

    this->isShutdown = true;

    std::error_code ec;
    this->acceptor.close(ec);

    if (ec)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, ec.message().c_str());
    }
}

void TCPServer::Configure(const std::string& adapter, std::error_code& ec)
{
    auto address = asio::ip::address::from_string(adapter, ec);

    if (ec)
    {
        return;
    }

    endpoint.address(address);
    acceptor.open(this->endpoint.protocol(), ec);

    if (ec)
    {
        return;
    }

    acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true), ec);

    if (ec)
    {
        return;
    }

    acceptor.bind(this->endpoint, ec);

    if (ec)
    {
        return;
    }

    acceptor.listen(asio::socket_base::max_connections, ec);

    if (!ec)
    {
        std::ostringstream oss;
        oss << this->endpoint;
        FORMAT_LOG_BLOCK(this->logger, flags::INFO, "Listening on: %s", oss.str().c_str());
    }
}

void TCPServer::StartAccept()
{
    // this ensures that the TCPListener is never deleted during an active callback
    auto callback = [self = shared_from_this()](std::error_code ec) {
        if (ec)
        {
            SIMPLE_LOG_BLOCK(self->logger, flags::INFO, ec.message().c_str());
            self->OnShutdown();
        }
        else
        {
            // With epoll, even if the acceptor was closed, if a socket was accepted
            // and put in ASIO handler queue, it will survive up to here.
            // So we need to make sure we are still alive before really accepting the connection.
            if (self->isShutdown)
            {
                return;
            }

            // For an unknown reason, the socket may not be properly opened when accepted.
            // We simply ignore it.
            if (!self->socket.is_open())
            {
                self->StartAccept();
                return;
            }

            const auto ID = self->session_id;
            ++self->session_id;

            FORMAT_LOG_BLOCK(self->logger, flags::INFO, "Accepted connection from: %s",
                             self->remote_endpoint.address().to_string().c_str());

            // method responsible for closing
            self->AcceptConnection(ID, self->executor, std::move(self->socket));
            self->StartAccept();
        }
    };

    this->acceptor.async_accept(socket, remote_endpoint, this->executor->wrap(callback));
}

} // namespace opendnp3
