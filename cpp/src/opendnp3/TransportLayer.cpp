
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "TransportLayer.h"

#include <opendnp3/Logger.h>
#include <opendnp3/Exception.h>
#include <opendnp3/TransportConstants.h>

#include "LoggableMacros.h"
#include "TransportStates.h"

#include <assert.h>
#include <sstream>

using namespace std;

namespace opendnp3
{

TransportLayer::TransportLayer(Logger* apLogger, size_t aFragSize) :
	Loggable(apLogger),
	IUpperLayer(apLogger),
	ILowerLayer(apLogger),
	mpState(TLS_Closed::Inst()),
	M_FRAG_SIZE(aFragSize),
	mReceiver(apLogger, this, aFragSize),
	mTransmitter(apLogger, this, aFragSize),
	mThisLayerUp(false)
{

}

///////////////////////////////////////
// Actions
///////////////////////////////////////

void TransportLayer::ThisLayerUp()
{
	mThisLayerUp = true;
	if(mpUpperLayer != NULL) mpUpperLayer->OnLowerLayerUp();
}

void TransportLayer::ThisLayerDown()
{
	mReceiver.Reset();
	mThisLayerUp = false;
	if(mpUpperLayer != NULL) mpUpperLayer->OnLowerLayerDown();
}

void TransportLayer::ChangeState(TLS_Base* apNewState)
{
	LOG_BLOCK(LEV_DEBUG, "State Change: " << mpState->Name() << " -> " << apNewState->Name());
	mpState = apNewState;
}

void TransportLayer::TransmitAPDU(const uint8_t* apData, size_t aNumBytes)
{
	mTransmitter.Send(apData, aNumBytes);
}

void TransportLayer::TransmitTPDU(const uint8_t* apData, size_t aNumBytes)
{
	if(mpLowerLayer != NULL) mpLowerLayer->Send(apData, aNumBytes);
}

void TransportLayer::ReceiveTPDU(const uint8_t* apData, size_t aNumBytes)
{
	mReceiver.HandleReceive(apData, aNumBytes);
}

void TransportLayer::ReceiveAPDU(const uint8_t* apData, size_t aNumBytes)
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnReceive(apData, aNumBytes);
}

bool TransportLayer::ContinueSend()
{
	return !mTransmitter.SendSuccess();
}

void TransportLayer::SignalSendSuccess()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnSendSuccess();
}

void TransportLayer::SignalSendFailure()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnSendFailure();
}

///////////////////////////////////////
// ILayerDown NVII implementations
///////////////////////////////////////
void TransportLayer::_Send(const uint8_t* apData, size_t aNumBytes)
{
	if(aNumBytes == 0 || aNumBytes > M_FRAG_SIZE) {
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Illegal arg: " << aNumBytes << ", Array length must be in the range [1," << M_FRAG_SIZE << "]");
	}

	mpState->Send(apData, aNumBytes, this);
}

///////////////////////////////////////
// ILayerUp NVII implementations
///////////////////////////////////////
void TransportLayer::_OnReceive(const uint8_t* apData, size_t aNumBytes)
{
	mpState->HandleReceive(apData, aNumBytes, this);
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

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string TransportLayer::ToString(uint8_t aHeader)
{
	std::ostringstream oss;
	oss << "TL: ";
	if((aHeader & TL_HDR_FIR) != 0) oss << "FIR ";
	if((aHeader & TL_HDR_FIN) != 0) oss << "FIN ";
	oss << "#" << static_cast<int>(aHeader & TL_HDR_SEQ);
	return oss.str();
}
#endif

} //end namespace

