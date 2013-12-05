#include "AsyncTaskNonPeriodic.h"
#include "AsyncTaskGroup.h"

#include <openpal/TimeDuration.h>

using namespace openpal;

namespace opendnp3
{


AsyncTaskNonPeriodic::AsyncTaskNonPeriodic(openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName) :
	AsyncTaskBase(aPriority, arCallback, apGroup, MonotonicTimestamp::Min(), arName),
	mRetryDelay(aRetryDelay)
{

}

void AsyncTaskNonPeriodic::_OnComplete(bool aSuccess)
{
	if(aSuccess) {
		mIsComplete = true;
		mNextRunTime = MonotonicTimestamp::Max();
	}
	else {
		mNextRunTime = mpGroup->GetCurrentTime().Add(mRetryDelay);
	}
}

} //end ns

