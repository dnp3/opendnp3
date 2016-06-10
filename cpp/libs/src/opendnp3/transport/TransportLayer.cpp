/*
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
#include "TransportLayer.h"

#include <openpal/logging/LogMacros.h>

#include "TransportConstants.h"

#include "opendnp3/LogLevels.h"

#include <assert.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{


TransportLayer::TransportLayer(openpal::Logger logger, openpal::IExecutor& executor, uint32_t maxRxFragSize, StackStatistics* pStatistics) :
	logger(logger),
	pUpperLayer(nullptr),
	pLinkLayer(nullptr),
	isOnline(false),
	isSending(false),
	pExecutor(&executor),
	receiver(logger, maxRxFragSize, pStatistics),
	transmitter(logger, pStatistics)
{

}

///////////////////////////////////////
// Actions
///////////////////////////////////////

void TransportLayer::BeginTransmit(const RSlice& apdu)
{
	if (!isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
		return;
	}


	if (apdu.IsEmpty())
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "APDU cannot be empty");
		auto lambda = [this]()
		{
			this->OnSendResult(false);
		};
		pExecutor->PostLambda(lambda);
		return;
	}


	if (isSending)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Invalid BeginTransmit call, already transmitting");
		return;
	}

	if (!pLinkLayer)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Can't send without an attached link layer");
		auto lambda = [this]()
		{
			this->OnSendResult(false);
		};
		pExecutor->PostLambda(lambda);
		return;
	}

	isSending = true;
	transmitter.Configure(apdu);
	pLinkLayer->Send(transmitter);
}

///////////////////////////////////////
// IUpperLayer
///////////////////////////////////////

bool TransportLayer::OnReceive(const RSlice& tpdu)
{
	if (isOnline)
	{
		auto apdu = receiver.ProcessReceive(tpdu);
		if (apdu.IsNotEmpty() && pUpperLayer)
		{
			pUpperLayer->OnReceive(apdu);
		}
		return true;
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
		return false;
	}
}

bool TransportLayer::OnSendResult(bool isSuccess)
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

	if (pUpperLayer)
	{
		pUpperLayer->OnSendResult(isSuccess);
	}

	return true;
}

void TransportLayer::SetAppLayer(IUpperLayer& upperLayer)
{
	assert(pUpperLayer == nullptr);
	pUpperLayer = &upperLayer;
}

void TransportLayer::SetLinkLayer(ILinkLayer& linkLayer)
{
	assert(pLinkLayer == nullptr);
	pLinkLayer = &linkLayer;
}

bool TransportLayer::OnLowerLayerUp()
{
	if (isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already online");
		return false;
	}

	isOnline = true;
	if (pUpperLayer)
	{
		pUpperLayer->OnLowerLayerUp();
	}
	return true;
}

bool TransportLayer::OnLowerLayerDown()
{
	if (!isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already offline");
		return true;
	}

	isOnline = false;
	isSending = false;
	receiver.Reset();

	if (pUpperLayer)
	{
		pUpperLayer->OnLowerLayerDown();
	}

	return true;
}

///////////////////////////////////////
// Helpers
///////////////////////////////////////

} //end namespace

