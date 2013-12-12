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

		typedef std::function<T (openpal::ReadOnlyBuffer&, uint32_t)> ReadFunction;
		
		LazyIterable(const openpal::ReadOnlyBuffer& aBuffer, uint32_t aCount, const ReadFunction& aReadBuffer):
			buffer(aBuffer),
			count(aCount),
			readBuffer(aReadBuffer)
		{
		
		}

		template <class U>
		LazyIterable<U> Map(const std::function<U (const T&)>& fun) const
		{
			auto convert = [fun, this](openpal::ReadOnlyBuffer& buff, uint32_t position) { 
				return fun(this->readBuffer(buff, position));
			};
			return LazyIterable<U>(buffer, count, convert);
		}

		Iterator<T> GetIterator() const
		{			
			openpal::ReadOnlyBuffer copy(buffer);
			auto convert = [copy, this](uint32_t index) mutable { return readBuffer(copy, index); };
			return Iterator<T>(count, convert);
		}
		

		uint32_t Size() const
		{
			return count;
		}

	private:
		
		LazyIterable();
		
		openpal::ReadOnlyBuffer buffer;		
		uint32_t count;
		ReadFunction readBuffer;
};

}

#endif
