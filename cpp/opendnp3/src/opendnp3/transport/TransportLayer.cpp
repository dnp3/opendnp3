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


#include "TransportConstants.h"

#include <openpal/LoggableMacros.h>

#include "TransportStates.h"

#include <assert.h>
#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportLayer::TransportLayer(const openpal::Logger& logger, openpal::IExecutor* pExecutor_, uint32_t maxFragSize) :
	Loggable(logger),
	isOnline(false),
	pState(TLS_Ready::Inst()),
	pExecutor(pExecutor_),
	M_FRAG_SIZE(maxFragSize),
	receiver(logger, this, maxFragSize)	
{

}

///////////////////////////////////////
// Actions
///////////////////////////////////////

void TransportLayer::ChangeState(TLS_Base* pNewState)
{
	LOG_BLOCK(LogLevel::Debug, "State Change: " << pState->Name() << " -> " << pNewState->Name());
	pState = pNewState;
}

void TransportLayer::TransmitAPDU(const openpal::ReadOnlyBuffer& apdu)
{
	transmitter.Configure(apdu);
	if(pLinkLayer)
	{
		//pLowerLayer->Send()
	}
}

void TransportLayer::ReceiveTPDU(const openpal::ReadOnlyBuffer& tpdu)
{
	receiver.HandleReceive(tpdu);
}

void TransportLayer::ReceiveAPDU(const openpal::ReadOnlyBuffer& apdu)
{
	if(pUpperLayer)
	{
		pUpperLayer->OnReceive(apdu);
	}
}

void TransportLayer::SignalSendResult(bool isSuccess)
{
	if (pUpperLayer)
	{
		pUpperLayer->OnSendResult(isSuccess);
	}
}

void TransportLayer::Send(const ReadOnlyBuffer& apdu)
{	
	if (isOnline)
	{
		if (apdu.IsEmpty() || apdu.Size() > M_FRAG_SIZE)
		{
			LOG_BLOCK(LogLevel::Error, "Illegal arg: " << apdu.Size() << ", Array length must be in the range [1," << M_FRAG_SIZE << "]");
			pExecutor->Post([this]()
			{
				this->OnSendResult(false);
			});
		}
		else
		{
			pState->Send(apdu, this);
		}
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Layer offline");
	}
}

///////////////////////////////////////
// IUpperLayer
///////////////////////////////////////

void TransportLayer::OnReceive(const ReadOnlyBuffer& tpdu)
{
	if (isOnline)
	{
		pState->HandleReceive(tpdu, this);
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Layer offline");
	}
}

void TransportLayer::OnSendResult(bool isSuccess)
{
	if (isOnline)
	{
		if (isSuccess)
		{
			pState->HandleSendSuccess(this);
		}
		else
		{
			pState->HandleSendFailure(this);
		}
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Layer offline");
	}
}

void TransportLayer::SetAppLayer(openpal::IUpperLayer* pUpperLayer_)
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
		LOG_BLOCK(LogLevel::Error, "Layer already online");
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
		receiver.Reset();
		pState = TLS_Ready::Inst();
		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerDown();
		}		
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Layer already offline");
	}
}

///////////////////////////////////////
// Helpers
///////////////////////////////////////

std::string TransportLayer::ToString(uint8_t aHeader)
{
	std::ostringstream oss;
	oss << "TL: ";
	if((aHeader & TL_HDR_FIR) != 0) oss << "FIR ";
	if((aHeader & TL_HDR_FIN) != 0) oss << "FIN ";
	oss << "#" << static_cast<int>(aHeader & TL_HDR_SEQ);
	return oss.str();
}

} //end namespace

