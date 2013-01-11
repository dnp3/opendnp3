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
#include "CopyableBuffer.h"

#include <memory.h>

#include "ToHex.h"

namespace apl
{

std::ostream& operator<<(std::ostream& output, const CopyableBuffer& arBuff)
{
	output << "[" << toHex(arBuff.Buffer(), arBuff.Size(), true) << "]";
	return output;
}

CopyableBuffer::CopyableBuffer() :
	mpBuff(NULL),
	mSize(0)
{

}

CopyableBuffer::CopyableBuffer(size_t aSize) :
	mpBuff(new uint8_t[aSize]),
	mSize(aSize)
{
	this->Zero();
}

CopyableBuffer::CopyableBuffer(const uint8_t* apData, size_t aSize) :
	mpBuff(new uint8_t[aSize]),
	mSize(aSize)
{
	memcpy(mpBuff, apData, mSize);
}

CopyableBuffer::CopyableBuffer(const CopyableBuffer& arBuffer) :
	mpBuff(new uint8_t[arBuffer.Size()]),
	mSize(arBuffer.Size())
{
	memcpy(mpBuff, arBuffer, mSize);
}

void CopyableBuffer::Zero()
{
	memset(mpBuff, 0, mSize);
}

CopyableBuffer& CopyableBuffer::operator=(const CopyableBuffer& arRHS)
{
	//check for assignment to self
	if(this == &arRHS) return *this;

	if(arRHS.Size() != mSize) {
		mSize = arRHS.Size();
		delete mpBuff;
		mpBuff = new uint8_t[mSize];
	}

	memcpy(mpBuff, arRHS, mSize);

	return *this;
}

CopyableBuffer::~CopyableBuffer()
{
	delete [] mpBuff;
}

bool CopyableBuffer::operator==( const CopyableBuffer& other) const
{
	if(other.Size() != this->Size()) return false;
	else {
		for(size_t i = 0; i < this->Size(); ++i) {
			if(this->mpBuff[i] != other.mpBuff[i]) return false;
		}

		return true;
	}
}

}
