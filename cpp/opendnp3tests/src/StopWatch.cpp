#include "StopWatch.h"

using namespace asiopal;
using namespace std::chrono;

namespace opendnp3
{

StopWatch::StopWatch() : mStartTime(timer_clock::now())
{

}

timer_clock::duration StopWatch :: Elapsed(bool aReset)
{
	return timer_clock::now() - mStartTime;
}

void StopWatch :: Restart()
{
	mStartTime = timer_clock::now();
}

}
