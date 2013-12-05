#include "AsyncTestObject.h"

#include "Timeout.h"

#include <functional>
#include <chrono>

using namespace openpal;
using namespace std;

namespace opendnp3
{

bool AsyncTestObject::ProceedUntil(const EvalFunc& arFunc, openpal::TimeDuration aTimeout)
{
	Timeout to(std::chrono::milliseconds(aTimeout.GetMilliseconds()));

	do {
		if(arFunc()) return true;
		else this->Next();
	}
	while(!to.IsExpired());

	return false;
}

void AsyncTestObject::ProceedForTime(openpal::TimeDuration aTimeout)
{
	ProceedUntil(std::bind(&AsyncTestObject::AlwaysBoolean, false), aTimeout);
}

bool AsyncTestObject::ProceedUntilFalse(const EvalFunc& arFunc, openpal::TimeDuration aTimeout)
{
	return ProceedUntil(std::bind(&AsyncTestObject::Negate, std::cref(arFunc)), aTimeout);
}

}
