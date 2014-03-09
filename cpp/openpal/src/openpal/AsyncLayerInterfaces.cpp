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

#include "AsyncLayerInterfaces.h"

#include "openpal/LoggableMacros.h"
#include "openpal/ToHex.h"

#include <assert.h>

namespace openpal
{

//////////////////////////////////
// IUpDown
//////////////////////////////////

void IUpDown::OnLowerLayerUp()
{
	if (mIsLowerLayerUp)
	{
		LOG_BLOCK(LogLevel::Error, "Layer already online");
	}
	else
	{
		mIsLowerLayerUp = true;
		this->_OnLowerLayerUp();
	}
}

void IUpDown::OnLowerLayerDown()
{
	if (mIsLowerLayerUp)
	{
		mIsLowerLayerUp = false;
		this->_OnLowerLayerDown();
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Layer not online");
	}
}


//////////////////////////////////
// IUpperLayer
//////////////////////////////////
IUpperLayer::IUpperLayer(openpal::Logger& arLogger) :
	IUpDown(arLogger),
	Loggable(arLogger),
	mpLowerLayer(nullptr)
{

}

void IUpperLayer::OnReceive(const ReadOnlyBuffer& arBuffer)
{
	if(this->LogReceive())
	{
		LOG_BLOCK(LogLevel::Comm, RecvString() << " " << toHex(arBuffer, true));
	}
	this->_OnReceive(arBuffer); //call the implementation
}

void IUpperLayer::SetLowerLayer(ILowerLayer* apLowerLayer)
{
	assert(apLowerLayer != nullptr);
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

ILowerLayer::ILowerLayer(openpal::Logger& arLogger) :
	Loggable(arLogger),
	mpUpperLayer(nullptr)
{

}

void ILowerLayer::Send(const ReadOnlyBuffer& arBuffer)
{
	LOG_BLOCK(LogLevel::Comm, SendString() << " " << toHex(arBuffer, true));
	this->_Send(arBuffer);
}

void ILowerLayer::SetUpperLayer(IUpperLayer* apUpperLayer)
{
	assert(apUpperLayer != nullptr);
	mpUpperLayer = apUpperLayer;
	apUpperLayer->SetLowerLayer(this);
}

}//end namespace
