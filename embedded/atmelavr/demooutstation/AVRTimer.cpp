
#include "AVRTimer.h"

#include "AVRExecutor.h"

using namespace openpal;

AVRTimer::AVRTimer() : pExecutor(nullptr), expirationTime(MonotonicTimestamp::Min())
{}

void AVRTimer::Set(AVRExecutor* pExeuctor_, const openpal::Action0 action_, const openpal::MonotonicTimestamp& expirationTime_)
{
	pExecutor = pExeuctor_;
	action = action_;
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
