#ifndef _STOP_WATCH_H__
#define _STOP_WATCH_H__

#include <chrono>
#include <asiopal/TimerClockType.h>

namespace opendnp3
{

/**
	This class is designed to make it easier to do simple timing tests
*/
class StopWatch
{
public:

	StopWatch();

	//get the elapsed time since creation or the last restart
	//by default each call to Elapsed restarts the timer.
	asiopal::timer_clock::duration Elapsed(bool aReset = true);

	//restart or re-zero the StopWatch.
	void Restart();

private:
	asiopal::timer_clock::time_point mStartTime;
};


}

#endif
