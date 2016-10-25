/*
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
#include "LowerLayerToPhysAdapter.h"

#include <openpal/channel/IPhysicalLayer.h>

using namespace openpal;

namespace opendnp3
{

LowerLayerToPhysAdapter::LowerLayerToPhysAdapter(IPhysicalLayer& phys, bool aAutoRead) :
	mAutoRead(aAutoRead),
	mNumOpenFailure(0),
	mpPhys(&phys)
{
	mpPhys->SetHandler(this);
}

void LowerLayerToPhysAdapter::StartRead()
{
	WSlice buffer(mpBuff, BUFFER_SIZE);
	mpPhys->BeginRead(buffer);
}

/* Implement IHandler */
void LowerLayerToPhysAdapter::OnOpenFailure()
{
	++mNumOpenFailure;
}

/* Implement IUpperLayer */
void LowerLayerToPhysAdapter::OnReceive(const openpal::RSlice& buffer)
{
	// process the data into another buffer *before* kicking off another call,
	// otherwise you have a potential race condition
	if (pUpperLayer)
	{
		pUpperLayer->OnReceive(buffer);
	}

	if (mAutoRead)
	{
		this->StartRead();
	}
}

void LowerLayerToPhysAdapter::OnSendResult(bool isSuccess)
{
	if (pUpperLayer)
	{
		pUpperLayer->OnSendResult(isSuccess);
	}
}

void LowerLayerToPhysAdapter::OnLowerLayerUp()
{
	if (mAutoRead)
	{
		this->StartRead();
	}

	if (pUpperLayer)
	{
		pUpperLayer->OnLowerLayerUp();
	}
}

void LowerLayerToPhysAdapter::OnLowerLayerDown()
{
	if (pUpperLayer)
	{
		pUpperLayer->OnLowerLayerDown();
	}
}


void LowerLayerToPhysAdapter::BeginTransmit(const openpal::RSlice& buffer)
{
	mpPhys->BeginWrite(buffer);
}

}//end namespace
