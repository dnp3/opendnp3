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
#ifndef __MOCK_PHYSICAL_LAYER_ASYNC_TS_H_
#define __MOCK_PHYSICAL_LAYER_ASYNC_TS_H_

#include <APL/PhysicalLayerAsyncBase.h>
#include <APL/ShiftableBuffer.h>
#include <APL/IHandlerAsync.h>

#include <boost/system/error_code.hpp>

#include "BufferTestObject.h"

namespace apl
{

class MockExecutor;
class ITimer;

// Provides a backend for testing physical layers
class MockPhysicalLayerAsyncTS : public PhysicalLayerAsyncBase, public BufferTestObject
{
public:
	MockPhysicalLayerAsyncTS(Logger*, MockExecutor*);

	void WriteToLayer(const uint8_t* apData, size_t aNumBytes);
	void Advance();

private:

	void DoOpen();
	void DoClose();
	void DoOpeningClose();
	void DoOpenSuccess();
	void DoOpenFailure();
	void DoAsyncRead(uint8_t* apBuff, size_t aNumBytes);
	void DoAsyncWrite(const uint8_t* apData, size_t aNumBytes);

	void Reset();
	void CheckForRead();

	MockExecutor* mpExecutor;
	ITimer* mpOpenTimer;
	boost::system::error_code mSuccessCode;
	boost::system::error_code mErrorCode;
	uint8_t* mpBuff;
	size_t mNumBytes;
	ShiftableBuffer mWriteBuffer;
};
}

#endif
