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

#include "LoopbackPhysicalLayerAsync.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <APL/Logger.h>
#include <APL/LoggableMacros.h>

using namespace boost;
using namespace boost::system;

namespace apl
{

LoopbackPhysicalLayerAsync::LoopbackPhysicalLayerAsync(Logger* apLogger, boost::asio::io_service* apSrv) :
	PhysicalLayerAsyncBase(apLogger),
	mpService(apSrv),
	mReadSize(0),
	mpReadBuff(NULL)
{

}

void LoopbackPhysicalLayerAsync::DoOpen()
{
	//always open successfully
	error_code ec(errc::success, get_generic_category());
	mpService->post(bind(&LoopbackPhysicalLayerAsync::OnOpenCallback, this, ec));
}

void LoopbackPhysicalLayerAsync::DoOpenSuccess()
{
	LOG_BLOCK(LEV_INFO, "Loopback Open Success");
}

void LoopbackPhysicalLayerAsync::DoClose()
{
	//empty any remaining written data
	mWritten.erase(mWritten.begin(), mWritten.end());

	//dispatch any pending reads with failures
	if(mReadSize > 0) {
		mReadSize = 0;
		error_code ec(errc::permission_denied, get_generic_category());
		mpService->post(bind(&LoopbackPhysicalLayerAsync::OnReadCallback, this, ec, mpReadBuff, 0));
	}
}

void LoopbackPhysicalLayerAsync::DoAsyncRead(uint8_t* apBuff, size_t aNumBytes)
{
	assert(mReadSize == 0);
	mReadSize = aNumBytes;
	mpReadBuff = apBuff;

	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayerAsync::DoAsyncWrite(const uint8_t* apData, size_t aNumBytes)
{
	for(size_t i = 0; i < aNumBytes; ++i) mWritten.push_back(apData[i]);

	//always write successfully
	error_code ec(errc::success, get_generic_category());
	mpService->post(bind(&LoopbackPhysicalLayerAsync::OnWriteCallback, this, ec, aNumBytes));

	//now check to see if this write will dispatch a read
	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayerAsync::CheckForReadDispatch()
{
	if(mReadSize > 0 && mWritten.size() > 0) {
		size_t num = (mReadSize < mWritten.size()) ? mReadSize : mWritten.size();

		for(size_t i = 0; i < num; ++i) {
			mpReadBuff[i] = mWritten.front();
			mWritten.pop_front();
		}

		mReadSize = 0;

		error_code ec(errc::success, get_generic_category());
		mpService->post(bind(&LoopbackPhysicalLayerAsync::OnReadCallback, this, ec, mpReadBuff, num));
	}

}

}
