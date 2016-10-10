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
#include <catch.hpp>

#include <asiodnp3/PhysicalLayerMonitor.h>
#include <testlib/CopyableBuffer.h>

#include <queue>

using namespace testlib;

namespace opendnp3
{

class MockPhysicalLayerMonitor : public asiodnp3::PhysicalLayerMonitor
{
public:
	MockPhysicalLayerMonitor(
	    openpal::Logger logger,
	    openpal::IExecutor& executor,
	    openpal::IPhysicalLayer* apPhys,
	    openpal::TimeDuration aMinOpenRetry,
	    openpal::TimeDuration aMaxOpenRetry
	);

	uint32_t mOpens;
	uint32_t mCloses;

	uint32_t mNumReads;
	uint32_t mBytesRead;
	uint32_t mBytesWritten;
	uint32_t mLastWriteSize;

	CopyableBuffer mReadBuffer;
	CopyableBuffer mWSlice;
	CopyableBuffer mExpectReadBuffer;

	void OnPhysicalLayerOpenSuccessCallback();
	void OnPhysicalLayerOpenFailureCallback() {}
	void OnPhysicalLayerCloseCallback();

	void _OnReceive(const uint8_t* apData, size_t aNumBytes);
	void _OnSendSuccess(void);
	void _OnSendFailure(void);

	void WriteData(const CopyableBuffer& arData);
	void ExpectData(const CopyableBuffer& arData);

	bool NextStateIs(ChannelState aState);
	bool AllExpectedDataHasBeenReceived();

protected:
	void OnStateChange(ChannelState);
	std::queue< ChannelState > mState;

private:

	void TransmitNext();

};

}
