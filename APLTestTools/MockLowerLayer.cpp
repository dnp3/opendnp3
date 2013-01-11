
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
#include "MockLowerLayer.h"

#include "BufferHelpers.h"
#include <assert.h>

namespace apl
{

MockLowerLayer::MockLowerLayer(Logger* apLogger) :
	Loggable(apLogger),
	ILowerLayer(apLogger),
	mAutoSendCallback(true),
	mIsSuccess(true)
{

}


void MockLowerLayer::EnableAutoSendCallback(bool aIsSuccess)
{
	mAutoSendCallback = true;
	mIsSuccess = aIsSuccess;
}

void MockLowerLayer::DisableAutoSendCallback()
{
	mAutoSendCallback = false;
}

void MockLowerLayer::_Send(const uint8_t* apData, size_t aNumBytes)
{
	this->WriteToBuffer(apData, aNumBytes);
	if(mAutoSendCallback && mpUpperLayer != NULL) {
		if(mIsSuccess) mpUpperLayer->OnSendSuccess();
		else mpUpperLayer->OnSendFailure();
	}
}

void MockLowerLayer::SendUp(const uint8_t* apData, size_t aNumBytes)
{
	if(this->mpUpperLayer != NULL)
		mpUpperLayer->OnReceive(apData, aNumBytes);
}

void MockLowerLayer::SendUp(const std::string& arHexData)
{
	HexSequence hs(arHexData);
	this->SendUp(hs.Buffer(), hs.Size());
}

void MockLowerLayer::SendSuccess()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnSendSuccess();
}

void MockLowerLayer::SendFailure()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnSendFailure();
}

void MockLowerLayer::ThisLayerUp()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnLowerLayerUp();
}
void MockLowerLayer::ThisLayerDown()
{
	if(mpUpperLayer != NULL) mpUpperLayer->OnLowerLayerDown();
}

}
