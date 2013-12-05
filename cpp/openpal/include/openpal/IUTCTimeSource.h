#ifndef __I_UTC_TIME_SOURCE_H_
#define __I_UTC_TIME_SOURCE_H_

#include <openpal/UTCTimestamp.h>

namespace openpal
{

/**
*  Interface that defines a method to get UTC timestamps
*/
class IUTCTimeSource
{

public:
	/**
	*  Returns a UTCTimestamp of the current time
	*/
	virtual UTCTimestamp Now() = 0;
};

/**
* Mock usable for testing
*/
class FixedUTCTimeSource : public IUTCTimeSource
{
public:
	FixedUTCTimeSource(int64_t aTimeSinceEpoch): mTimeSinceEpoch(aTimeSinceEpoch)
	{}

	UTCTimestamp Now() {
		return UTCTimestamp (mTimeSinceEpoch);
	}

	int64_t mTimeSinceEpoch;
};


}

#endif

