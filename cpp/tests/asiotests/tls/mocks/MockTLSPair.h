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

#ifndef OPENDNP3_ASIOTESTS_MOCKTLSPAIR_H
#define OPENDNP3_ASIOTESTS_MOCKTLSPAIR_H

#include "channel/tls/TLSClient.h"
#include "mocks/MockIO.h"
#include "tls/mocks/MockTLSClientHandler.h"
#include "tls/mocks/MockTLSServer.h"

#include <dnp3mocks/MockLogHandler.h>

class MockTLSPair
{

public:
    MockTLSPair(const std::shared_ptr<MockIO>& io,
                uint16_t port,
                const opendnp3::TLSConfig& client,
                const opendnp3::TLSConfig& server,
                std::error_code ec = std::error_code());

    ~MockTLSPair();

    void Connect(size_t num = 1);

    bool NumConnectionsEqual(size_t num) const;

    MockLogHandler log;

private:
    std::shared_ptr<MockIO> io;
    uint16_t port;
    std::shared_ptr<MockTLSClientHandler> chandler;
    std::shared_ptr<opendnp3::TLSClient> client;
    std::shared_ptr<MockTLSServer> server;
};

#endif
