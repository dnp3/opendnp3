#ifndef __AVR_TIMER_H_
#define __AVR_TIMER_H_

#include <openpal/executor/ITimer.h>
#include <openpal/executor/Action0.h>

class AVRExecutor;

class AVRTimer : public openpal::ITimer
{
	public:
	
	AVRTimer();
	
	void Set(AVRExecutor* pExecutor, const openpal::Action0 action, const openpal::MonotonicTimestamp& expirationTime_);
		
	virtual void Cancel() override final;
	virtual openpal::MonotonicTimestamp ExpiresAt() override final;
	
	AVRExecutor* pExecutor;
	openpal::Action0 action;
	openpal::MonotonicTimestamp expirationTime;
};

#endif

