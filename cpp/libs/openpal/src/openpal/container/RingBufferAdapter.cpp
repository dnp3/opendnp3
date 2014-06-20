/**
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

#include "RingBufferAdapter.h"

#include "openpal/WriteBuffer.h"
#include "openpal/ReadOnlyBuffer.h"

namespace openpal
{

RingBufferAdapter::RingBufferAdapter(uint8_t* pBuffer_, uint32_t size) : 
	pBuffer(pBuffer_),
	capacity(size),
	start(0),
	nextWrite(0),
	count(0)
{}

void RingBufferAdapter::Put(uint8_t value)
{
	pBuffer[nextWrite] = value;
	nextWrite = (nextWrite + 1) % capacity;
	if (count == capacity) // overflow, overwrite oldest value
	{
		start = (start + 1) % capacity;
	}
	else
	{
		++count;
	}
}

uint32_t RingBufferAdapter::Read(openpal::WriteBuffer& output)
{
	auto numToRead = output.Size() > count ? count : output.Size();
	
	// the number of bytes on the tail of the buffer
	auto tailSize = capacity - start;
	
	if (tailSize >= numToRead) // data is contiguous, only one read is required
	{
		output.ReadFrom(ReadOnlyBuffer(pBuffer + start, numToRead));
	}
	else // data wraps around, perform 2 reads
	{
		// read the first part from the end of buffer
		output.ReadFrom(ReadOnlyBuffer(pBuffer + start, tailSize));
		// read the remainder from the front of buffer
		output.ReadFrom(ReadOnlyBuffer(pBuffer, numToRead - tailSize));
	}

	start = (start + numToRead) % capacity;
	count -= numToRead;
	return numToRead;
}

}


