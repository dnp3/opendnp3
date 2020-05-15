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
#ifndef OPENDNP3_LAYERINTERFACES_H
#define OPENDNP3_LAYERINTERFACES_H

#include "app/Message.h"

#include <cassert>

namespace opendnp3
{

/**
 * Describes a layer that can be opened or closed in response to the
 * availability of the layer below it.
 */
class IUpDown
{

public:
    virtual ~IUpDown() {}

    // Called by a lower Layer when it is available to this layer
    // return false if the layer is already up
    virtual bool OnLowerLayerUp() = 0;

    // Called by a lower layer when it is no longer available to this layer
    // return false if the layer is already down
    virtual bool OnLowerLayerDown() = 0;
};

class IUpperLayer : public IUpDown
{

public:
    virtual ~IUpperLayer() {}

    // Called by the lower layer when data arrives
    // return false if the layer is down
    virtual bool OnReceive(const Message& message) = 0;

    // Called by the lower layer when it is ready to transmit more data
    virtual bool OnTxReady() = 0;
};

class ILowerLayer
{

public:
    virtual ~ILowerLayer() {}

    virtual bool BeginTransmit(const Message& message) = 0;
};

class HasLowerLayer
{

public:
    HasLowerLayer() : pLowerLayer(nullptr) {}

    // Called by the lower layer when data arrives

    void SetLowerLayer(ILowerLayer& lowerLayer)
    {
        assert(!pLowerLayer);
        pLowerLayer = &lowerLayer;
    }

protected:
    ILowerLayer* pLowerLayer;
};

class HasUpperLayer
{

public:
    HasUpperLayer() : pUpperLayer(nullptr) {}

    // Called by the lower layer when data arrives

    void SetUpperLayer(IUpperLayer& upperLayer)
    {
        assert(!pUpperLayer);
        pUpperLayer = &upperLayer;
    }

protected:
    IUpperLayer* pUpperLayer;
};

} // namespace opendnp3

#endif
