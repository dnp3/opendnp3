#ifndef __ASYNC_TASK_CONTINUOUS_H_
#define __ASYNC_TASK_CONTINUOUS_H_

#include "AsyncTaskInterfaces.h"
#include "AsyncTaskBase.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

class AsyncTaskGroup;

/** Continus asynchronous task, runs over and over as long as it is enabled.
*/

class DLL_LOCAL AsyncTaskContinuous : public AsyncTaskBase
{
	friend class AsyncTaskGroup;

public:

	virtual ~AsyncTaskContinuous() {}

private:

	/**
		@param aPriority Tie break between non-dependent tasks. Lower is higher priority.
		@param arCallback Bound function used for starting the asynchronous task.
		@param apGroup Associated task group
		@param arName Name associated with the tasak
	*/
	AsyncTaskContinuous(int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName);


};

}

#endif
