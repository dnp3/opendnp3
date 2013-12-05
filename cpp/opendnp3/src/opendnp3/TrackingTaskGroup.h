#ifndef __TRACKING_TASK_GROUP_H_
#define __TRACKING_TASK_GROUP_H_

#include <openpal/Visibility.h>
#include <openpal/TimeDuration.h>

#include <opendnp3/Uncopyable.h>

#include "AsyncTaskInterfaces.h"

#include <set>
#include <queue>
#include <string>

namespace opendnp3
{

class AsyncTaskGroup;
class AsyncTaskContinuous;
class AsyncTaskBase;

/**
 Tracks all tasks that are created and releases them on destruction
*/
class DLL_LOCAL TrackingTaskGroup
{

public:

	TrackingTaskGroup(AsyncTaskGroup* apGroup);
	~TrackingTaskGroup();

	void ResetTasks(int aMask);

	AsyncTaskBase* Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	AsyncTaskContinuous* AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName = "");


private:

	AsyncTaskGroup* mpGroup;

	typedef std::vector<AsyncTaskBase*> TaskVec;
	TaskVec mTaskVec;
};

}

#endif
