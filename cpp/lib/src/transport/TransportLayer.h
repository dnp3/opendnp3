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
#ifndef OPENDNP3_TRANSPORTLAYER_H
#define OPENDNP3_TRANSPORTLAYER_H

#include "LayerInterfaces.h"
#include "TransportRx.h"
#include "TransportTx.h"
#include "link/ILinkLayer.h"

#include "opendnp3/StackStatistics.h"
#include "opendnp3/logging/Logger.h"

#include <exe4cpp/IExecutor.h>

namespace opendnp3
{

/**
    Implements the DNP3 transport layer
*/
class TransportLayer final : public IUpperLayer, public ILowerLayer
{

public:
    TransportLayer(const Logger& logger, uint32_t maxRxFragSize);

    // ------ ILowerLayer ------

    virtual bool BeginTransmit(const Message& message) override;

    // ------ IUpperLayer ------

    virtual bool OnReceive(const Message& message) override;
    virtual bool OnLowerLayerUp() override final;
    virtual bool OnLowerLayerDown() override;
    virtual bool OnTxReady() override;

    void SetAppLayer(IUpperLayer& upperLayer);

    void SetLinkLayer(ILinkLayer& linkLayer);

    StackStatistics::Transport GetStatistics() const;

private:
    Logger logger;

    IUpperLayer* upper = nullptr;
    ILinkLayer* lower = nullptr;

    // ---- state ----
    bool isOnline = false;
    bool isSending = false;

    // ----- Transmitter and Receiver Classes ------
    TransportRx receiver;
    TransportTx transmitter;
};

} // namespace opendnp3

#endif
