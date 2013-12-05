#ifndef __ASYNC_TASK_NON_PERIODIC_H_
#define __ASYNC_TASK_NON_PERIODIC_H_

#include "AsyncTaskInterfaces.h"
#include "AsyncTaskBase.h"

#include <opendnp3/Uncopyable.h>
#include <openpal/Visibility.h>

#include <vector>


namespace opendnp3
{

class AsyncTaskGroup;

/**
	@section desc Asynchronous task. Task execution order is controlled by the period, retry, priority (for resolving ties)
	and task dependencies.
*/

class DLL_LOCAL AsyncTaskNonPeriodic : public AsyncTaskBase
{
	friend class AsyncTaskGroup;

public:

	virtual ~AsyncTaskNonPeriodic() {}

private:

	// Implements ITaskCompletion
	void _OnComplete(bool aSuccess);


	/**
		@param aRetryDelay Retry period of the task in milliseconds. < 0 => no retry.
		@param aPriority Tie break between non-dependent tasks. Lower is higher priority.
		@param arCallback Bound function used for starting the asynchronous task.
		@param apGroup Associated task group
		@param arName Name associated with the task
	*/
	AsyncTaskNonPeriodic(openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName);

	openpal::TimeDuration mRetryDelay;
};

}

#endif
