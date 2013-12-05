#ifndef __CLASS_COUNTER_H_
#define __CLASS_COUNTER_H_

#include <stddef.h>
#include <assert.h>

#include <opendnp3/PointClass.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

/** Utility class that keeps class event counters accessible by enumeration.
*/
class DLL_LOCAL ClassCounter
{
public:

	ClassCounter() : mNumClass1(0), mNumClass2(0), mNumClass3(0) {};
	~ClassCounter() {};

	inline size_t GetNum(PointClass aClass) {
		switch(aClass) {
		case(PC_CLASS_1):
			return mNumClass1;
		case(PC_CLASS_2):
			return mNumClass2;
		case(PC_CLASS_3):
			return mNumClass3;
		case(PC_ALL_EVENTS):
			return mNumClass1 + mNumClass2 + mNumClass3;
		default:
			return 0;
		}
	}

	size_t GetNumAllClass() {
		return mNumClass1 + mNumClass2 + mNumClass3;
	}

	void IncrCount(PointClass aClass);
	void DecrCount(PointClass aClass);

private:

	size_t mNumClass1;
	size_t mNumClass2;
	size_t mNumClass3;

};

}


#endif

