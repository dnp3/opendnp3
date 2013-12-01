
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

#include "MockLowerLayer.h"

#include "BufferHelpers.h"
#include <assert.h>

using namespace openpal;

namespace opendnp3
{

MockLowerLayer::MockLowerLayer(Logger aLogger) :
	Loggable(aLogger),
	ILowerLayer(aLogger),
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

void MockLowerLayer::_Send(const openpal::ReadOnlyBuffer& arBuffer)
{
	this->WriteToBuffer(arBuffer);
	if(mAutoSendCallback && mpUpperLayer != nullptr) {
		if(mIsSuccess) mpUpperLayer->OnSendSuccess();
		else mpUpperLayer->OnSendFailure();
	}
}

void MockLowerLayer::SendUp(const openpal::ReadOnlyBuffer& arBuffer)
{
	if(this->mpUpperLayer != nullptr)
		mpUpperLayer->OnReceive(arBuffer);
}

void MockLowerLayer::SendUp(const std::string& arHexData)
{
	HexSequence hs(arHexData);
	this->SendUp(hs.ToReadOnly());
}

void MockLowerLayer::SendSuccess()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendSuccess();
}

void MockLowerLayer::SendFailure()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendFailure();
}

void MockLowerLayer::ThisLayerUp()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerUp();
}
void MockLowerLayer::ThisLayerDown()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerDown();
}

}
