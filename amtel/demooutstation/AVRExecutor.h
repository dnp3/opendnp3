#ifndef __AVR_EXECUTOR_H_
#define __AVR_EXECUTOR_H_

#include <openpal/IExecutor.h>
#include <openpal/StaticQueue.h>
#include <openpal/StaticArray.h>

#include "AVRTimer.h"

class AVRExecutor : public openpal::IExecutor
{
	friend class AVRTimer;
	
	public:	
	
	AVRExecutor();
	
	virtual openpal::MonotonicTimestamp GetTime() override final;
	
	virtual openpal::ITimer* Start(const openpal::TimeDuration& arDuration, const openpal::Runnable& runnable) override final;
	
	virtual openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const openpal::Runnable& runnable) override final;
	
	virtual void Post(const openpal::Runnable& runnable) override final;
	
	bool RunOne();
	
	void Init();
	
	void Tick();
	
	protected:
	
	virtual void Pause() override final {}
	
	virtual void Resume() override final {}
	
	private:
	
	void OnCancel(AVRTimer* pTimer);
	
	int64_t ticks;	
	
	openpal::StaticArray<AVRTimer, uint8_t, 3> timers;
	openpal::StaticQueue<openpal::Runnable, uint8_t, 3> work;
	openpal::StaticQueue<AVRTimer*, uint8_t, 3> idleTimers;
};

#endif
