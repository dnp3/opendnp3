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

#ifndef OPENDNP3_ASIOTESTS_MOCKTCPSERVER_H
#define OPENDNP3_ASIOTESTS_MOCKTCPSERVER_H

#include "channel/IAsyncChannel.h"
#include "channel/TCPServer.h"
#include "channel/TCPSocketChannel.h"

#include <deque>

class MockTCPServer final : public opendnp3::TCPServer
{

public:
    MockTCPServer(const opendnp3::Logger& logger,
                  std::shared_ptr<exe4cpp::StrandExecutor> executor,
                  opendnp3::IPEndpoint endpoint,
                  std::error_code& ec)
        : TCPServer(logger, executor, endpoint, ec)
    {
    }

    static std::shared_ptr<MockTCPServer> Create(const opendnp3::Logger& logger,
                                                 std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                                 opendnp3::IPEndpoint endpoint,
                                                 std::error_code& ec)
    {
        auto server = std::make_shared<MockTCPServer>(logger, executor, endpoint, ec);

        if (!ec)
        {
            server->StartAccept();
        }

        return server;
    }

    virtual void AcceptConnection(uint64_t sessionid,
                                  const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                  asio::ip::tcp::socket socket) override
    {
        this->channels.push_back(opendnp3::TCPSocketChannel::Create(executor, std::move(socket)));
    }

    virtual void OnShutdown() override {}

    ~MockTCPServer()
    {
        for (auto& channel : channels)
        {
            channel->Shutdown();
        }
    }

    std::deque<std::shared_ptr<opendnp3::IAsyncChannel>> channels;
};

#endif
