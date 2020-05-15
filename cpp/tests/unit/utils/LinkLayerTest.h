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
#ifndef OPENDNP3_UNITTESTS_LINKLAYERTEST_H
#define OPENDNP3_UNITTESTS_LINKLAYERTEST_H

#include <opendnp3/logging/LogLevels.h>

#include <exe4cpp/MockExecutor.h>

#include "dnp3mocks/MockLinkListener.h"
#include "dnp3mocks/MockLogHandler.h"
#include "dnp3mocks/MockTransportLayer.h"

#include <link/ILinkTx.h>
#include <link/LinkLayer.h>

#include <queue>

class LinkLayerTest : public opendnp3::ILinkTx
{
public:
    LinkLayerTest(const opendnp3::LinkConfig& config);

    LinkLayerTest(const opendnp3::LinkLayerConfig& config = DefaultConfig());

    bool OnFrame(opendnp3::LinkFunction func,
                 bool isMaster,
                 bool fcb,
                 bool fcvdfc,
                 uint16_t dest,
                 uint16_t source,
                 const ser4cpp::rseq_t& userdata = ser4cpp::rseq_t::empty());

    // ILinkTx interface
    void BeginTransmit(const ser4cpp::rseq_t& buffer, opendnp3::ILinkSession& context) final;

    static opendnp3::LinkLayerConfig DefaultConfig();

    MockLogHandler log;
    std::shared_ptr<exe4cpp::MockExecutor> exe;
    std::shared_ptr<MockLinkListener> listener;
    std::shared_ptr<MockTransportLayer> upper;

    opendnp3::LinkLayer link;

    std::string PopLastWriteAsHex();
    uint32_t NumTotalWrites();

private:
    uint32_t numTotalWrites;

    std::deque<std::string> writeQueue;
};

#endif
