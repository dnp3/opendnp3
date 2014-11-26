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
#ifndef OPENPAL_ARRAYVIEWITERATOR_H
#define OPENPAL_ARRAYVIEWITERATOR_H

#include <assert.h>

namespace openpal
{

template <class ValueType, class IndexType>
class ArrayView;

template <class ValueType, class IndexType>
class ArrayViewIterator
{
public:

	ArrayViewIterator(const ArrayView<ValueType, IndexType>& view_, IndexType position_, IndexType stop_) :
		empty(false),
		view(view_),
		position(position_),
		stop(stop_)
	{}

	ArrayViewIterator(const ArrayView<ValueType, IndexType>& view_) :
		empty(view_.IsEmpty()),
		view(view_),
		position(0),
		stop(view.IsEmpty() ? 0 : view.Size() - 1)
	{}

	inline const IndexType Index() const
	{
		assert(!empty);
		return position;
	}

	inline const ValueType& Value() const
	{
		assert(!empty);
		return indexable[position];
	}

	inline const IndexType Stop() const
	{
		return stop;
	}

	inline bool IsNotEmpty() const
	{
		return !empty;
	}

	inline bool IsEmpty() const
	{
		return empty;
	}

	inline void Next()
	{
		assert(!empty);
		if(position == stop)
		{
			empty = true;
		}
		else
		{
			++position;
		}
	}

private:

	bool empty;
	ArrayView<ValueType, IndexType> view;
	IndexType position;
	IndexType stop;
};

}

#endif
