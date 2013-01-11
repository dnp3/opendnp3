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
#include "ShiftableBuffer.h"

#include "Exception.h"
#include "Location.h"

#include <memory.h>

namespace apl
{

ShiftableBuffer::ShiftableBuffer(size_t aSize) :
	mpBuffer(new uint8_t[aSize]),
	M_SIZE(aSize),
	mWritePos(0),
	mReadPos(0)
{
}


ShiftableBuffer::ShiftableBuffer( const uint8_t* aBuffer, size_t aSize) :
	mpBuffer(new uint8_t[aSize]),
	M_SIZE(aSize),
	mWritePos(0),
	mReadPos(0)
{
	if( aBuffer) {
		memcpy( mpBuffer, aBuffer, aSize) ;
		mWritePos += aSize ;
	}
}


void ShiftableBuffer::Shift()
{
	//copy all unread data to the front of the buffer
	memmove(mpBuffer, this->ReadBuff(), this->NumReadBytes());
	mWritePos = this->NumReadBytes();
	mReadPos = 0;
}

ShiftableBuffer::~ShiftableBuffer()
{
	delete[] mpBuffer;
}


void ShiftableBuffer::AdvanceRead(size_t aNumBytes)
{
	if(aNumBytes > this->NumReadBytes())
		throw ArgumentException(LOCATION, "Cannot be greater than the number of currently available reader bytes");

	mReadPos += aNumBytes;
}

void ShiftableBuffer::AdvanceWrite(size_t aNumBytes)
{

	if(aNumBytes > this->NumWriteBytes()) // This could indicate a buffer overflow
		throw ArgumentException(LOCATION, "Cannot be greater than the number of currently available writer bytes");

	mWritePos += aNumBytes;
}

bool ShiftableBuffer::Sync(const uint8_t* apPattern, size_t aNumBytes)
{
	if(aNumBytes < 1) throw ArgumentException(LOCATION, "Pattern must be at least 1 byte");

	size_t offset = SyncSubsequence(apPattern, aNumBytes, 0);
	bool res = (this->NumReadBytes() - offset) >= aNumBytes;
	if(offset > 0) this->AdvanceRead(offset);

	return res;
}

size_t ShiftableBuffer::SyncSubsequence(const uint8_t* apPattern, size_t aNumPatternBytes, size_t aOffset)
{
	size_t read_bytes = this->NumReadBytes() - aOffset;
	if(aNumPatternBytes > read_bytes) aNumPatternBytes = read_bytes;

	const uint8_t* pRead = this->ReadBuff() + aOffset;

	for(size_t i = 0; i < aNumPatternBytes; ++i) {
		if(apPattern[i] != pRead[i])
			return SyncSubsequence(apPattern, aNumPatternBytes, aOffset + 1);
	}

	return aOffset;
}

}
