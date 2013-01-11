
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
#include "PhysicalLayerSyncProxy.h"

#include <boost/asio.hpp>
#include <iostream>

using namespace std;

namespace apl
{

const boost::system::error_code PhysicalLayerSyncProxy::mSuccessCode(boost::system::errc::success, boost::system::get_generic_category());
const boost::system::error_code PhysicalLayerSyncProxy::mErrorCode(boost::system::errc::permission_denied, boost::system::get_generic_category());

PhysicalLayerSyncProxy::PhysicalLayerSyncProxy(Logger* apLogger, boost::asio::io_service* apService) :
	PhysicalLayerAsyncASIO(apLogger, apService),	
	mThread(this)
{
	this->Reset();
	mThread.Start();
}

void PhysicalLayerSyncProxy::SignalStop()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mCondition.notify_all();
}

void PhysicalLayerSyncProxy::Reset()
{
	mReading = false;
	mpBuffer = NULL;
	mNumToRead = 0;
	mLineQueue.clear();
}

void PhysicalLayerSyncProxy::CheckForRead()
{
	if(mReading && mLineQueue.size() > 0) {
		std::string copy(mLineQueue.front());
		mLineQueue.pop_front();
		size_t len = copy.size();
		size_t min = len > mNumToRead ? mNumToRead : len;
		size_t remainder = len - min;
		memcpy(mpBuffer, copy.c_str(), min);
		if(remainder > 0) {
			std::string r(copy.c_str() + mNumToRead, remainder);
			mLineQueue.push_front(r);
		}
		mReading = false;
		mStrand.post(std::bind(&PhysicalLayerSyncProxy::OnReadCallback, this, mSuccessCode, mpBuffer, min));
	}
}

void PhysicalLayerSyncProxy::DoOpen()
{
	this->Reset();
	mStrand.post(std::bind(&PhysicalLayerSyncProxy::OnOpenCallback, this, mSuccessCode));
}

void PhysicalLayerSyncProxy::DoClose()
{

}

void PhysicalLayerSyncProxy::DoAsyncRead(uint8_t* apData, size_t aLength)
{
	std::unique_lock<std::mutex> lock(mMutex);
	mpBuffer = apData;
	mNumToRead = aLength;
	mReading = true;
	this->CheckForRead();
	if(mReading) mCondition.notify_all(); //wake up the thread to go and get more data from stdin
}

void PhysicalLayerSyncProxy::DoAsyncWrite(const uint8_t* apData, size_t aLength)
{
	const char* pBuff = reinterpret_cast<const char*>(apData);
	string s(pBuff, aLength);
	this->Write(s);
	mStrand.post(std::bind(&PhysicalLayerSyncProxy::OnWriteCallback, this, mSuccessCode, aLength));
}

void PhysicalLayerSyncProxy::Run()
{
	while(!IsExitRequested()) {
		std::unique_lock<std::mutex> lock(mMutex);
		if( !mReading ) mCondition.wait(lock);
		if( mReading ) {
			mLineQueue.push_back(this->Read());
			this->CheckForRead();
		}
	}
}
}
