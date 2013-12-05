#ifndef __EXCEPTION_SCAN_H_
#define __EXCEPTION_SCAN_H_

#include <openpal/TimeDuration.h>

namespace  opendnp3
{

/**
* Represents an exception scan
*/
struct ExceptionScan {

	/// Defaults to all events every 5 seconds
	ExceptionScan() :
		ClassMask(PC_ALL_EVENTS),
		ScanRate(openpal::TimeDuration::Seconds(5))
	{}

	ExceptionScan(int aClassMask, openpal::TimeDuration aScanRate) :
		ClassMask(aClassMask),
		ScanRate(aScanRate)
	{}

	/// Bitmask for which classes will be scanned
	int ClassMask;

	/// Scan period
	openpal::TimeDuration ScanRate;
};

}

#endif

