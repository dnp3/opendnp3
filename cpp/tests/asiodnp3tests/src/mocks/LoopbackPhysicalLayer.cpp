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
#include "LoopbackPhysicalLayer.h"

#include <asio.hpp>

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

using namespace boost;

using namespace openpal;

namespace opendnp3
{

LoopbackPhysicalLayer::LoopbackPhysicalLayer(openpal::Logger logger, asio::io_service& service) :
	PhysicalLayerASIO(logger, service)
{

}

void LoopbackPhysicalLayer::DoOpen()
{
	//always open successfully
	auto lambda = [this]()
	{
		this->OnOpenCallback(std::error_code(0, std::generic_category()));
	};
	executor.PostLambda(lambda);
}

void LoopbackPhysicalLayer::DoOpenSuccess()
{

}

void LoopbackPhysicalLayer::DoClose()
{
	//empty any remaining written data
	mWritten.erase(mWritten.begin(), mWritten.end());

	// dispatch any pending reads with failures
	if(mBytesForReading.IsNotEmpty())
	{
		mBytesForReading.Clear();
		auto lambda = [this]()
		{
			this->OnReadCallback(std::error_code(1, std::generic_category()), nullptr, 0);
		};
		executor.PostLambda(lambda);
	}
}

void LoopbackPhysicalLayer::DoRead(openpal::WSlice& arBuffer)
{
	assert(mBytesForReading.IsEmpty());
	mBytesForReading = arBuffer;
	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayer::DoWrite(const openpal::RSlice& arBuffer)
{
	for(size_t i = 0; i < arBuffer.Size(); ++i) mWritten.push_back(arBuffer[i]);

	//always write successfully

	auto size = arBuffer.Size();

	auto lambda = [this, size]()
	{
		this->OnWriteCallback(std::error_code(0, std::generic_category()), size);
	};

	executor.PostLambda(lambda);

	//now check to see if this write will dispatch a read
	this->CheckForReadDispatch();
}

void LoopbackPhysicalLayer::CheckForReadDispatch()
{
	if(!mBytesForReading.IsEmpty() && mWritten.size() > 0)
	{
		uint32_t num = (mBytesForReading.Size() < mWritten.size()) ? mBytesForReading.Size() : mWritten.size();

		for(size_t i = 0; i < num; ++i)
		{
			mBytesForReading[i] = mWritten.front();
			mWritten.pop_front();
		}

		mBytesForReading.Clear();

		auto lambda = [this, num]()
		{
			this->OnReadCallback(std::error_code(0, std::generic_category()), mBytesForReading, num);
		};

		executor.PostLambda(lambda);
	}

}

}
