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
#include <openpal/BufferWrapper.h>

#include <cstring>
#include <assert.h>

namespace openpal
{
	HasSize::HasSize(size_t aSize) : mSize(aSize)
	{}

	size_t HasSize::Size() const
	{
		return mSize;
	}

	bool HasSize::IsNotEmpty() const
	{
		return mSize > 0;
	}

	bool HasSize::IsEmpty() const
	{
		return mSize == 0;
	}

	ReadOnlyBuffer::ReadOnlyBuffer(): HasSize(0), mpBuffer(nullptr)
	{
	
	}

	ReadOnlyBuffer::ReadOnlyBuffer(uint8_t const* apBuffer, size_t aSize) :
		HasSize(aSize),
		mpBuffer(apBuffer)
	{}

	void ReadOnlyBuffer::CopyTo(uint8_t* apDest) const
	{
		memcpy(apDest, mpBuffer, mSize);
	}

	void ReadOnlyBuffer::Advance(size_t aNum)
	{
		assert(aNum <= mSize);
		mpBuffer += aNum;
		mSize -= aNum;
	}

	WriteBuffer::WriteBuffer(): HasSize(0), mpBuffer(nullptr)
	{
		
	}

	WriteBuffer::WriteBuffer(uint8_t* apBuffer, size_t aSize) :
		HasSize(aSize),
		mpBuffer(apBuffer)
	{}

	void WriteBuffer::Clear()
	{
		mpBuffer = nullptr;
		mSize = 0;
	}

	void WriteBuffer::Advance(size_t aNum)
	{
		assert(aNum <= mSize);
		mpBuffer += aNum;
		mSize -= aNum;
	}

	ReadOnlyBuffer WriteBuffer::ToReadOnly() const
	{
		return ReadOnlyBuffer(mpBuffer, mSize);
	}

}


