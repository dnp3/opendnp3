#ifndef __UTC_TIMESTAMP_H_
#define __UTC_TIMESTAMP_H_

#include <cstdint>

namespace openpal
{

/**
*  Strong typing for UTCTimestamps
*/
class UTCTimestamp
{

public:
	UTCTimestamp() : msSinceEpoch(0)
	{}

	UTCTimestamp(int64_t aMsSinceEpoch) : msSinceEpoch(aMsSinceEpoch)
	{}

	int64_t msSinceEpoch;

};

}

#endif
