#ifndef __BUFFER_SET_TYPES_H_
#define __BUFFER_SET_TYPES_H_

#include <openpal/Visibility.h>

#include <map>
#include <set>
#include <vector>
#include <limits>

namespace opendnp3
{

// C++ doesn't allow templated typedefs, but this technique simulates this behavior

// Set that forces data exclusivity by index
template <class T>
struct DLL_LOCAL IndexSet {
	struct LessThanByIndex {
		// Const to fix VS compilation bug
		bool operator()(const T& a, const T& b) const {
			return a.mIndex < b.mIndex;
		}
	};
	typedef std::set< T, LessThanByIndex > Type;
};

//  Multiset that orders data by order by timestamp, multi-entries allowed
template <class T>
struct DLL_LOCAL TimeMultiSet {
	struct LessThanByTime {
		bool operator()(const T& a, const T& b) const {
			return a.mValue.GetTime() < b.mValue.GetTime();
		}
	};

	typedef std::multiset<T, LessThanByTime > Type;
};

/** Sorts events by the order in which they are inserted.
	The last event value has been hijacked here for use with VTO.
*/
template <class T>
struct DLL_LOCAL InsertionOrderSet {
	struct InsertionOrder {
		bool operator()(const T& a, const T& b) const {
			return a.mSequence < b.mSequence;
		}
	};

	typedef std::set<T, InsertionOrder > Type;
};


} //end NS

#endif
