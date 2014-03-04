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
#ifndef __STATIC_LIST_H_
#define __STATIC_LIST_H_

#include "ListAdapter.h"
#include "StaticArray.h"

namespace openpal
{

// References a fixed-size buffer somewhere, providing a list-like interface
// Gives the appearance of a list that can grow, but not shrink
template <class ValueType, class IndexType, IndexType N>
class StaticList : public ListAdapter<ValueType, IndexType>
{

	public:
		StaticList() : underlying(), ListAdapter<ValueType, IndexType>(underlying.ToIndexable())
		{}

	private:

		StaticList(const StaticList&);
		StaticList& operator= (const StaticList&);
		StaticArray<ValueType, IndexType, N> underlying;
};

}

#endif
