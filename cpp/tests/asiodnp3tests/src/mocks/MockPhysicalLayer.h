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
#ifndef __MOCK_PHYSICAL_LAYER_H_
#define __MOCK_PHYSICAL_LAYER_H_

#include <asiopal/PhysicalLayerBase.h>

#include <openpal/channel/IPhysicalLayerCallbacks.h>

#include <testlib/BufferTestObject.h>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{


// Provides a backend for testing physical layers
class MockPhysicalLayer : public asiopal::PhysicalLayerBase, public testlib::BufferTestObject
{
public:
	MockPhysicalLayer(openpal::Logger logger, openpal::IExecutor& executor);

	openpal::IExecutor* GetExecutor()
	{
		return pExecutor;
	}

	void SignalOpenSuccess();
	void SignalOpenFailure();
	void SignalSendSuccess();
	void SignalSendFailure();
	void SignalReadFailure();

	void TriggerRead(const std::string& arData);
	void TriggerClose();

	size_t NumWrites()
	{
		return mNumWrites;
	}
	size_t NumOpen()
	{
		return mNumOpen;
	}
	size_t NumOpenSuccess()
	{
		return mNumOpenSuccess;
	}
	size_t NumOpenFailure()
	{
		return mNumOpenFailure;
	}
	size_t NumClose()
	{
		return mNumClose;
	}

private:

	void DoOpeningClose();
	void DoOpen();
	void DoClose();
	void DoOpenSuccess()
	{
		++mNumOpenSuccess;
	}
	void DoOpenFailure()
	{
		++mNumOpenFailure;
	}

	void DoRead(openpal::WSlice& arBuffer)
	{
		mpWriteBuff = arBuffer;
		mNumToRead = arBuffer.Size();
	}

	void DoWrite(const openpal::RSlice& arBuffer)
	{
		mNumToWrite = arBuffer.Size();
		++mNumWrites;
		WriteToBuffer(arBuffer);
	}

	uint8_t* mpWriteBuff;

	uint32_t mNumToRead;
	uint32_t mNumToWrite;
	uint32_t mNumWrites;
	uint32_t mNumOpen;
	uint32_t mNumOpenSuccess;
	uint32_t mNumOpenFailure;
	uint32_t mNumClose;
	uint32_t mNumOpeningClose;

};

}

#endif
