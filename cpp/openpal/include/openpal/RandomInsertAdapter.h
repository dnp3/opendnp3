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
#ifndef __RANDOM_INSERT_ADAPTER_H_
#define __RANDOM_INSERT_ADAPTER_H_

#include "Indexable.h"
#include "StackAdapter.h"

namespace openpal
{

template <class ValueType>
class RandomInsertAdapter
{
	public:

		RandomInsertAdapter(Indexable<ValueType> aValues, Indexable<uint32_t> aIndices) : 
			values(aValues),
			availableIndices(aIndices)
		{
			assert(aValues.Size() == aIndices.Size());
			for(uint32_t i = 0; i < aValues.Size(); ++i) availableIndices.Push(i);
		}				

		inline ValueType& operator[](uint32_t index) 
		{
			assert(index < values.Size());
			return values[index];
		}		

		inline bool IsFull() const
		{
			return availableIndices.IsEmpty();
		}

		// put in a value and it tells you what index it was assigned
		// the value will never be overwritten
		uint32_t Add(const ValueType& value)
		{
			assert(!availableIndices.IsEmpty());
			auto index = availableIndices.Pop();
			values[index] = value;
			return index;
		}

		// This is really based on trust the index is actually being used		
		void Release(uint32_t index)
		{
			assert(index < values.Size());
			assert(!availableIndices.IsFull());
			availableIndices.Push(index);
		}
	
	private:
		Indexable<ValueType> values;
		StackAdapter<uint32_t> availableIndices;
		RandomInsertAdapter();
};

}

#endif
