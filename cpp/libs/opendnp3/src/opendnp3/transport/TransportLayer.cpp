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
#include "TransportLayer.h"

#include <openpal/logging/LogMacros.h>

#include "TransportConstants.h"

#include "opendnp3/LogLevels.h"

#include <assert.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportLayer::TransportLayer(openpal::LogRoot& root, openpal::IExecutor* pExecutor_, uint32_t maxRxFragSize, StackStatistics* pStatistics) :
	logger(root.GetLogger()),
	pUpperLayer(nullptr),
	pLinkLayer(nullptr),
	isOnline(false),
	isSending(false),
	pExecutor(pExecutor_),	
	receiver(logger, maxRxFragSize, pStatistics),
	transmitter(logger, pStatistics)
{

}

///////////////////////////////////////
// Actions
///////////////////////////////////////

void TransportLayer::BeginTransmit(const ReadOnlyBuffer& apdu)
{
	if (isOnline)
	{
		if (apdu.IsEmpty())
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "APDU cannot be empty");
			auto lambda = [this]() { this->OnSendResult(false); };
			pExecutor->PostLambda(lambda);
		}
		else
		{			
			if (isSending)
			{
				SIMPLE_LOG_BLOCK(logger, flags::ERR, "Invalid BeginTransmit call, already transmitting");
			}
			else
			{	
				isSending = true;

				if (pLinkLayer)
				{					
					transmitter.Configure(apdu);
					pLinkLayer->Send(transmitter);
				}
				else
				{	
					SIMPLE_LOG_BLOCK(logger, flags::ERR, "Can't send without an attached link layer");
					auto lambda = [this]() { this->OnSendResult(false); };
					pExecutor->PostLambda(lambda);
				}
			}
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
	}
}

///////////////////////////////////////
// IUpperLayer
///////////////////////////////////////

void TransportLayer::OnReceive(const ReadOnlyBuffer& tpdu)
{
	if (isOnline)
	{
		auto apdu = receiver.ProcessReceive(tpdu);
		if (apdu.IsNotEmpty() && pUpperLayer)
		{
			pUpperLayer->OnReceive(apdu);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
	}
}

void TransportLayer::OnSendResult(bool isSuccess)
{
	if (isOnline)
	{
		if (isSending)
		{
			isSending = false;

			if (pUpperLayer)
			{
				pUpperLayer->OnSendResult(isSuccess);
			}
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Invalid send callback");
		}		
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer offline");
	}
}

void TransportLayer::SetAppLayer(IUpperLayer* pUpperLayer_)
{
	assert(pUpperLayer_ != nullptr);
	assert(pUpperLayer == nullptr);
	pUpperLayer = pUpperLayer_;	
}

void TransportLayer::SetLinkLayer(ILinkLayer* pLinkLayer_)
{
	assert(pLinkLayer_ != nullptr);
	assert(pLinkLayer == nullptr);
	pLinkLayer = pLinkLayer_;
}

void TransportLayer::OnLowerLayerUp()
{
	if (isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already online");
	}
	else
	{
		isOnline = true;
		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerUp();
		}
	}
}

void TransportLayer::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
		isSending = false;
		receiver.Reset();
		
		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerDown();
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already offline");
	}
}

///////////////////////////////////////
// Helpers
///////////////////////////////////////

} //end namespace

