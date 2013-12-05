#include <asiopal/ASIOExecutor.h>

#include "TimerASIO.h"

#include <openpal/Exception.h>
#include <openpal/Location.h>

#include <boost/asio.hpp>
#include <functional>

using namespace std;

namespace asiopal
{

ASIOExecutor::ASIOExecutor(boost::asio::strand* apStrand) :
	mpStrand(apStrand),
	mNumActiveTimers(0),
	mIsShuttingDown(false)
{

}

ASIOExecutor::~ASIOExecutor()
{
	this->Shutdown();
for(auto pTimer: mAllTimers) delete pTimer;
}

openpal::MonotonicTimestamp ASIOExecutor::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(asiopal::timer_clock::now().time_since_epoch()).count();
}

openpal::ITimer* ASIOExecutor::Start(const openpal::TimeDuration& arDelay, const function<void ()>& arCallback)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mIsShuttingDown) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "Can't start a timer while executor is shutting down");
	TimerASIO* pTimer = GetTimer();
	pTimer->mTimer.expires_from_now(std::chrono::milliseconds(arDelay.GetMilliseconds()));
	this->StartTimer(pTimer, arCallback);
	return pTimer;
}

openpal::ITimer* ASIOExecutor::Start(const openpal::MonotonicTimestamp& arTime, const function<void ()>& arCallback)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mIsShuttingDown) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "Can't start a timer while executor is shutting down");
	TimerASIO* pTimer = GetTimer();
	pTimer->mTimer.expires_at(asiopal::timer_clock::time_point(std::chrono::milliseconds(arTime.milliseconds)));
	this->StartTimer(pTimer, arCallback);
	return pTimer;
}

void ASIOExecutor::Post(const std::function<void ()>& arHandler)
{
	mpStrand->post(arHandler);
}

TimerASIO* ASIOExecutor::GetTimer()
{
	TimerASIO* pTimer;
	if(mIdleTimers.size() == 0) {
		pTimer = new TimerASIO(mpStrand);
		mAllTimers.push_back(pTimer);
	}
	else {
		pTimer = mIdleTimers.front();
		mIdleTimers.pop_front();
	}

	pTimer->mCanceled = false;
	return pTimer;
}

void ASIOExecutor::Shutdown()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mIsShuttingDown = true;
	while(mNumActiveTimers) {
		mCondition.wait(lock);
	}
}

void ASIOExecutor::StartTimer(TimerASIO* apTimer, const std::function<void ()>& arCallback)
{
	++mNumActiveTimers;
	apTimer->mTimer.async_wait(
	        mpStrand->wrap(
	                std::bind(&ASIOExecutor::OnTimerCallback, this, std::placeholders::_1, apTimer, arCallback)
	        )
	);
}

void ASIOExecutor::OnTimerCallback(const boost::system::error_code& ec, TimerASIO* apTimer, std::function<void ()> aCallback)
{
	bool callback = false;
	{
		std::lock_guard<std::mutex> lock(mMutex);
		--mNumActiveTimers;
		mIdleTimers.push_back(apTimer);
		if(! (ec || apTimer->mCanceled) ) callback = true;
		if(mNumActiveTimers == 0) mCondition.notify_all();
	}
	if(callback) aCallback();
}

} //end namespace

