#ifndef __EXECUTOR_IMPL_H_
#define __EXECUTOR_IMPL_H_

#include <openpal/executor/IExecutor.h>
#include <openpal/container/Queue.h>
#include <openpal/container/DynamicArray.h>
#include <openpal/container/LinkedList.h>

#include "TimerImpl.h"

class ExecutorImpl : public openpal::IExecutor
{
	friend class TimerImpl;
	
	public:	
	
	ExecutorImpl(uint8_t maxQueueSize, uint8_t maxtimers);
	
	virtual openpal::MonotonicTimestamp GetTime() override final;
	
	virtual openpal::ITimer* Start(const openpal::TimeDuration& arDuration, const openpal::Action0& action) override final;
	
	virtual openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const openpal::Action0& action) override final;
	
	virtual void Post(const openpal::Action0& action) override final;	
	
	void Run();	
	
	void Tick();	
	
	protected:
	
	bool RunOne();	
	
	private:
	
	void OnCancel(TimerImpl* pTimer);
	
	bool RunOneTimer();
	
	int64_t ticks;		
	
	openpal::DynamicArray<TimerImpl, uint8_t> timers;
	openpal::Queue<openpal::Action0, uint8_t> work;
	
	openpal::Queue<TimerImpl*, uint8_t> idleTimers;
	openpal::LinkedList<TimerImpl*, uint8_t> activeTimers;
};

#endif
