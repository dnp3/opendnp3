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
#include "utils/LinkLayerTest.h"

#include <ser4cpp/util/HexConversions.h>

using namespace opendnp3;
using namespace ser4cpp;

LinkLayerTest::LinkLayerTest(const LinkConfig& config) : LinkLayerTest(LinkLayerConfig(config, false)) {}

LinkLayerTest::LinkLayerTest(const LinkLayerConfig& config)
    : exe(std::make_shared<exe4cpp::MockExecutor>()),
      listener(std::make_shared<MockLinkListener>()),
      upper(std::make_shared<MockTransportLayer>()),
      link(log.logger, exe, upper, listener, config),
      numTotalWrites(0)
{
    upper->SetLinkLayer(link);
    link.SetRouter(*this);
}

bool LinkLayerTest::OnFrame(LinkFunction func,
                            bool isMaster,
                            bool fcb,
                            bool fcvdfc,
                            uint16_t dest,
                            uint16_t source,
                            const ser4cpp::rseq_t& userdata)
{
    LinkHeaderFields fields(func, isMaster, fcb, fcvdfc, Addresses(source, dest));
    return link.OnFrame(fields, userdata);
}

std::string LinkLayerTest::PopLastWriteAsHex()
{
    if (writeQueue.empty())
    {
        return "";
    }

    while (writeQueue.size() > 1)
    {
        writeQueue.pop_front();
    }

    auto ret = writeQueue.front();
    writeQueue.pop_front();
    return ret;
}

uint32_t LinkLayerTest::NumTotalWrites()
{
    return numTotalWrites;
}

void LinkLayerTest::BeginTransmit(const ser4cpp::rseq_t& buffer, ILinkSession& /*context*/)
{
    ++numTotalWrites;
    this->writeQueue.push_back(HexConversions::to_hex(buffer));
}

LinkLayerConfig LinkLayerTest::DefaultConfig()
{
    return LinkLayerConfig(LinkConfig(true, false), false);
}
