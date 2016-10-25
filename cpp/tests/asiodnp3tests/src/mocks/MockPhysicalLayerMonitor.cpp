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
#include "MockPhysicalLayerMonitor.h"

#include <catch.hpp>

#include <openpal/logging/LogMacros.h>
#include <openpal/channel/IPhysicalLayer.h>
#include <openpal/util/ToHex.h>
#include <openpal/util/Comparisons.h>

#include <opendnp3/LogLevels.h>

#include <sstream>

using namespace openpal;

namespace opendnp3
{

MockPhysicalLayerMonitor::MockPhysicalLayerMonitor(
    openpal::Logger logger,
    openpal::IExecutor& executor,
    IPhysicalLayer* apPhys,
    TimeDuration aMinOpenRetry,
    TimeDuration aMaxOpenRetry
) :
	PhysicalLayerMonitor(logger, executor, apPhys, ChannelRetry(aMinOpenRetry, aMaxOpenRetry)),
	mOpens(0),
	mCloses(0),
	mNumReads(0),
	mBytesRead(0),
	mBytesWritten(0),
	mLastWriteSize(0),
	mReadBuffer(512),
	mWSlice(0),
	mExpectReadBuffer(0)
{


}

void MockPhysicalLayerMonitor::OnPhysicalLayerOpenSuccessCallback()
{
	mOpens++;
	WSlice buffer(mReadBuffer, mReadBuffer.Size());
	pPhys->BeginRead(buffer);
}

void MockPhysicalLayerMonitor::OnPhysicalLayerCloseCallback()
{
	mCloses++;
}

void MockPhysicalLayerMonitor::_OnReceive(const uint8_t* apData, size_t aNumBytes)
{
	++mNumReads;
	// we should never receive more than we're expecting
	if(mExpectReadBuffer.Size() < mBytesRead + aNumBytes)
	{
		throw std::invalid_argument("Read more data than expected");
	}
	CopyableBuffer expecting(mExpectReadBuffer + mBytesRead, static_cast<uint32_t>(aNumBytes));
	CopyableBuffer read(apData, static_cast<uint32_t>(aNumBytes));
	// check that we're receiving what was written
	if(expecting != read)
	{
		std::ostringstream oss;
		oss << "Data corruption on receive, " << read << " != " << expecting;
		throw std::invalid_argument(oss.str());
	}
	mBytesRead += static_cast<uint32_t>(aNumBytes);
	WSlice buffer(mReadBuffer, mReadBuffer.Size());
	pPhys->BeginRead(buffer);
}

void MockPhysicalLayerMonitor::ExpectData(const CopyableBuffer& arData)
{
	mBytesRead = 0;
	mNumReads = 0;
	mExpectReadBuffer = arData;
}

void MockPhysicalLayerMonitor::WriteData(const CopyableBuffer& arData)
{
	REQUIRE(pPhys->CanWrite());
	mBytesWritten = 0;
	mWSlice = arData;
	this->TransmitNext();
}

void MockPhysicalLayerMonitor::_OnSendSuccess(void)
{
	this->mBytesWritten += this->mLastWriteSize;
	this->TransmitNext();
}

void MockPhysicalLayerMonitor::_OnSendFailure(void)
{
	REQUIRE(false);
}

void MockPhysicalLayerMonitor::OnStateChange(ChannelState aState)
{
	this->mState.push(aState);
}

bool MockPhysicalLayerMonitor::NextStateIs(ChannelState aState)
{
	if(mState.empty()) return false;
	else
	{
		ChannelState state = mState.front();
		mState.pop();
		return (state == aState);
	}
}

bool MockPhysicalLayerMonitor::AllExpectedDataHasBeenReceived()
{
	return mBytesRead == mExpectReadBuffer.Size();
}

void MockPhysicalLayerMonitor::TransmitNext()
{
	if(mWSlice.Size() > this->mBytesWritten)
	{
		uint32_t remaining = mWSlice.Size() - mBytesWritten;
		uint32_t toWrite = Min<uint32_t>(4096, remaining);
		RSlice buff(mWSlice + mBytesWritten, toWrite);
		pPhys->BeginWrite(buff);
		this->mLastWriteSize = toWrite;
	}
}

}
