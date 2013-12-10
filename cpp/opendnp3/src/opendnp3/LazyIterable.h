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
#ifndef __LAZY_ITERABLE_H_
#define __LAZY_ITERABLE_H_

#include <functional>
#include <assert.h>

#include <openpal/BufferWrapper.h>

#include "Iterator.h"
#include "Iterable.h"

namespace opendnp3
{

template <class T>
class LazyIterable : public Iterable<T>
{
	public:
		
		LazyIterable(const openpal::ReadOnlyBuffer& arBuffer, size_t aCount, const typename Iterator<T>::ReadFunction& aReadFunction):
			mBuffer(arBuffer),
			mCount(aCount),
			mReadFunction(aReadFunction)
		{
		
		}

		Iterator<T> GetIterator() const
		{
			return Iterator<T>(mBuffer, mCount, mReadFunction);
		}
		

		size_t Size() const
		{
			return mCount;
		}

	private:
		
		LazyIterable();
		
		openpal::ReadOnlyBuffer mBuffer;		
		size_t mCount;
		typename Iterator<T>::ReadFunction mReadFunction;
};


}

#endif
