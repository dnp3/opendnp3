#include "TrackingTaskGroup.h"

#include "AsyncTaskGroup.h"
#include "AsyncTaskContinuous.h"

namespace opendnp3
{

TrackingTaskGroup::TrackingTaskGroup(AsyncTaskGroup* apGroup) : mpGroup(apGroup)
{}

void TrackingTaskGroup::ResetTasks(int aMask)
{
for(AsyncTaskBase * p: mTaskVec) {
		if(!p->IsRunning() && (p->GetFlags() & aMask)) p->Reset();
	}
}

TrackingTaskGroup::~TrackingTaskGroup()
{
	//remove all the tasks that were created
for(auto pTask: mTaskVec) mpGroup->Remove(pTask);
}

AsyncTaskBase* TrackingTaskGroup::Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskBase* pTask = mpGroup->Add(aPeriod, aRetryDelay, aPriority, arCallback, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

AsyncTaskContinuous* TrackingTaskGroup::AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskContinuous* pTask = mpGroup->AddContinuous(aPriority, arCallback, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

}


