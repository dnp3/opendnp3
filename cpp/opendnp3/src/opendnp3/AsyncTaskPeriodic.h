#ifndef __ASYNC_TASK_PERIODIC_H_
#define __ASYNC_TASK_PERIODIC_H_


#include "AsyncTaskInterfaces.h"
#include "AsyncTaskBase.h"

#include <opendnp3/Uncopyable.h>
#include <openpal/Visibility.h>

#include <vector>

namespace opendnp3
{

class AsyncTaskGroup;

/**
	Asynchronous task. Task execution order is controlled by the period, retry, priority (for resolving ties)
	and task dependencies.
*/

class DLL_LOCAL AsyncTaskPeriodic : public AsyncTaskBase
{
	friend class AsyncTaskGroup;

public:

	virtual ~AsyncTaskPeriodic() {}

private:

	// Implements ITaskCompletion
	void _OnComplete(bool aSuccess);


	/**
		@param aPeriod Period of the task in milliseconds.
		@param aRetryDelay Retry period of the task in milliseconds. < 0 => no retry.
		@param aPriority Tie break between non-dependent tasks. Lower is higher priority.
		@param arCallback - Bound function used for starting the asynchronous task.
		@param apGroup Associated task group
		@param arName Name associated with the task
	*/
	AsyncTaskPeriodic(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName);


	openpal::TimeDuration mPeriod;
	openpal::TimeDuration mRetryDelay;
};

}

#endif
