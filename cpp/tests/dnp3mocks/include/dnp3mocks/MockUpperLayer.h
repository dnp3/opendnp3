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
#ifndef OPENDNP3_UNITTESTS_MOCKUPPERLAYER_H
#define OPENDNP3_UNITTESTS_MOCKUPPERLAYER_H

#include "dnp3mocks/DataSink.h"

#include <LayerInterfaces.h>

class MockUpperLayer final : public opendnp3::IUpperLayer, public opendnp3::HasLowerLayer
{
public:
    struct Counters
    {
        size_t numTxReady = 0;
        size_t numLayerUp = 0;
        size_t numLayerDown = 0;
    };

    MockUpperLayer();

    bool IsOnline() const
    {
        return isOnline;
    }

    bool SendDown(const std::string& hex, const opendnp3::Addresses& addresses = opendnp3::Addresses());
    bool SendDown(const ser4cpp::rseq_t& data, const opendnp3::Addresses& addresses = opendnp3::Addresses());

    const Counters& GetCounters() const
    {
        return counters;
    }

    // these are the NVII delegates
    virtual bool OnReceive(const opendnp3::Message& message) override;
    virtual bool OnTxReady() override;
    virtual bool OnLowerLayerUp() override;
    virtual bool OnLowerLayerDown() override;

    DataSink received;

private:
    bool isOnline;
    Counters counters;
};

#endif
