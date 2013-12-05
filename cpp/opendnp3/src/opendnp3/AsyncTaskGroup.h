#ifndef __ASYNC_TASK_GROUP_H_
#define __ASYNC_TASK_GROUP_H_


#include <opendnp3/Uncopyable.h>
#include <openpal/Visibility.h>
#include <openpal/MonotonicTimestamp.h>

#include "AsyncTaskInterfaces.h"

#include <set>
#include <queue>
#include <string>

namespace openpal
{
class IExecutor;
class ITimer;
}

namespace opendnp3
{

class AsyncTaskBase;
class AsyncTaskPeriodic;
class AsyncTaskNonPeriodic;
class AsyncTaskContinuous;
class ITimeSource;

/**
 A collection of related tasks with optional dependencies
*/
class DLL_LOCAL AsyncTaskGroup : private Uncopyable
{
	friend class AsyncTaskBase;
	friend class AsyncTaskContinuous;

public:

	AsyncTaskGroup(openpal::IExecutor*);
	~AsyncTaskGroup();

	AsyncTaskBase* Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	AsyncTaskContinuous* AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	void Remove(AsyncTaskBase* apTask);

	void Shutdown();

	void Enable();
	void Disable();

	void Enable(int aMask);
	void Disable(int aMask);

	void ResetTasks(int aMask);

	void CheckState();

	bool IsRunning() {
		return mIsRunning;
	}

	openpal::MonotonicTimestamp GetCurrentTime() const;

private:

	void OnCompletion();
	void RestartTimer(const openpal::MonotonicTimestamp& arTime);
	void OnTimerExpiration();
	void Update(const  openpal::MonotonicTimestamp& arTime);
	AsyncTaskBase* GetNext(const  openpal::MonotonicTimestamp& arTime);

	bool mIsRunning;
	bool mShutdown;
	openpal::IExecutor* mpExecutor;
	openpal::ITimer* mpTimer;

	typedef std::vector< AsyncTaskBase* > TaskVec;
	TaskVec mTaskVec;
};

}

#endif
