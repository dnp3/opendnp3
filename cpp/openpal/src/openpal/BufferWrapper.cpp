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
#include "BufferWrapper.h"

#include "Configure.h"
#include "Limits.h"

#include <cstring>
#include <assert.h>

namespace openpal
{

ReadOnlyBuffer ReadOnlyBuffer::Empty()
{
	return ReadOnlyBuffer();
}

ReadOnlyBuffer::ReadOnlyBuffer(): HasSize(0), pBuffer(nullptr)
{}

ReadOnlyBuffer::ReadOnlyBuffer(uint8_t const* pBuffer, uint32_t size) :
	HasSize(size),
	pBuffer(pBuffer)
{}

ReadOnlyBuffer ReadOnlyBuffer::CopyTo(WriteBuffer& dest) const
{
	if (dest.Size() < size)
	{
		return ReadOnlyBuffer::Empty();
	}
	else
	{
		memcpy(dest, pBuffer, size);
		return ReadOnlyBuffer(dest, size);
	}
}

ReadOnlyBuffer ReadOnlyBuffer::Take(uint32_t count) const
{	
	return ReadOnlyBuffer(pBuffer, openpal::MinimumOf(size, count));
}

ReadOnlyBuffer ReadOnlyBuffer::Skip(uint32_t count) const
{
	auto num = openpal::MinimumOf(size, count);
	return ReadOnlyBuffer(pBuffer + num, size - num);
}

void ReadOnlyBuffer::Clear()
{
	pBuffer = nullptr;
	size = 0;
}

bool ReadOnlyBuffer::Equals(const ReadOnlyBuffer& rhs) const
{
	if (this->Size() == rhs.Size())
	{
		return memcmp(pBuffer, rhs.pBuffer, Size()) == 0;
	}
	else
	{
		return false;
	}
}

void ReadOnlyBuffer::Advance(uint32_t count)
{
	auto num = openpal::MinimumOf(size, count);
	pBuffer += num;
	size -= num;
}

WriteBuffer WriteBuffer::Empty()
{
	return WriteBuffer();
}

WriteBuffer::WriteBuffer(const WriteBuffer& copy) : 
	HasSize(copy),
	mpBuffer(copy.mpBuffer)
{}

WriteBuffer::WriteBuffer(): 
	HasSize(0),
	mpBuffer(nullptr)
{}

WriteBuffer::WriteBuffer(uint8_t* apBuffer, uint32_t aSize) :
	HasSize(aSize),
	mpBuffer(apBuffer)
{}

uint32_t WriteBuffer::ReadFrom(const ReadOnlyBuffer& buffer)
{
	auto num = buffer.Size() > size ? size : buffer.Size();
	memcpy(mpBuffer, buffer, num);
	this->Advance(num);
	return num;
}

void WriteBuffer::Clear()
{
	mpBuffer = nullptr;
	size = 0;
}

WriteBuffer WriteBuffer::Truncate(uint32_t aNum) const
{
	assert(aNum <= size);
	return WriteBuffer(mpBuffer, aNum);
}

void WriteBuffer::Advance(uint32_t aNum)
{
	assert(aNum <= size);
	mpBuffer += aNum;
	size -= aNum;
}

ReadOnlyBuffer WriteBuffer::ToReadOnly() const
{
	return ReadOnlyBuffer(mpBuffer, size);
}

}


