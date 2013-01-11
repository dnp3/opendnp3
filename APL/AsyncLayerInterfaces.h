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
#ifndef __ASYNC_LAYER_INTERFACES_H_
#define __ASYNC_LAYER_INTERFACES_H_



#include "Types.h"
#include "Loggable.h"

/*
Reusable asynchronous interfaces for protocol stacks.
*/

namespace apl
{

class IUpDown
{
public:
	IUpDown() : mIsLowerLayerUp(false) {}
	virtual ~IUpDown() {}

	// Called by a 'LowerLayer' when it can start performing send services
	void OnLowerLayerUp();

	// Called by a 'LowerLayer' when it can no longer perform Tx services for this layer.
	// Implies previous sends have failed.
	void OnLowerLayerDown();

	bool IsLowerLayerUp() {
		return mIsLowerLayerUp;
	}

private:

private:
	bool mIsLowerLayerUp;

	virtual void _OnLowerLayerUp() = 0;
	virtual void _OnLowerLayerDown() = 0;
};


class Logger;
class ILowerLayer;

class IUpperLayer : public IUpDown, protected virtual Loggable
{
public:
	IUpperLayer(Logger*);
	virtual ~IUpperLayer() {}

	// Called by 'layer down' when data arrives
	void OnReceive(const uint8_t*, size_t);

	// Called by 'layer down' when a previously requested send operation succeeds
	// Layers can only have 1 outstanding send operation. The callback is guaranteed
	// unless the the OnLowerLayerDown() function is called before
	void OnSendSuccess();

	// Called by 'layer down' when a previously requested send operation fails
	void OnSendFailure();

	void SetLowerLayer(ILowerLayer*);


protected:

	ILowerLayer* mpLowerLayer;

	//these are the NVII delegates
	virtual void _OnReceive(const uint8_t*, size_t) = 0;
	virtual void _OnSendSuccess() = 0;
	virtual void _OnSendFailure() = 0;
	virtual bool LogReceive() {
		return true;
	}


	// override this descriptor, it's use in the Hex log messages
	virtual std::string RecvString() const {
		return "<-";
	}
};

class ILowerLayer : protected virtual Loggable
{
public:
	ILowerLayer(Logger*);
	virtual ~ILowerLayer() {}

	void Send(const uint8_t*, size_t);

	void SetUpperLayer(IUpperLayer*);

protected:

	IUpperLayer* mpUpperLayer;

private:

	virtual void _Send(const uint8_t*, size_t) = 0;

	// override this descriptor, it's use in the Hex log messages
	virtual std::string SendString() const {
		return "->";
	}
};

}

#endif
