/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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

#include "MockTCPPair.h"

using namespace opendnp3;

MockTCPPair::MockTCPPair(std::shared_ptr<MockIO> io, uint16_t port, std::error_code ec)
    : log(),
      io(io),
      port(port),
      chandler(std::make_shared<MockTCPClientHandler>()),
      client(TCPClient::Create(log.logger, io->GetExecutor(), "127.0.0.1")),
      server(MockTCPServer::Create(log.logger, io->GetExecutor(), IPEndpoint::Localhost(20000), ec))
{
    if (ec)
    {
        throw std::logic_error(ec.message());
    }
}

MockTCPPair::~MockTCPPair()
{
    this->server->Shutdown();
    this->client->Cancel();
}

void MockTCPPair::Connect(size_t num)
{
    auto callback = [handler = this->chandler](const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                               asio::ip::tcp::socket socket, const std::error_code& ec) {
        handler->OnConnect(executor, std::move(socket), ec);
    };

    if (!this->client->BeginConnect(IPEndpoint::Localhost(this->port), callback))
    {
        throw std::logic_error("BeginConnect returned false");
    }

    auto connected = [this, num]() -> bool { return this->NumConnectionsEqual(num); };

    io->CompleteInMaxXIterations(2, connected);
}

bool MockTCPPair::NumConnectionsEqual(size_t num) const
{
    return (this->server->channels.size() == num) && (this->chandler->channels.size() == num);
}
