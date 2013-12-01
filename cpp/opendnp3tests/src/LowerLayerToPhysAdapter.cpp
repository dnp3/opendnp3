
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

#include "LowerLayerToPhysAdapter.h"

#include <openpal/IPhysicalLayerAsync.h>

using namespace openpal;

namespace opendnp3
{

LowerLayerToPhysAdapter::LowerLayerToPhysAdapter(Logger aLogger, IPhysicalLayerAsync* apPhys, bool aAutoRead) :
	Loggable(aLogger),
	IHandlerAsync(aLogger),
	ILowerLayer(aLogger),
	mAutoRead(aAutoRead),
	mNumOpenFailure(0),
	mpPhys(apPhys)
{
	mpPhys->SetHandler(this);
}

LowerLayerToPhysAdapter::~LowerLayerToPhysAdapter()
{

}

void LowerLayerToPhysAdapter::StartRead()
{
	WriteBuffer buffer(mpBuff, BUFFER_SIZE);
	mpPhys->AsyncRead(buffer);
}

/* Implement IAsyncHandler */
void LowerLayerToPhysAdapter::_OnOpenFailure()
{
	++mNumOpenFailure;
}

/* Implement IUpperLayer */
void LowerLayerToPhysAdapter::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	// process the data into another buffer *before* kicking off another call,
	// otherwise you have a potential race condition
	if(mpUpperLayer != nullptr) mpUpperLayer->OnReceive(arBuffer);
	if(mAutoRead) this->StartRead();
}

void LowerLayerToPhysAdapter::_OnSendSuccess()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendSuccess();
}

void LowerLayerToPhysAdapter::_OnSendFailure()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendFailure();
}

void LowerLayerToPhysAdapter::_OnLowerLayerUp()
{
	if(mAutoRead) this->StartRead();
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerUp();
}

void LowerLayerToPhysAdapter::_OnLowerLayerDown()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerDown();
}

void LowerLayerToPhysAdapter::_OnLowerLayerShutdown()
{

}

/* Implement ILowerLayer */
void LowerLayerToPhysAdapter::_Send(const openpal::ReadOnlyBuffer& arBuffer)
{	
	mpPhys->AsyncWrite(arBuffer);
}

}//end namespace
