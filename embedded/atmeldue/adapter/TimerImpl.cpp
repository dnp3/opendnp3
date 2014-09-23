
#include "TimerImpl.h"

#include "ExecutorImpl.h"

using namespace openpal;

TimerImpl::TimerImpl() : pExecutor(nullptr), expirationTime(MonotonicTimestamp::Min())
{}

void TimerImpl::Set(ExecutorImpl* pExeuctor_, const openpal::Action0 action_, const openpal::MonotonicTimestamp& expirationTime_)
{
	pExecutor = pExeuctor_;
	action = action_;
	expirationTime = expirationTime_;
}

void TimerImpl::Cancel()
{
	if(pExecutor)
	{
		pExecutor->OnCancel(this);	
	}	
}

MonotonicTimestamp TimerImpl::ExpiresAt()
{
	return expirationTime;
}
