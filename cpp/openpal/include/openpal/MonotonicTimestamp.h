#ifndef __MONOTONIC_TIMESTAMP_H_
#define __MONOTONIC_TIMESTAMP_H_

#include <cstdint>

#include "TimeDuration.h"

namespace openpal
{

/**
*  Strong typing for millisecond-based monotonic timestamps
*/
class MonotonicTimestamp
{

public:

	static MonotonicTimestamp Max();
	static MonotonicTimestamp Min();

	MonotonicTimestamp();
	MonotonicTimestamp(int64_t aMilliseconds);
	MonotonicTimestamp Add(const TimeDuration& arDuration) const;

	int64_t milliseconds;
};

bool operator==(const MonotonicTimestamp& first, const MonotonicTimestamp& second);
bool operator<(const MonotonicTimestamp& first, const MonotonicTimestamp& second);


}

#endif
