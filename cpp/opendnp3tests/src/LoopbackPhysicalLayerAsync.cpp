
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

#include <openpal/LoggableMacros.h>

using namespace boost;
using namespace boost::system;
using namespace openpal;

namespace opendnp3
{

LoopbackPhysicalLayerAsync::LoopbackPhysicalLayerAsync(openpal::Logger aLogger, boost::asio::io_service* apSrv) :
	PhysicalLayerAsyncASIO(aLogger, apSrv)	
{

}

void LoopbackPhysicalLayerAsync::DoOpen()
{
	//always open successfully	
	mExecutor.Post([this](){ 
		this->OnOpenCallback(error_code(errc::success, get_generic_category())); 
	});				
}

void LoopbackPhysicalLayerAsync::DoOpenSuccess()
{
	LOG_BLOCK(LogLevel::Info, "Loopback Open Success");
}

void LoopbackPhysicalLayerAsync::DoClose()
{
	//empty any remaining written data
	mWritten.erase(mWritten.begin(), mWritten.end());

	// dispatch any pending reads with failures
	if(mBytesForReading.IsNotEmpty()) {		
		mBytesForReading.Clear();		
		mExecutor.Post([this](){ 
			this->OnReadCallback(error_code(errc::permission_denied, get_generic_category()), nullptr, 0);
		});						
	}
}

void LoopbackPhysicalLayerAsync::DoAsyncRead(openpal::WriteBuffer& arBuffer)
{
	assert(mBytesForReading.IsEmpty());
	mBytesForReading = arBuffer;	
	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayerAsync::DoAsyncWrite(const openpal::ReadOnlyBuffer& arBuffer)
{
	for(size_t i = 0; i < arBuffer.Size(); ++i) mWritten.push_back(arBuffer[i]);

	//always write successfully

	auto size = arBuffer.Size();
	
	mExecutor.Post([this, size](){
		this->OnWriteCallback(error_code(errc::success, get_generic_category()), size);		
	});

	//now check to see if this write will dispatch a read
	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayerAsync::CheckForReadDispatch()
{
	if(!mBytesForReading.IsEmpty() && mWritten.size() > 0) {
		size_t num = (mBytesForReading.Size() < mWritten.size()) ?mBytesForReading.Size() : mWritten.size();

		for(size_t i = 0; i < num; ++i) {
			mBytesForReading[i] = mWritten.front();
			mWritten.pop_front();
		}

		mBytesForReading.Clear();
		
		mExecutor.Post([this, num](){
			this->OnReadCallback(error_code(errc::success, get_generic_category()), mBytesForReading, num);
		});
	}

}

}
