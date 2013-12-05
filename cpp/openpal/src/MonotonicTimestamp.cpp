#include <openpal/MonotonicTimestamp.h>

#include <limits>

namespace openpal
{

MonotonicTimestamp MonotonicTimestamp::Max()
{
	return MonotonicTimestamp(std::numeric_limits<int64_t>::max());
}

MonotonicTimestamp MonotonicTimestamp::Min()
{
	return MonotonicTimestamp(std::numeric_limits<int64_t>::min());
}

MonotonicTimestamp::MonotonicTimestamp() : milliseconds(0)
{}

MonotonicTimestamp::MonotonicTimestamp(int64_t aMilliseconds) : milliseconds(aMilliseconds)
{}


MonotonicTimestamp MonotonicTimestamp::Add(const TimeDuration& arDuration) const
{
	return MonotonicTimestamp(milliseconds + arDuration.GetMilliseconds());
}

bool operator==(const MonotonicTimestamp& first, const MonotonicTimestamp& second)
{
	return first.milliseconds == second.milliseconds;
}

bool operator<(const MonotonicTimestamp& first, const MonotonicTimestamp& second)
{
	return first.milliseconds < second.milliseconds;
}

}

