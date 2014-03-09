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

TransportLayer::TransportLayer(Logger aLogger, IExecutor* pExecutor_, uint32_t aFragSize) :
	Loggable(aLogger),
	IUpperLayer(aLogger),
	ILowerLayer(aLogger),
	mpState(TLS_Closed::Inst()),
	pExecutor(pExecutor_),
	M_FRAG_SIZE(aFragSize),
	mReceiver(aLogger, this, aFragSize),
	mTransmitter(aLogger, this, aFragSize),
	mThisLayerUp(false)
{

}

///////////////////////////////////////
// Actions
///////////////////////////////////////

void TransportLayer::ThisLayerUp()
{
	mThisLayerUp = true;
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerUp();
}

void TransportLayer::ThisLayerDown()
{
	mReceiver.Reset();
	mThisLayerUp = false;
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerDown();
}

void TransportLayer::ChangeState(TLS_Base* apNewState)
{
	LOG_BLOCK(LogLevel::Debug, "State Change: " << mpState->Name() << " -> " << apNewState->Name());
	mpState = apNewState;
}

void TransportLayer::TransmitAPDU(const openpal::ReadOnlyBuffer& arBuffer)
{
	mTransmitter.Send(arBuffer);
}

void TransportLayer::TransmitTPDU(const openpal::ReadOnlyBuffer& arBuffer)
{
	if(mpLowerLayer)
	{
		mpLowerLayer->Send(arBuffer);
	}
}

void TransportLayer::ReceiveTPDU(const openpal::ReadOnlyBuffer& arBuffer)
{
	mReceiver.HandleReceive(arBuffer);
}

void TransportLayer::ReceiveAPDU(const openpal::ReadOnlyBuffer& arBuffer)
{
	if(mpUpperLayer)
	{
		mpUpperLayer->OnReceive(arBuffer);
	}
}

bool TransportLayer::ContinueSend()
{
	return !mTransmitter.SendSuccess();
}

void TransportLayer::SignalSendSuccess()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendSuccess();
}

void TransportLayer::SignalSendFailure()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendFailure();
}

///////////////////////////////////////
// ILayerDown NVII implementations
///////////////////////////////////////
void TransportLayer::_Send(const ReadOnlyBuffer& arBuffer)
{
	if(arBuffer.IsEmpty() || arBuffer.Size() > M_FRAG_SIZE)
	{
		LOG_BLOCK(LogLevel::Error, "Illegal arg: " << arBuffer.Size() << ", Array length must be in the range [1," << M_FRAG_SIZE << "]");
		pExecutor->Post([this]()
		{
			this->OnSendFailure();
		});
	}
	else
	{
		mpState->Send(arBuffer, this);
	}
}

///////////////////////////////////////
// ILayerUp NVII implementations
///////////////////////////////////////
void TransportLayer::_OnReceive(const ReadOnlyBuffer& arBuffer)
{
	mpState->HandleReceive(arBuffer, this);
}

void TransportLayer::_OnSendSuccess()
{
	mpState->HandleSendSuccess(this);
}

void TransportLayer::_OnSendFailure()
{
	mpState->HandleSendFailure(this);
}

void TransportLayer::_OnLowerLayerUp()
{
	mpState->LowerLayerUp(this);
}

void TransportLayer::_OnLowerLayerDown()
{
	mpState->LowerLayerDown(this);
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

