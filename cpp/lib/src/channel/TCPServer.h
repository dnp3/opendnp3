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
#ifndef OPENDNP3_TCPSERVER_H
#define OPENDNP3_TCPSERVER_H

#include "opendnp3/channel/IListener.h"
#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/util/Uncopyable.h"

#include <exe4cpp/asio/StrandExecutor.h>

#include <memory>

namespace opendnp3
{

/**
 * Binds and listens on an IPv4 TCP port
 *
 * Meant to be used exclusively as a shared_ptr
 */
class TCPServer : public std::enable_shared_from_this<TCPServer>, public IListener, private Uncopyable
{

public:
    TCPServer(const Logger& logger,
              const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
              const IPEndpoint& endpoint,
              std::error_code& ec);

    /// Implement IListener
    void Shutdown() override final;

protected:
    /// Inherited classes must define these functions

    virtual void OnShutdown() = 0;

    virtual void AcceptConnection(uint64_t sessionid,
                                  const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                  asio::ip::tcp::socket)
        = 0;

    /// Start asynchronously accepting connections on the strand
    void StartAccept();

    Logger logger;
    std::shared_ptr<exe4cpp::StrandExecutor> executor;

private:
    void Configure(const std::string& adapter, std::error_code& ec);

    asio::ip::tcp::endpoint endpoint;
    asio::ip::tcp::acceptor acceptor;
    asio::ip::tcp::socket socket;
    asio::ip::tcp::endpoint remote_endpoint;
    bool isShutdown = false;
    uint64_t session_id = 0;
};

} // namespace opendnp3

#endif
