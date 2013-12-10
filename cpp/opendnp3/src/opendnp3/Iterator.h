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
#ifndef __OPENDNP3_ITERATOR_H_
#define __OPENDNP3_ITERATOR_H_

#include <functional>
#include <assert.h>

#include <openpal/BufferWrapper.h>

#include "BufferRange.h"

namespace opendnp3
{

template <class T>
class Iterator : private BufferRange
{						
	public:

	typedef std::function<T (openpal::ReadOnlyBuffer& buffer, size_t position)> ReadFunction;
		
	Iterator(const openpal::ReadOnlyBuffer& arBuffer, size_t aNumValues, const ReadFunction& aReadFunction):
		BufferRange(arBuffer, aNumValues),
		mReadFunction(aReadFunction)
	{}

	Iterator(const Iterator& rhs) : BufferRange(rhs), mReadFunction(rhs.mReadFunction)
	{}		

	bool HasNext() const
	{
		return mPosition < mNumValues;		
	}

	T Next()
	{			
		assert(mPosition < mNumValues);
		size_t pos = mPosition++;
		return mReadFunction(mBuffer, pos);
	}

	private:		 
	Iterator();	

	ReadFunction mReadFunction;
};

}

#endif
