#include "AsyncTaskPeriodic.h"

#include "AsyncTaskGroup.h"

using namespace openpal;

namespace opendnp3
{

AsyncTaskPeriodic::AsyncTaskPeriodic(TimeDuration aPeriod, TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName) :
	AsyncTaskBase(aPriority, arCallback, apGroup, MonotonicTimestamp::Min(), arName),
	mPeriod(aPeriod),
	mRetryDelay(aRetryDelay)
{

}

void AsyncTaskPeriodic::_OnComplete(bool aSuccess)
{
	const auto now = mpGroup->GetCurrentTime();
	if(aSuccess) {
		mIsComplete = true;
		mNextRunTime = now.Add(mPeriod);
	}
	else {
		mNextRunTime = now.Add(mPeriod);
	}
}

} //end ns

