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
#ifndef __QUEUE_ADAPTER_H_
#define __QUEUE_ADAPTER_H_

#include "Indexable.h"

namespace openpal
{

// References a fixed-size buffer somewhere, providing a queue-like interface
// that must occasionally be cleared to ever add more elements than it's fixed capacity
template <class ValueType, class IndexType>
class QueueAdapter
{

public:

	QueueAdapter(Indexable<ValueType, IndexType> indexable) :
		front(0),
		back(0),
		indexable(indexable)
	{}

	inline IndexType Size() const
	{
		return back - front;
	}

	inline IndexType Capacity() const
	{
		return indexable.Size();
	}

	inline bool IsNotEmpty() const
	{
		return front < back;
	}

	inline bool IsEmpty() const
	{
		return front == back;
	}

	inline bool IsFull() const
	{
		return back == Capacity();
	}

	inline void Clear()
	{
		front = back = 0;
	}

	bool Push(const ValueType& value)
	{
		if(!IsFull())
		{
			indexable[back] = value;
			++back;
			return true;
		}
		else return false;
	}

	ValueType& Front()
	{
		assert(IsNotEmpty());
		return indexable[front];
	}

	const ValueType& Front() const
	{
		assert(IsNotEmpty());
		return indexable[front];
	}

	ValueType Pop()
	{
		assert(IsNotEmpty());
		auto index = front;
		++front;
		return indexable[index];
	}

private:
	IndexType front;
	IndexType back;
	Indexable<ValueType, IndexType> indexable;
	QueueAdapter();
};

}

#endif
