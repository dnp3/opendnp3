
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
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "MockPhysicalLayerMonitor.h"

#include <boost/test/unit_test.hpp>

#include <APL/IPhysicalLayerAsync.h>
#include <APL/Logger.h>
#include <APL/LoggableMacros.h>
#include <APL/Util.h>
#include <APL/ToHex.h>

#include <iostream>
#include <sstream>

using namespace std::chrono;

namespace apl
{

MockPhysicalLayerMonitor::MockPhysicalLayerMonitor(
													Logger* apLogger, 
													IPhysicalLayerAsync* apPhys, 													
													high_resolution_clock::duration aMinOpenRetry,
													high_resolution_clock::duration aMaxOpenRetry
													) :
	Loggable(apLogger),
	PhysicalLayerMonitor(apLogger, apPhys, aMinOpenRetry, aMaxOpenRetry),
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
	mState.push(this->GetState());
	this->AddObserver(this);
}

void MockPhysicalLayerMonitor::OnStateChange(PhysicalLayerState aState)
{
	mState.push(aState);
}

void MockPhysicalLayerMonitor::OnPhysicalLayerOpenSuccessCallback()
{
	mOpens++;
	mpPhys->AsyncRead(mReadBuffer, mReadBuffer.Size());
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
	mpPhys->AsyncRead(mReadBuffer, mReadBuffer.Size());
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

bool MockPhysicalLayerMonitor::NextStateIs(PhysicalLayerState aState)
{
	if(mState.empty()) return false;
	else {
		PhysicalLayerState state = mState.front();
		LOG_BLOCK(LEV_INFO, "Saw state: " + ConvertPhysicalLayerStateToString(state));
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
		size_t toWrite = apl::Min<size_t>(4096, remaining);
		mpPhys->AsyncWrite(mWriteBuffer.Buffer() + mBytesWritten, toWrite);
		this->mLastWriteSize = toWrite;
	}
}

}
