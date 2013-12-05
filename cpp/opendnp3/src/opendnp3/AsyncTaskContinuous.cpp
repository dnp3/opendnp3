#include "AsyncTaskContinuous.h"

#include "AsyncTaskGroup.h"

namespace opendnp3
{

AsyncTaskContinuous::AsyncTaskContinuous(int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName) :
	AsyncTaskBase(aPriority, arCallback, apGroup, openpal::MonotonicTimestamp::Min(), arName)
{

}

}

