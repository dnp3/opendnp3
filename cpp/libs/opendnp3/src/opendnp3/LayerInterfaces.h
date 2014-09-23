/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef OPENDNP3_LAYERINTERFACES_H
#define OPENDNP3_LAYERINTERFACES_H

#include <openpal/container/WriteBuffer.h>
#include <openpal/container/ReadOnlyBuffer.h>

#include "Route.h"

#include <assert.h>

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
	virtual void OnLowerLayerUp() = 0;

	// Called by a lower layer when it is no longer available to this layer
	virtual void OnLowerLayerDown() = 0;
};


class ILowerLayer;

class IUpperLayer : public IUpDown
{

public:

	virtual ~IUpperLayer() {}

	// Called by the lower layer when data arrives

	virtual void OnReceive(const openpal::ReadOnlyBuffer&) = 0;

	// Called by lower layer when a previously requested send operation succeeds or fails.
	// Layers can only have 1 outstanding send operation. The callback is guaranteed
	// unless the the OnLowerLayerDown() function is called beforehand
	virtual void OnSendResult(bool isSucccess) = 0;

};

class ILowerLayer
{

public:

	virtual ~ILowerLayer() {}	

	virtual void BeginTransmit(const openpal::ReadOnlyBuffer&) = 0;

};

class HasLowerLayer
{

public:

	HasLowerLayer() : pLowerLayer(nullptr) {}

	// Called by the lower layer when data arrives

	void SetLowerLayer(ILowerLayer& lowerLayer)
	{		
		assert(pLowerLayer == nullptr);
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
		assert(pUpperLayer == nullptr);
		pUpperLayer = &upperLayer;
	}

protected:

	IUpperLayer* pUpperLayer;
};

}

#endif
