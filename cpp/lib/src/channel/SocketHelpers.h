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

#ifndef OPENDNP3_SOCKET_HELPERS_H
#define OPENDNP3_SOCKET_HELPERS_H

#include "opendnp3/util/StaticOnly.h"

#include <asio.hpp>

#include <system_error>

namespace opendnp3
{

class SocketHelpers : private StaticOnly
{

public:
    /**
     * Bind a socket object to a local endpoint given an address. If the address is empty, 0.0.0.0 is used
     */
    template<typename proto_t, typename socket_t>
    static void BindToLocalAddress(const std::string& address, uint16_t port, socket_t& socket, std::error_code& ec)
    {
        typename proto_t::endpoint endpoint;
        auto string = address.empty() ? "0.0.0.0" : address;
        auto addr = asio::ip::address::from_string(string, ec);
        if (!ec)
        {
            endpoint.address(addr);
            endpoint.port(port);
            socket.open(proto_t::v4(), ec);
            if (!ec)
            {
                socket.bind(endpoint, ec);
            }
        }
    }
};

} // namespace opendnp3

#endif
