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

#ifndef OPENDNP3_ASIOTESTS_MOCKTLSSERVER_H
#define OPENDNP3_ASIOTESTS_MOCKTLSSERVER_H

#include "channel/IAsyncChannel.h"
#include "channel/tls/TLSServer.h"
#include "channel/tls/TLSStreamChannel.h"

#include <deque>

class MockTLSServer final : public opendnp3::TLSServer
{

public:
    MockTLSServer(const opendnp3::Logger& logger,
                  std::shared_ptr<exe4cpp::StrandExecutor> executor,
                  opendnp3::IPEndpoint endpoint,
                  const opendnp3::TLSConfig& config,
                  std::error_code& ec)
        : TLSServer(logger, executor, endpoint, config, ec)
    {
    }

    static std::shared_ptr<MockTLSServer> Create(const opendnp3::Logger& logger,
                                                 std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                                 opendnp3::IPEndpoint endpoint,
                                                 const opendnp3::TLSConfig& config,
                                                 std::error_code& ec)
    {
        auto server = std::make_shared<MockTLSServer>(logger, executor, endpoint, config, ec);

        if (ec)
            return nullptr;

        server->StartAccept();

        return server;
    }

    bool AcceptConnection(uint64_t sessionid, const asio::ip::tcp::endpoint& remote) override
    {
        return true;
    }

    bool VerifyCallback(uint64_t sessionid, bool preverified, asio::ssl::verify_context& ctx) override
    {
        return preverified;
    }

    void AcceptStream(uint64_t sessionid,
                      const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                      std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream) override
    {
        channels.push_back(opendnp3::TLSStreamChannel::Create(executor, stream));
    }

    void OnShutdown() override {}

    ~MockTLSServer()
    {
        for (auto& channel : channels)
        {
            channel->Shutdown();
        }
    }

    std::deque<std::shared_ptr<opendnp3::IAsyncChannel>> channels;
};

#endif
