#ifndef __AVR_EXECUTOR_H_
#define __AVR_EXECUTOR_H_

#include <openpal/IExecutor.h>
#include <openpal/StaticQueue.h>

class AVRExecutor : public openpal::IExecutor
{
	public:
	
	virtual openpal::MonotonicTimestamp GetTime() override final;
	
	virtual openpal::ITimer* Start(const openpal::TimeDuration& arDuration, const openpal::Runnable& runnable) override final;
	
	virtual openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const openpal::Runnable& runnable) override final;
	
	virtual void Post(const openpal::Runnable& runnable) override final;
	
	void RunOne();
	
	protected:
	
	virtual void Pause() override final;
	
	virtual void Resume() override final;
	
	private:
	
	openpal::StaticQueue<openpal::Runnable, uint16_t, 4> work;
};

class AVRTimer : openpal::ITimer
{
	public:
	
	static openpal::ITimer* Inst();
	
	virtual void Cancel() override final;
	virtual openpal::MonotonicTimestamp ExpiresAt() override final;
	
	private:
	
	static AVRTimer instance;
	
	AVRTimer() {}
};

#endif /* AVREXECUTOR_H_ */
