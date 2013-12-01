
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

#include "LoopbackPhysicalLayerAsync.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <openpal/LoggableMacros.h>

using namespace boost;
using namespace boost::system;
using namespace openpal;

namespace opendnp3
{

LoopbackPhysicalLayerAsync::LoopbackPhysicalLayerAsync(openpal::Logger aLogger, boost::asio::io_service* apSrv) :
	PhysicalLayerAsyncASIO(aLogger, apSrv),
	mReadSize(0),
	mpReadBuff(NULL)
{

}

void LoopbackPhysicalLayerAsync::DoOpen()
{
	//always open successfully
	error_code ec(errc::success, get_generic_category());
	mExecutor.Post(bind(&LoopbackPhysicalLayerAsync::OnOpenCallback, this, ec));
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
		mExecutor.Post(bind(&LoopbackPhysicalLayerAsync::OnReadCallback, this, ec, mpReadBuff, 0));
	}
}

void LoopbackPhysicalLayerAsync::DoAsyncRead(openpal::WriteBuffer& arBuffer)
{
	assert(mReadSize == 0);
	mReadSize = arBuffer.Size();
	mpReadBuff = arBuffer;

	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayerAsync::DoAsyncWrite(const openpal::ReadOnlyBuffer& arBuffer)
{
	for(size_t i = 0; i < arBuffer.Size(); ++i) mWritten.push_back(arBuffer[i]);

	//always write successfully
	error_code ec(errc::success, get_generic_category());
	mExecutor.Post(bind(&LoopbackPhysicalLayerAsync::OnWriteCallback, this, ec, arBuffer.Size()));

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
		mExecutor.Post(bind(&LoopbackPhysicalLayerAsync::OnReadCallback, this, ec, mpReadBuff, num));
	}

}

}
