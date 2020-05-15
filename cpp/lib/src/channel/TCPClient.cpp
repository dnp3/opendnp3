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

#include "channel/TCPClient.h"

#include "channel/SocketHelpers.h"

#include <sstream>
#include <utility>

namespace opendnp3
{

TCPClient::TCPClient(const Logger& logger,
                     const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                     std::string adapter)
    : condition(logger),
      executor(executor),
      adapter(std::move(adapter)),
      socket(*executor->get_context()),
      resolver(*executor->get_context())
{
}

bool TCPClient::Cancel()
{
    if (this->canceled || !this->connecting)
    {
        return false;
    }

    std::error_code ec;
    socket.cancel(ec);
    resolver.cancel();
    this->canceled = true;
    return true;
}

bool TCPClient::BeginConnect(const IPEndpoint& remote, const connect_callback_t& callback)
{
    if (connecting || canceled)
        return false;

    this->connecting = true;

    std::error_code ec;
    SocketHelpers::BindToLocalAddress<asio::ip::tcp>(this->adapter, 0, this->socket, ec);

    if (ec)
    {
        return this->PostConnectError(callback, ec);
    }

    const auto address = asio::ip::address::from_string(remote.address, ec);
    auto self = this->shared_from_this();
    if (ec)
    {
        // Try DNS resolution instead
        auto cb = [self, callback](const std::error_code& ec, asio::ip::tcp::resolver::iterator endpoints) {
            self->HandleResolveResult(callback, endpoints, ec);
        };

        std::stringstream portstr;
        portstr << remote.port;

        resolver.async_resolve(asio::ip::tcp::resolver::query(remote.address, portstr.str()), executor->wrap(cb));

        return true;
    }

    asio::ip::tcp::endpoint remoteEndpoint(address, remote.port);
    auto cb = [self, callback](const std::error_code& ec) {
        self->connecting = false;
        if (!self->canceled)
        {
            callback(self->executor, std::move(self->socket), ec);
        }
    };

    socket.async_connect(remoteEndpoint, executor->wrap(cb));
    return true;
}

void TCPClient::HandleResolveResult(const connect_callback_t& callback,
                                    const asio::ip::tcp::resolver::iterator& endpoints,
                                    const std::error_code& ec)
{
    if (ec)
    {
        this->PostConnectError(callback, ec);
    }
    else
    {
        // attempt a connection to each endpoint in the iterator until we connect
        auto cb = [self = shared_from_this(), callback](const std::error_code& ec,
                                                        asio::ip::tcp::resolver::iterator endpoints) {
            self->connecting = false;
            if (!self->canceled)
            {
                callback(self->executor, std::move(self->socket), ec);
            }
        };

        asio::async_connect(this->socket, endpoints, this->condition, this->executor->wrap(cb));
    }
}

bool TCPClient::PostConnectError(const connect_callback_t& callback, const std::error_code& ec)
{
    auto cb = [self = shared_from_this(), ec, callback]() {
        self->connecting = false;
        if (!self->canceled)
        {
            callback(self->executor, std::move(self->socket), ec);
        }
    };
    executor->post(cb);
    return true;
}

} // namespace opendnp3
