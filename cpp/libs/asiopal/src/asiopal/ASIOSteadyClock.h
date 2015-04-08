
#ifndef ASIOPAL_ASIOSTEADYCLOCK_H
#define ASIOPAL_ASIOSTEADYCLOCK_H

#include<chrono>
#include<Windows.h>

namespace asiopal {
	/*
	Custom steady clock implementation to handle the situation where the Windows steady clock
	implementation is not monotonic. Normal steady clock implementation is used on other platforms.
	*/
	struct ASIOSteadyClock
	{
		// Type defininitions for this clock - ticks are in nanoseconds for this clock
		typedef LONGLONG representation;
		typedef std::ratio_multiply<std::ratio<1, 1>, std::nano> period;
		typedef std::chrono::duration<representation, period> duration;
		typedef std::chrono::time_point<ASIOSteadyClock, duration> time_point;


		static time_point now() {
#if WIN32
			// Special case for WIN32 because std::chrono::steady_clock is broken
			LARGE_INTEGER freq;
			LONGLONG nanoSecondsPerTick = 0;
			if (QueryPerformanceFrequency(&freq)) {
				nanoSecondsPerTick = LONGLONG(1000000000.0L / freq.QuadPart);
			}

			LARGE_INTEGER performanceCount;
			if (nanoSecondsPerTick <= 0 || !QueryPerformanceCounter(&performanceCount)) {
				// Error condition, return 0 time
				return time_point();
			}

			// Return time in nanoseconds
			LONGLONG timeNanos = performanceCount.QuadPart * nanoSecondsPerTick;
			return time_point(duration(timeNanos));
#else
			// Get normal steady clock time in nanoseconds and convert to a time point for this clock
			std::chrono::steady_clock::duration steadyClockTime = std::chrono::steady_clock::now().time_since_epoch();
			std::chrono::duration<LONGLONG, std::nano> durationNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(steadyClockTime);
			LONGLONG nanoseconds = durationNanoseconds.count();
			return time_point(duration(nanoseconds));
#endif
		}
	};
}

#endif