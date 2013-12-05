#ifndef __ASYNC_TEST_OBJECT_H_
#define __ASYNC_TEST_OBJECT_H_

#include "TestTypedefs.h"

namespace opendnp3
{

class AsyncTestObject
{
	static bool AlwaysBoolean(bool aAlwaysThis) {
		return aAlwaysThis;
	}

public:
	AsyncTestObject() {}

	void ProceedForTime(openpal::TimeDuration aTimeout);
	bool ProceedUntil(const EvalFunc& arFunc, openpal::TimeDuration aTimeout = G_TEST_TIMEOUT);
	bool ProceedUntilFalse(const EvalFunc& arFunc, openpal::TimeDuration aTimeout = G_TEST_TIMEOUT);

	static bool Negate(const EvalFunc& arFunc) {
		return !arFunc();
	}

protected:
	// Inherited classes implement this function. Test functions iteratively call this function
	// to try and make progress
	virtual void Next() = 0;



};

}

#endif
