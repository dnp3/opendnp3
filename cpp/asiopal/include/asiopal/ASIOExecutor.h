#ifndef __ASIO_EXECUTOR_H_
#define __ASIO_EXECUTOR_H_

#include <openpal/Exception.h>
#include <openpal/IExecutor.h>
#include <openpal/Visibility.h>

#include <queue>

#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>


namespace asiopal
{

class TimerASIO;

class DLL_LOCAL ASIOExecutor : public openpal::IExecutor
{

public:
	ASIOExecutor(boost::asio::strand*);
	~ASIOExecutor();

	openpal::MonotonicTimestamp GetTime();

	openpal::ITimer* Start(const openpal::TimeDuration&, const std::function<void ()>&);
	openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const std::function<void ()>&);
	void Post(const std::function<void ()>&);

private:

	void Shutdown();

	TimerASIO* GetTimer();
	void StartTimer(TimerASIO*, const std::function<void ()>&);

	boost::asio::strand* mpStrand;

	typedef std::deque<TimerASIO*> TimerQueue;

	TimerQueue mAllTimers;
	TimerQueue mIdleTimers;

	size_t mNumActiveTimers;
	std::mutex mMutex;
	std::condition_variable mCondition;
	bool mIsShuttingDown;

	void OnTimerCallback(const boost::system::error_code&, TimerASIO*, std::function<void ()>);
};
}

#endif

