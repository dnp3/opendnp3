#include <openpal/TimeDuration.h>

#include <limits>

namespace openpal
{

TimeDuration TimeDuration::Min()
{
	return TimeDuration(std::numeric_limits<int64_t>::min());
}

TimeDuration TimeDuration::Zero()
{
	return TimeDuration(0);
}

TimeDuration TimeDuration::Milliseconds(int64_t aMilliseconds)
{
	return TimeDuration(aMilliseconds);
}

TimeDuration TimeDuration::Seconds(int64_t aSeconds)
{
	return TimeDuration(1000 * aSeconds);
}

TimeDuration TimeDuration::Minutes(int64_t aMinutes)
{
	return TimeDuration(1000 * 60 * aMinutes);
}

TimeDuration::TimeDuration() : TimeDurationBase(0) {}

TimeDuration::TimeDuration(int64_t aMilliseconds) : TimeDurationBase(aMilliseconds)
{}

bool operator==(const TimeDuration& lhs, const TimeDuration& rhs)
{
	return lhs.GetMilliseconds() == rhs.GetMilliseconds();
}

}
