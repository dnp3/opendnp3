#ifndef __I_TIMER_H_
#define __I_TIMER_H_

#include "MonotonicTimestamp.h"

namespace openpal
{

/**
 * Timer are used to defer events for a later time on an executor.
 */

class ITimer
{
public:
	virtual ~ITimer() {}
	virtual void Cancel() = 0;
	virtual MonotonicTimestamp ExpiresAt() = 0;
};

}

#endif
