
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

#include "MockPhysicalLayerMonitor.h"

#include <boost/test/unit_test.hpp>

#include <openpal/LoggableMacros.h>
#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/ToHex.h>

#include <opendnp3/Util.h>

#include <sstream>

using namespace openpal;
using namespace std::chrono;

namespace opendnp3
{

MockPhysicalLayerMonitor::MockPhysicalLayerMonitor(
        openpal::Logger& arLogger,
        IPhysicalLayerAsync* apPhys,
        TimeDuration aMinOpenRetry,
        TimeDuration aMaxOpenRetry
) :
	Loggable(arLogger),
	PhysicalLayerMonitor(arLogger, apPhys, aMinOpenRetry, aMaxOpenRetry),
	mOpens(0),
	mCloses(0),
	mNumReads(0),
	mBytesRead(0),
	mBytesWritten(0),
	mLastWriteSize(0),
	mReadBuffer(512),
	mWriteBuffer(0),
	mExpectReadBuffer(0)
{


}

void MockPhysicalLayerMonitor::OnPhysicalLayerOpenSuccessCallback()
{
	mOpens++;
	WriteBuffer buffer(mReadBuffer, mReadBuffer.Size());
	mpPhys->AsyncRead(buffer);
}

void MockPhysicalLayerMonitor::OnPhysicalLayerCloseCallback()
{
	mCloses++;
}

void MockPhysicalLayerMonitor::_OnReceive(const uint8_t* apData, size_t aNumBytes)
{
	++mNumReads;
	// we should never receive more than we're expecting
	if(mExpectReadBuffer.Size() < mBytesRead + aNumBytes) {
		BOOST_FAIL("Read more data than expected");
	}
	CopyableBuffer expecting(mExpectReadBuffer.Buffer() + mBytesRead, aNumBytes);
	CopyableBuffer read(apData, aNumBytes);
	// check that we're receiving what was written
	if(expecting != read) {
		std::ostringstream oss;
		oss << "Data corruption on receive, " << read << " != " << expecting;
		BOOST_FAIL(oss.str());
	}
	mBytesRead += aNumBytes;
	LOG_BLOCK(LEV_INFO, "Received " << mBytesRead << " of " << mExpectReadBuffer.Size());
	WriteBuffer buffer(mReadBuffer, mReadBuffer.Size());
	mpPhys->AsyncRead(buffer);
}

void MockPhysicalLayerMonitor::ExpectData(const CopyableBuffer& arData)
{
	mBytesRead = 0;
	mNumReads = 0;
	mExpectReadBuffer = arData;
}

void MockPhysicalLayerMonitor::WriteData(const CopyableBuffer& arData)
{
	BOOST_REQUIRE(mpPhys->CanWrite());
	mBytesWritten = 0;
	mWriteBuffer = arData;
	this->TransmitNext();
}

void MockPhysicalLayerMonitor::_OnSendSuccess(void)
{
	this->mBytesWritten += this->mLastWriteSize;
	this->TransmitNext();
}

void MockPhysicalLayerMonitor::_OnSendFailure(void)
{
	BOOST_REQUIRE(false);
}

void MockPhysicalLayerMonitor::OnStateChange(ChannelState aState)
{
	this->mState.push(aState);
}

bool MockPhysicalLayerMonitor::NextStateIs(ChannelState aState)
{
	if(mState.empty()) return false;
	else {
		ChannelState state = mState.front();
		LOG_BLOCK(LEV_INFO, "Saw state: " + ConvertChannelStateToString(state));
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
	if(mWriteBuffer.Size() > this->mBytesWritten) {
		size_t remaining = mWriteBuffer.Size() - mBytesWritten;
		size_t toWrite = Min<size_t>(4096, remaining);
		mpPhys->AsyncWrite(ReadOnlyBuffer(mWriteBuffer.Buffer() + mBytesWritten, toWrite));
		this->mLastWriteSize = toWrite;
	}
}

}
