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
#ifndef OPENPAL_STACKADAPTER_H
#define OPENPAL_STACKADAPTER_H

#include "openpal/container/DynamicArray.h"

namespace openpal
{

// References a fixed-size buffer somewhere, providing a list-like interface
// Gives the appearance of a list that can grow, but not shrink
template <class ValueType, class IndexType>
class Stack : public HasSize<IndexType>
{

public:

	Stack(IndexType maxSize) : HasSize<IndexType>(0), underlying(maxSize)
	{}

	inline IndexType Capacity() const
	{
		return underlying.Size();
	}

	inline bool IsFull() const
	{
		return this->size == underlying.Size();
	}

	inline void Clear()
	{
		this->size = 0;
	}

	bool Push(const ValueType& value)
	{
		if(IsFull())
		{
			return false;
		}
		else
		{
			underlying[this->size] = value;
			++(this->size);
			return true;			
		}
	}

	ValueType Pop()
	{
		assert(this->IsNotEmpty());
		--(this->size);
		return underlying[this->size];
	}

private:
	DynamicArray<ValueType, IndexType> underlying;
	Stack();
};

}

#endif
