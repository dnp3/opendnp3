/*
 * Copyright 2013-2019 Automatak, LLC
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
#ifndef OPENDNP3_LINKLAYERTEST_H
#define OPENDNP3_LINKLAYERTEST_H

#include "MockTransportLayer.h"

#include <opendnp3/LogLevels.h>
#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/LinkLayer.h>

#include <dnp3mocks/MockLinkListener.h>

#include "testlib/MockExecutor.h"
#include "testlib/MockLogHandler.h"

#include <queue>

namespace opendnp3
{

class LinkLayerTest : public ILinkTx
{
public:
    LinkLayerTest(const LinkConfig& config);

    LinkLayerTest(const LinkLayerConfig& config = DefaultConfig());

    bool OnFrame(LinkFunction func,
                 bool isMaster,
                 bool fcb,
                 bool fcvdfc,
                 uint16_t dest,
                 uint16_t source,
                 const openpal::RSlice& userdata = openpal::RSlice::Empty());

    // ILinkTx interface
    virtual void BeginTransmit(const openpal::RSlice& buffer, ILinkSession& context) override final;

    static LinkLayerConfig DefaultConfig();

    testlib::MockLogHandler log;
    std::shared_ptr<testlib::MockExecutor> exe;
    std::shared_ptr<MockLinkListener> listener;
    std::shared_ptr<MockTransportLayer> upper;

    LinkLayer link;

    std::string PopLastWriteAsHex();
    uint32_t NumTotalWrites();

private:
    uint32_t numTotalWrites;

    std::deque<std::string> writeQueue;
};

} // namespace opendnp3

#endif
