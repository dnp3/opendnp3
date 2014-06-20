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
#ifndef __PHYSICAL_LAYER_WRAPPER_H_
#define __PHYSICAL_LAYER_WRAPPER_H_

#include <openpal/channel/IPhysicalLayer.h>
#include <openpal/channel/IPhysicalLayerCallbacks.h>
#include <openpal/logging/Logger.h>

#include "RandomDouble.h"

namespace opendnp3
{

class PhysicalLayerWrapper : public openpal::IPhysicalLayer, public openpal::IPhysicalLayerCallbacks
{
public:

	PhysicalLayerWrapper(const openpal::Logger& logger, openpal::IPhysicalLayer* apProxy);

	openpal::IExecutor* GetExecutor()
	{
		return mpProxy->GetExecutor();
	}

	bool CanOpen() const
	{
		return mpProxy->CanOpen();
	}
	bool CanClose() const
	{
		return mpProxy->CanClose();
	}
	bool CanRead() const
	{
		return mpProxy->CanRead();
	}
	bool CanWrite() const
	{
		return mpProxy->CanWrite();
	}

	bool IsReading() const
	{
		return mpProxy->IsReading();
	}
	bool IsWriting() const
	{
		return mpProxy->IsWriting();
	}
	bool IsClosing() const
	{
		return mpProxy->IsClosing();
	}
	bool IsClosed() const
	{
		return mpProxy->IsClosed();
	}
	bool IsOpening() const
	{
		return mpProxy->IsOpening();
	}
	bool IsOpen() const
	{
		return mpProxy->IsOpen();
	}	

	virtual void BeginOpen() override final;
	virtual void BeginClose() override final;
	virtual void BeginWrite(const openpal::ReadOnlyBuffer&) override final;
	virtual void BeginRead(openpal::WriteBuffer&) override final;

	void SetHandler(openpal::IPhysicalLayerCallbacks* apHandler);

	// testing helpers
	void SetCorruptionProbability(double aProbability);

	virtual void OnLowerLayerUp() override final;
	virtual void OnLowerLayerDown() override final;
	virtual void OnReceive(const openpal::ReadOnlyBuffer& arBuffer) override final;
	virtual void OnSendResult(bool isSuccess) override final;
	virtual void OnOpenFailure() override final;

private:

	openpal::Logger logger;
	double mCorruptionProbability;
	RandomDouble mRandom;
	openpal::IPhysicalLayer* mpProxy;
	openpal::IPhysicalLayerCallbacks* mpHandler;
};

}

#endif
