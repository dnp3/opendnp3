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
#include "dnp3mocks/MockTransportLayer.h"

#include <ser4cpp/util/HexConversions.h>

using namespace opendnp3;
using namespace ser4cpp;

MockTransportLayer::MockTransportLayer() : pLinkLayer(nullptr), isOnline(false) {}

void MockTransportLayer::SetLinkLayer(ILinkLayer& linkLayer)
{
    this->pLinkLayer = &linkLayer;
}

bool MockTransportLayer::SendDown(ITransportSegment& segments)
{
    return pLinkLayer->Send(segments);
}

bool MockTransportLayer::OnReceive(const Message& message)
{
    receivedQueue.push_back(HexConversions::to_hex(message.payload));
    return true;
}

bool MockTransportLayer::OnTxReady()
{
    ++(this->counters.numTxReady);
    return true;
}

bool MockTransportLayer::OnLowerLayerUp()
{
    assert(!isOnline);
    isOnline = true;
    ++counters.numLayerUp;
    return true;
}

bool MockTransportLayer::OnLowerLayerDown()
{
    assert(isOnline);
    isOnline = false;
    ++counters.numLayerDown;
    return true;
}
