/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <boost/test/unit_test.hpp>

#include <APL/PhysicalLayerMonitor.h>
#include <APL/CopyableBuffer.h>

#include <queue>

namespace apl
{

class MockPhysicalLayerMonitor : public PhysicalLayerMonitor, public IPhysicalLayerObserver
{
public:
	MockPhysicalLayerMonitor(	
								Logger* apLogger, 
								IPhysicalLayerAsync* apPhys, 
								IExecutor* apTimer, 
								std::chrono::high_resolution_clock::duration aMinOpenRetry,
								std::chrono::high_resolution_clock::duration aMaxOpenRetry
							);

	size_t mOpens;
	size_t mCloses;

	size_t mNumReads;
	size_t mBytesRead;
	size_t mBytesWritten;
	size_t mLastWriteSize;

	CopyableBuffer mReadBuffer;
	CopyableBuffer mWriteBuffer;
	CopyableBuffer mExpectReadBuffer;

	void OnStateChange(PhysicalLayerState);
	std::queue< PhysicalLayerState > mState;

	void OnPhysicalLayerOpenSuccessCallback();
	void OnPhysicalLayerOpenFailureCallback() {}
	void OnPhysicalLayerCloseCallback();

	void _OnReceive(const uint8_t* apData, size_t aNumBytes);
	void _OnSendSuccess(void);
	void _OnSendFailure(void);

	void WriteData(const CopyableBuffer& arData);
	void ExpectData(const CopyableBuffer& arData);

	bool NextStateIs(PhysicalLayerState aState);
	bool AllExpectedDataHasBeenReceived();

private:

	void TransmitNext();

};

}