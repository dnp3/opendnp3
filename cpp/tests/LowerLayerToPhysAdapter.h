
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

#ifndef __LOWER_LAYER_TO_PHYS_ADAPTER_H_
#define __LOWER_LAYER_TO_PHYS_ADAPTER_H_


#include <opendnp3/IHandlerAsync.h>
#include <opendnp3/AsyncLayerInterfaces.h>

namespace opendnp3
{

class IPhysicalLayerAsync;

/** Class for turning an async physical layer into an ILowerLayer
*/
class LowerLayerToPhysAdapter : public IHandlerAsync, public ILowerLayer
{
public:
	LowerLayerToPhysAdapter(openpal::Logger&, IPhysicalLayerAsync*, bool aAutoRead = true);
	~LowerLayerToPhysAdapter();

	size_t GetNumOpenFailure() {
		return mNumOpenFailure;
	}
	bool OpenFailureEquals(size_t aNum) {
		return GetNumOpenFailure() == aNum;
	}

	void StartRead();



private:

	virtual std::string RecvString() const {
		return "Adapter <-";
	}
	virtual std::string SendString() const {
		return "Adapter ->";
	}

	bool mAutoRead;
	size_t mNumOpenFailure;

	static const size_t BUFFER_SIZE = 1 << 16; // 65,536

	uint8_t mpBuff[BUFFER_SIZE]; // Temporary buffer since IPhysicalLayerAsync now directly supports a read operation

	/* Implement IAsyncHandler */
	void _OnOpenFailure();

	/* Implement IUpperLayer */
	void _OnReceive(const uint8_t*, size_t);
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnLowerLayerShutdown();

	IPhysicalLayerAsync* mpPhys;

	/* Implement ILowerLayer */
	void _Send(const uint8_t*, size_t);
};

}

#endif
