#ifndef __AVR_TIMER_H_
#define __AVR_TIMER_H_

#include <openpal/ITimer.h>
#include <openpal/Runnable.h>

class AVRExecutor;

class AVRTimer : public openpal::ITimer
{
	public:
	
	AVRTimer();
	
	void Set(AVRExecutor* pExecutor, const openpal::Runnable runnable, const openpal::MonotonicTimestamp& expirationTime_);
		
	virtual void Cancel() override final;
	virtual openpal::MonotonicTimestamp ExpiresAt() override final;
	
	private:
	AVRExecutor* pExecutor;
	openpal::Runnable runnable;
	openpal::MonotonicTimestamp expirationTime;
};

#endif

