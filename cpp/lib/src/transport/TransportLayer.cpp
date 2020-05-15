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
#include "TransportLayer.h"

#include "TransportConstants.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <cassert>

namespace opendnp3
{

TransportLayer::TransportLayer(const Logger& logger, uint32_t maxRxFragSize)
    : logger(logger), receiver(logger, maxRxFragSize), transmitter(logger)
{
}

///////////////////////////////////////
// Actions
///////////////////////////////////////

bool TransportLayer::BeginTransmit(const Message& message)
{
    if (!isOnline)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
        return false;
    }

    if (message.payload.is_empty())
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "APDU cannot be empty");
        return false;
    }

    if (isSending)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Invalid BeginTransmit call, already transmitting");
        return false;
    }

    if (!lower)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Can't send without an attached link layer");
        return false;
    }

    isSending = true;
    transmitter.Configure(message);
    lower->Send(transmitter);

    return true;
}

///////////////////////////////////////
// IUpperLayer
///////////////////////////////////////

bool TransportLayer::OnReceive(const Message& message)
{
    if (isOnline)
    {
        const auto asdu = receiver.ProcessReceive(message);
        if (upper && asdu.payload.is_not_empty())
        {
            upper->OnReceive(asdu);
        }
        return true;
    }

    SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
    return false;
}

bool TransportLayer::OnTxReady()
{
    if (!isOnline)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
        return false;
    }

    if (!isSending)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Invalid send callback");
        return false;
    }

    isSending = false;

    if (upper)
    {
        upper->OnTxReady();
    }

    return true;
}

void TransportLayer::SetAppLayer(IUpperLayer& upperLayer)
{
    assert(!upper);
    upper = &upperLayer;
}

void TransportLayer::SetLinkLayer(ILinkLayer& linkLayer)
{
    assert(!lower);
    lower = &linkLayer;
}

StackStatistics::Transport TransportLayer::GetStatistics() const
{
    return StackStatistics::Transport(this->receiver.Statistics(), this->transmitter.Statistics());
}

bool TransportLayer::OnLowerLayerUp()
{
    if (isOnline)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already online");
        return false;
    }

    isOnline = true;
    if (upper)
    {
        upper->OnLowerLayerUp();
    }
    return true;
}

bool TransportLayer::OnLowerLayerDown()
{
    if (!isOnline)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already offline");
        return false;
    }

    isOnline = false;
    isSending = false;
    receiver.Reset();

    if (upper)
    {
        upper->OnLowerLayerDown();
    }

    return true;
}

} // namespace opendnp3
