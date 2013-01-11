
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
#include "AsyncLayerInterfaces.h"


#include "Exception.h"
#include "Logger.h"
#include "ToHex.h"
#include "LoggableMacros.h"

namespace apl
{

//////////////////////////////////
// IUpDown
//////////////////////////////////

void IUpDown::OnLowerLayerUp()
{
	if(mIsLowerLayerUp) throw InvalidStateException(LOCATION, "LowerLayerUp");
	mIsLowerLayerUp = true;
	this->_OnLowerLayerUp();
}

void IUpDown::OnLowerLayerDown()
{
	if(!mIsLowerLayerUp) throw InvalidStateException(LOCATION, "LowerLayerDown");
	mIsLowerLayerUp = false;
	this->_OnLowerLayerDown();
}


//////////////////////////////////
// IUpperLayer
//////////////////////////////////
IUpperLayer::IUpperLayer(Logger* apLogger) :
	Loggable(apLogger),
	mpLowerLayer(NULL)
{

}

void IUpperLayer::OnReceive(const uint8_t* apData, size_t aNumBytes)
{
	if(this->LogReceive()) {
		LOG_BLOCK(LEV_COMM, RecvString() << " " << toHex(apData, aNumBytes, true));
	}
	this->_OnReceive(apData, aNumBytes); //call the implementation
}

void IUpperLayer::SetLowerLayer(ILowerLayer* apLowerLayer)
{
	assert(apLowerLayer != NULL);
	mpLowerLayer = apLowerLayer;
}

void IUpperLayer::OnSendSuccess()
{
	this->_OnSendSuccess();
}

void IUpperLayer::OnSendFailure()
{
	this->_OnSendFailure();
}

//////////////////////////////////
// ILowerLayer
//////////////////////////////////

ILowerLayer::ILowerLayer(Logger* apLogger) :
	Loggable(apLogger),
	mpUpperLayer(NULL)
{

}

void ILowerLayer::Send(const uint8_t* apData, size_t aNumBytes)
{
	LOG_BLOCK(LEV_COMM, SendString() << " " << toHex(apData, aNumBytes, true));
	this->_Send(apData, aNumBytes);
}

void ILowerLayer::SetUpperLayer(IUpperLayer* apUpperLayer)
{
	assert(apUpperLayer != NULL);
	mpUpperLayer = apUpperLayer;
	apUpperLayer->SetLowerLayer(this);
}

}//end namespace
