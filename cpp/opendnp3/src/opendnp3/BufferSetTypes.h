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
#ifndef __BUFFER_SET_TYPES_H_
#define __BUFFER_SET_TYPES_H_



#include <map>
#include <set>
#include <vector>
#include <limits>

namespace opendnp3
{

// C++ doesn't allow templated typedefs, but this technique simulates this behavior

// Set that forces data exclusivity by index
template <class T>
struct IndexSet {
	struct LessThanByIndex {
		// Const to fix VS compilation bug
		bool operator()(const T& a, const T& b) const {
			return a.index < b.index;
		}
	};
	typedef std::set< T, LessThanByIndex > Type;
};

//  Multiset that orders data by order by timestamp, multi-entries allowed
template <class T>
struct TimeMultiSet {
	struct LessThanByTime {
		bool operator()(const T& a, const T& b) const {
			return a.value.GetTime() < b.value.GetTime();
		}
	};

	typedef std::multiset<T, LessThanByTime > Type;
};

/** Sorts events by the order in which they are inserted.
	The last event value has been hijacked here for use with VTO.
*/
template <class T>
struct InsertionOrderSet {
	struct InsertionOrder {
		bool operator()(const T& a, const T& b) const {
			return a.mSequence < b.mSequence;
		}
	};

	typedef std::set<T, InsertionOrder > Type;
};


} //end NS

#endif
