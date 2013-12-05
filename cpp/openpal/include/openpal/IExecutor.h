#ifndef __I_EXECUTOR_H_
#define __I_EXECUTOR_H_

#include <functional>

#include "ITimer.h"
#include "TimeDuration.h"
#include "MonotonicTimestamp.h"

namespace openpal
{

/**
 * Interface for posting events to a queue.  Events can be posted for
 * immediate consumption or some time in the future.  Events can be consumbed
 * by the posting thread or another thread.
 *
 */
class IExecutor
{
public:

	virtual ~IExecutor() {}

	/** A non-absolute timestamp for the monotonic time source */
	virtual MonotonicTimestamp GetTime() = 0;

	/** Returns a new timer based on a relative time duration */
	virtual ITimer* Start(const TimeDuration& arDuration, const std::function<void ()> &) = 0;

	/** Returns a new timer based on an absolute timestamp of the monotonic clock */
	virtual ITimer* Start(const MonotonicTimestamp&, const std::function<void ()> &) = 0;

	/** Thread-safe way to post an event to be handled asynchronously */
	virtual void Post(const std::function<void ()> &) = 0;

};

}

#endif
