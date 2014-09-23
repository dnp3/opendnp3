#ifndef __TIMER_IMPL_H_
#define __TIMER_IMPL_H_

#include <openpal/executor/ITimer.h>
#include <openpal/executor/Action0.h>

class ExecutorImpl;

class TimerImpl : public openpal::ITimer
{
	public:
	
	TimerImpl();
	
	void Set(ExecutorImpl* pExecutor, const openpal::Action0 action, const openpal::MonotonicTimestamp& expirationTime_);
		
	virtual void Cancel() override final;
	virtual openpal::MonotonicTimestamp ExpiresAt() override final;
	
	ExecutorImpl* pExecutor;
	openpal::Action0 action;
	openpal::MonotonicTimestamp expirationTime;
};

#endif

