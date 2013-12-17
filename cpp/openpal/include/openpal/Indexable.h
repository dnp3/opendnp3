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
#ifndef __INDEXABLE_H_
#define __INDEXABLE_H_

#include "HasSize.h"
#include "IndexableIterator.h"

namespace openpal
{

/**
* Acts as a functional facade around a buffer of a certain type
*/
template <class T>
class Indexable : public HasSize 
{
	
	public:

		static Indexable Empty()
		{
			return Indexable(nullptr, 0);
		}

		Indexable(T* start, uint32_t aSize) : HasSize(aSize), buffer(start)
		{}		

		inline const bool Contains(uint32_t index) const 
		{
			return index < size;
		}

		inline const bool Contains(uint32_t start, uint32_t stop) const 
		{ 
			return (start < stop) && Contains(stop);
		}

		IndexableIterator<T> Range(uint32_t start, uint32_t stop) const 
		{
			if(Contain(start, stop)) return IndexableIterator<T>(this, start, stop);
			else return IndexableIterator<T>(Empty());			 		
		}

		IndexableIterator<T> FullRange() const 
		{			
			return IndexableIterator<T>(*this); 
		}

		inline T& operator[](uint32_t index) 
		{
			assert(index < size);
			return buffer[index];
		}

		const T& operator[](uint32_t index) const
		{ 
			assert(index < size);
			return buffer[index];
		}

		template <class Action>
		void foreach(const Action& action)
		{
			for(uint32_t i = 0; i < size; ++i) action(buffer[i]);
		}

		template <class Action>
		void foreachIndex(const Action& action)
		{
			for(uint32_t i = 0; i < size; ++i) action(buffer[i], i);
		}	

		private:
		T* buffer;
};



}

#endif
