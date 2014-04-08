
#include "AVRTimer.h"

#include "AVRExecutor.h"

using namespace openpal;

AVRTimer::AVRTimer() : pExecutor(nullptr), expirationTime(MonotonicTimestamp::Min())
{}

void AVRTimer::Set(AVRExecutor* pExeuctor_, const openpal::Runnable runnable_, const openpal::MonotonicTimestamp& expirationTime_)
{
	pExecutor = pExeuctor_;
	runnable = runnable_;
	expirationTime = expirationTime_;
}

void AVRTimer::Cancel()
{
	if(pExecutor)
	{
		pExecutor->OnCancel(this);	
	}	
}

MonotonicTimestamp AVRTimer::ExpiresAt()
{
	return expirationTime;
}
