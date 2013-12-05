#ifndef __TIME_DURATION_H_
#define __TIME_DURATION_H_

#include <cstdint>

namespace openpal
{

template <class T>
class TimeDurationBase
{

public:

	TimeDurationBase() : milliseconds(0)
	{}

	T GetMilliseconds() const {
		return milliseconds;
	}

protected:
	TimeDurationBase(T aMilliseconds) : milliseconds(aMilliseconds)
	{}

private:
	T milliseconds;

};


/**
*  Strong typing for millisecond based time durations
*/
class TimeDuration : public TimeDurationBase<int64_t>
{

public:

	TimeDuration();

	static TimeDuration Min();

	static TimeDuration Zero();

	static TimeDuration Milliseconds(int64_t aMilliseconds);

	static TimeDuration Seconds(int64_t aSeconds);

	static TimeDuration Minutes(int64_t aMinutes);

private:
	TimeDuration(int64_t aMilliseconds);
};

bool operator==(const TimeDuration& lhs, const TimeDuration& rhs);

}

#endif
