#include "ClassCounter.h"


#include <assert.h>

namespace opendnp3
{

void ClassCounter::IncrCount(PointClass aClass)
{
	switch(aClass) {
	case(PC_CLASS_1):
		mNumClass1++;
		break;
	case(PC_CLASS_2):
		mNumClass2++;
		break;
	case(PC_CLASS_3):
		mNumClass3++;
		break;
	default:
		break;
	}
}

void ClassCounter::DecrCount(PointClass aClass)
{
	switch(aClass) {
	case(PC_CLASS_1):
		assert(mNumClass1 > 0);
		mNumClass1--;
		break;
	case(PC_CLASS_2):
		assert(mNumClass2 > 0);
		mNumClass2--;
		break;
	case(PC_CLASS_3):
		assert(mNumClass3 > 0);
		mNumClass3--;
		break;
	default:
		break;
	}
}

}

