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
#ifndef __INDEXABLE_ITERATOR_H_
#define __INDEXABLE_ITERATOR_H_

#include <assert.h>

namespace openpal
{

template <class T>
class Indexable;

template <class T>
class IndexableIterator
{
	public:

		IndexableIterator(Indexable<T> const* pIndexable, uint32_t aPosition, uint32_t aCount) :
			indexable(pIndexable),
			position(aPosition),
			count(aCount)
		{}

		inline const uint32_t Index() const 
		{ 
			assert(count > 0);
			return position; 
		}
		
		inline const T& Value() const 
		{
			assert(count > 0);
			return indexable->Get(position);
		}

		inline const uint32_t Stop() const
		{
			assert(count > 0);
			return position + count - 1;
		}
		
		inline bool IsNotEmpty() const { return count > 0; }

		inline bool IsEmpty() const { return count == 0; }

		inline void Next()
		{
			assert(count > 0);
			--count;
			++position;
		}
		

	private:

		Indexable<T> const * indexable;
		uint32_t position;
		uint32_t count;
};

}

#endif
