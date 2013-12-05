#ifndef __IO_SERVICE_THREAD_POOL_
#define __IO_SERVICE_THREAD_POOL_

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>


#include "DeadlineTimerSteadyClock.h"

#include <boost/asio.hpp>
#include <functional>
#include <thread>

namespace asiopal
{

class DLL_LOCAL IOServiceThreadPool : private openpal::Loggable
{
public:

	IOServiceThreadPool(
	        openpal::Logger aLogger,
	        uint32_t aConcurrency,
	std::function<void()> onThreadStart = []() {},
	std::function<void()> onThreadExit = []() {}
	);

	~IOServiceThreadPool();

	boost::asio::io_service* GetIOService();

	void Shutdown();

private:

	std::function<void ()> mOnThreadStart;
	std::function<void ()> mOnThreadExit;

	bool mIsShutdown;

	void OnTimerExpiration(const boost::system::error_code& ec);

	void Run();

	boost::asio::io_service mService;
	boost::asio::monotonic_timer mInfiniteTimer;
	std::vector<std::thread*> mThreads;
};

}


#endif
