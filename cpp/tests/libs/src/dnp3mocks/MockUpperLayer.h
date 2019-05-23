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
#ifndef OPENDNP3_MOCKUPPERLAYER_H
#define OPENDNP3_MOCKUPPERLAYER_H

#include "DataSink.h"

#include <opendnp3/LayerInterfaces.h>

namespace opendnp3
{

class MockUpperLayer final : public IUpperLayer, public HasLowerLayer
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

    bool SendDown(const std::string& hex, const Addresses& addresses = Addresses());
    bool SendDown(const openpal::RSlice& data, const Addresses& addresses = Addresses());

    const Counters& GetCounters() const
    {
        return counters;
    }

    // these are the NVII delegates
    virtual bool OnReceive(const Message& message) override;
    virtual bool OnTxReady() override;
    virtual bool OnLowerLayerUp() override;
    virtual bool OnLowerLayerDown() override;

    DataSink received;

private:
    bool isOnline;
    Counters counters;
};

} // namespace opendnp3

#endif
