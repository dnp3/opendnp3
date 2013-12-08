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
#ifndef __LAZY_FIXED_SIZE_COLLECTION_H_
#define __LAZY_FIXED_SIZE_COLLECTION_H_

#include <functional>
#include <assert.h>

#include "LazyCollection.h"

namespace opendnp3
{

template <class T>
class LazyFixedSizeCollection : public LazyCollection<T>
{
	public:

		LazyFixedSizeCollection(const openpal::ReadOnlyBuffer& arBuffer, size_t aCount, const typename LazyIterator<T>::ReadFunction& aReadFunction):
			mBuffer(arBuffer),
			mCount(aCount),
			mReadFunction(aReadFunction)
		{
		
		}

		LazyIterator<T> begin() const
		{
			return LazyIterator<T>(mBuffer, mCount, mReadFunction);
		}
		
		LazyIterator<T> end() const
		{
			return LazyIterator<T>::End();
		}

		size_t size() const
		{
			return mCount;
		}

	private:
		
		LazyFixedSizeCollection();
		
		openpal::ReadOnlyBuffer mBuffer;		
		size_t mCount;
		typename LazyIterator<T>::ReadFunction mReadFunction;
};


}

#endif
