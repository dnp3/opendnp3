#ifndef __TIMER_ASIO_H_
#define __TIMER_ASIO_H_

#include <boost/asio.hpp>

#include <openpal/Visibility.h>
#include <openpal/IExecutor.h>

#include <asiopal/DeadlineTimerSteadyClock.h>

namespace asiopal
{

/**
 * This is a wrapper for ASIO timers that are used to post events
 * on a queue. Events can be posted for immediate consumption or
 * some time in the future. Events can be consumbed by the posting
 * thread or another thread.
 *
 * @section Class Goals
 *
 * Decouple APL code form ASIO so ASIO could be replace if need be.
 *
 * There is a problem with ASIO. When cancel is called, an event is
 * posted. We wanted a cancel that does not generate any events.
 *
 */
class DLL_LOCAL TimerASIO : public openpal::ITimer
{
	friend class ASIOExecutor;

public:
	TimerASIO(boost::asio::strand* apStrand);

	// Implement ITimer
	void Cancel();

	/**
	 * Return the timer's expiry time as an absolute time.
	 */
	openpal::MonotonicTimestamp ExpiresAt();

private:

	bool mCanceled;

	boost::asio::monotonic_timer mTimer;
};

}

#endif
