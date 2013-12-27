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
#ifndef __DYNAMIC_ARRAY_H_
#define __DYNAMIC_ARRAY_H_

#include "Indexable.h"

namespace openpal
{

template <class ValueType, class IndexType>
class DynamicArray : public HasSize<IndexType>
{

	public:

		DynamicArray(IndexType aSize) : 
			HasSize<IndexType>(aSize),
			buffer(new ValueType[aSize])
		{}
		
		Indexable<ValueType, IndexType> ToIndexable()
		{
			return Indexable<ValueType, IndexType>(buffer, size);
		}

		inline const bool Contains(IndexType index) const 
		{
			return index < size;
		}

		inline ValueType& operator[](IndexType index) 
		{
			assert(index < size);
			return buffer[index];
		}

		const ValueType& operator[](IndexType index) const
		{ 
			assert(index < size);
			return buffer[index];
		}

		template <class Action>
		void foreach(const Action& action) const
		{
			for(IndexType i = 0; i < size; ++i) action(buffer[i]);
		}

		template <class Action>
		void foreach(const Action& action)
		{
			for(IndexType i = 0; i < size; ++i) action(buffer[i]);
		}

		template <class Action>
		void foreachIndex(const Action& action)
		{
			for(IndexType i = 0; i < size; ++i) action(buffer[i], i);
		}	

		
		template <class Action>
		void foreachIndex(const Action& action) const
		{
			for(uint32_t i = 0; i < size; ++i) action(buffer[i], i);
		}	

		virtual ~DynamicArray()
		{
			delete[] buffer;
		}					
	
	private:
		ValueType* buffer;

		DynamicArray();
		DynamicArray(const DynamicArray&);
		DynamicArray& operator=(const DynamicArray&);		
};

}

#endif
