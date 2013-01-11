//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "TransportStates.h"

#include "TransportLayer.h"

namespace apl
{
namespace dnp
{

//////////////////////////////////////////////////////
//	TLS_Closed
//////////////////////////////////////////////////////
TLS_Closed TLS_Closed::mInstance;

void TLS_Closed::LowerLayerUp(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Ready::Inst());
	apContext->ThisLayerUp();
}

//////////////////////////////////////////////////////
//	TLS_Ready
//////////////////////////////////////////////////////
TLS_Ready TLS_Ready::mInstance;

void TLS_Ready::Send(const uint8_t* apData, size_t aNumBytes, TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Sending::Inst());
	apContext->TransmitAPDU(apData, aNumBytes);
}

void TLS_Ready::LowerLayerDown(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Closed::Inst());
	apContext->ThisLayerDown();
}

void TLS_Ready::HandleReceive(const uint8_t* apData, size_t aNumBytes, TransportLayer* apContext)
{
	apContext->ReceiveTPDU(apData, aNumBytes);
}

//////////////////////////////////////////////////////
//	TLS_Sending
//////////////////////////////////////////////////////
TLS_Sending TLS_Sending::mInstance;

void TLS_Sending::HandleReceive(const uint8_t* apData, size_t aNumBytes, TransportLayer* apContext)
{
	apContext->ReceiveTPDU(apData, aNumBytes);
}

void TLS_Sending::HandleSendSuccess(TransportLayer* apContext)
{
	if(!apContext->ContinueSend()) {
		apContext->ChangeState(TLS_Ready::Inst()); // order important here
		apContext->SignalSendSuccess();
	}
}

void TLS_Sending::LowerLayerDown(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Closed::Inst());
	apContext->ThisLayerDown();
}

void TLS_Sending::HandleSendFailure(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Ready::Inst());
	apContext->SignalSendFailure();
}

}
} //end namespaces
