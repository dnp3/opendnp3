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
#ifndef __LOWER_LAYER_TO_PHYS_ADAPTER_H_
#define __LOWER_LAYER_TO_PHYS_ADAPTER_H_

#include <openpal/channel/IPhysicalLayerCallbacks.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/LayerInterfaces.h>

namespace openpal
{
class IPhysicalLayer;
}

namespace opendnp3
{

/** Class for turning an async physical layer into an ILowerLayer
*/
class LowerLayerToPhysAdapter : public openpal::IPhysicalLayerCallbacks, public ILowerLayer, public HasUpperLayer
{

public:
	LowerLayerToPhysAdapter(openpal::IPhysicalLayer&, bool aAutoRead = true);

	uint32_t GetNumOpenFailure()
	{
		return mNumOpenFailure;
	}

	bool OpenFailureEquals(uint32_t aNum)
	{
		return GetNumOpenFailure() == aNum;
	}

	void StartRead();

	// -------- IHandler --------
	virtual void OnOpenFailure() override final;

	// --------  IUpperLayer ---------
	virtual void OnReceive(const openpal::RSlice& buffer) override final;
	virtual void OnSendResult(bool isSuccess) override final;

	virtual void OnLowerLayerUp() override final;
	virtual void OnLowerLayerDown() override final;

	// --------  ILowerLayer ---------
	virtual void BeginTransmit(const openpal::RSlice& buffer)  override final;


private:

	bool mAutoRead;
	uint32_t mNumOpenFailure;

	static const uint32_t BUFFER_SIZE = 1 << 16; // 65,536

	uint8_t mpBuff[BUFFER_SIZE]; // Temporary buffer since IPhysicalLayer now directly supports a read operation

	openpal::IPhysicalLayer* mpPhys;
};

}

#endif
