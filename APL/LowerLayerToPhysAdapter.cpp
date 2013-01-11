
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
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "LowerLayerToPhysAdapter.h"


#include "IPhysicalLayerAsync.h"

namespace apl
{

LowerLayerToPhysAdapter::LowerLayerToPhysAdapter(Logger* apLogger, IPhysicalLayerAsync* apPhys, bool aAutoRead) :
	Loggable(apLogger),
	IHandlerAsync(apLogger),
	ILowerLayer(apLogger),
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
	mpPhys->AsyncRead(mpBuff, BUFFER_SIZE);
}

/* Implement IAsyncHandler */
void LowerLayerToPhysAdapter::_OnOpenFailure()
{
	++mNumOpenFailure;
}

/* Implement IUpperLayer */
void LowerLayerToPhysAdapter::_OnReceive(const uint8_t* apData, size_t aNumBytes)
{
	// process the data into another buffer *before* kicking off another call,
	// otherwise you have a potential race condition
	if(mpUpperLayer != NULL) mpUpperLayer->OnReceive(apData, aNumBytes);
	if(mAutoRead) this->StartRead();
}

void LowerLayerToPhysAdapter::_OnSendSuccess()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnSendSuccess();
}

void LowerLayerToPhysAdapter::_OnSendFailure()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnSendFailure();
}

void LowerLayerToPhysAdapter::_OnLowerLayerUp()
{
	if(mAutoRead) this->StartRead();
	if(mpUpperLayer != NULL) mpUpperLayer->OnLowerLayerUp();
}

void LowerLayerToPhysAdapter::_OnLowerLayerDown()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnLowerLayerDown();
}

void LowerLayerToPhysAdapter::_OnLowerLayerShutdown()
{

}

/* Implement ILowerLayer */
void LowerLayerToPhysAdapter::_Send(const uint8_t* apData, size_t aNumBytes)
{
	mpPhys->AsyncWrite(apData, aNumBytes);
}

}//end namespace
