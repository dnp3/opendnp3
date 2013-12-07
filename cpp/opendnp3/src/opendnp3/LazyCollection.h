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
#ifndef __LAZY_COLLECTION_H_
#define __LAZY_COLLECTION_H_

#include <functional>
#include <assert.h>

#include "BufferIterator.h"

namespace opendnp3
{

template <class T>
class LazyCollection
{
	public:

		typedef std::function<T (openpal::ReadOnlyBuffer&)> ReadFromBuffer;

		class const_iterator : private BufferIterator, public std::iterator<std::output_iterator_tag, T> 
		{
			
			ReadFromBuffer mReadType;
			
			public:
		
			const_iterator(const openpal::ReadOnlyBuffer& arBuffer, size_t aNumValues, const ReadFromBuffer& aReadType):
				BufferIterator(arBuffer, aNumValues),
				mReadType(aReadType)
			{
				
			}
				
			bool operator !=(const const_iterator& rhs)
			{
				return mCount < mNumValues;
			}

			void operator ++()
			{
				assert(mCount < mNumValues);
				mCount += 1;
			}

			T operator *()
			{
				return mReadType(mBuffer);
			}

			const_iterator(const const_iterator& rhs) : 
				BufferIterator(rhs),
				mReadType(rhs.mReadType)				
			{
				
			}			 
		};


		LazyCollection(const openpal::ReadOnlyBuffer& arBuffer, size_t aCount, const ReadFromBuffer& aReadType):
			mBuffer(arBuffer),
			mCount(aCount),
			mConvert(aReadType)			
		{
		
		}

		const_iterator begin() const
		{
			return const_iterator(mBuffer, mCount, mConvert);
		}
		
		const_iterator end() const
		{
			return const_iterator(mBuffer, 0, mConvert);
		}

		size_t Size()
		{
			return mCount;
		}

	private:
		
		LazyCollection();
		LazyCollection(const LazyCollection&);

		openpal::ReadOnlyBuffer mBuffer;		
		size_t mCount;
		ReadFromBuffer mConvert;
};


}

#endif
