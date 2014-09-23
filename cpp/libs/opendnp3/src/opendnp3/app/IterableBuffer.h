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
#ifndef OPENDNP3_ITERABLEBUFFER_H
#define OPENDNP3_ITERABLEBUFFER_H

#include <openpal/container/ReadOnlyBuffer.h>

#include <assert.h>

namespace opendnp3
{

class BufferWithCount
{
	template <class T> friend class IterableTransforms;

protected:

	BufferWithCount(const openpal::ReadOnlyBuffer& aBuffer, uint32_t aCount) :
		count(aCount),
		buffer(aBuffer)
	{}

public:

	uint32_t Count() const
	{
		return count;
	}
	bool IsEmpty() const
	{
		return count == 0;
	}
	bool NonEmpty() const
	{
		return count != 0;
	}

protected:

	uint32_t count;
	openpal::ReadOnlyBuffer buffer;
};

template <class T>
class IterableBuffer : public BufferWithCount
{
public:

	class Iterator
	{
		public:
			Iterator(const IterableBuffer& ib) : pos(0), copy(ib.buffer), pBuffer(&ib)
			{}

			void Reset()
			{
				pos = 0;
			}

			bool MoveNext()
			{
				if ((pos+1) < pBuffer->Count())
				{
					++pos;
					return true;
				}
				else
				{
					return false;
				}			
			}

			T Current()
			{				
				return pBuffer->ValueAt(copy, pos);
			}

		private:
			uint32_t pos;
			openpal::ReadOnlyBuffer copy;
			const IterableBuffer* pBuffer;
	};

	friend class Iterator;

	template <class A, class B, class MapToU> friend class MappedIterableBuffer;


	IterableBuffer(const openpal::ReadOnlyBuffer& aBuffer, uint32_t aSize) : BufferWithCount(aBuffer, aSize)
	{}

	Iterator Iterate() const
	{
		return Iterator(*this);
	}

	bool ReadOnlyValue(T& value) const
	{
		if(count == 1)
		{
			openpal::ReadOnlyBuffer copy(this->buffer);
			value = ValueAt(copy, 0);
			return true;
		}
		else return false;
	}	
	
	template <class IterFunc>
	void foreach(const IterFunc& fun) const
	{
		openpal::ReadOnlyBuffer copy(this->buffer);  // iterate over a mutable copy of the buffer
		for (uint32_t pos = 0; pos < count; ++pos)
		{
			fun(ValueAt(copy, pos));
		}
	}

protected:

	virtual T ValueAt(openpal::ReadOnlyBuffer&, uint32_t pos) const = 0;
};


}

#endif
