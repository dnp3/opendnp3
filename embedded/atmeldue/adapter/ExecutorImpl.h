#ifndef __EXECUTOR_IMPL_H_
#define __EXECUTOR_IMPL_H_

#include <openpal/executor/IExecutor.h>
#include <openpal/container/StaticQueue.h>
#include <openpal/container/StaticArray.h>
#include <openpal/container/StaticLinkedList.h>

#include "TimerImpl.h"

class ExecutorImpl : public openpal::IExecutor
{
	friend class TimerImpl;
	
	public:	
	
	ExecutorImpl();
	
	virtual openpal::MonotonicTimestamp GetTime() override final;
	
	virtual openpal::ITimer* Start(const openpal::TimeDuration& arDuration, const openpal::Action0& action) override final;
	
	virtual openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const openpal::Action0& action) override final;
	
	virtual void Post(const openpal::Action0& action) override final;	
	
	void Run();
	
	void Init();
	
	void Tick();
	
	void Sleep();
	
	protected:
	
	bool RunOne();	
	
	private:
	
	void OnCancel(TimerImpl* pTimer);
	
	bool RunOneTimer();
	
	int64_t ticks;	
	
	openpal::StaticArray<TimerImpl, uint8_t, 5> timers;
	openpal::StaticQueue<openpal::Action0, uint8_t, 5> work;
	
	openpal::StaticQueue<TimerImpl*, uint8_t, 5> idleTimers;
	openpal::StaticLinkedList<TimerImpl*, uint8_t, 5> activeTimers;
};

#endif
