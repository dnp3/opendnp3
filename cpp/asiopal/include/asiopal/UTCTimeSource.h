#ifndef __UTC_TIME_SOURCE_H_
#define __UTC_TIME_SOURCE_H_

#include <openpal/IUTCTimeSource.h>

namespace asiopal
{

/**
*  Interface that defines a method to get UTC timestamps
*/
class UTCTimeSource : public openpal::IUTCTimeSource
{

public:
	static openpal::IUTCTimeSource* Inst();
	openpal::UTCTimestamp Now();

private:
	UTCTimeSource() {}
	static UTCTimeSource mInstance;
};


}

#endif

