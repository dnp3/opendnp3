#include <asiopal/IOServiceThreadPool.h>

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>

#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;
using namespace openpal;

#ifdef ERROR
#undef ERROR
#endif

namespace asiopal
{

IOServiceThreadPool::IOServiceThreadPool(
        openpal::Logger aLogger,
        uint32_t aConcurrency,
        std::function<void()> onThreadStart,
        std::function<void()> onThreadExit) :
	Loggable(aLogger),
	mOnThreadStart(onThreadStart),
	mOnThreadExit(onThreadExit),
	mIsShutdown(false),
	mService(),
	mInfiniteTimer(mService)
{
	if(aConcurrency == 0) {
		aConcurrency = 1;
		LOG_BLOCK(LogLevel::Warning, "Concurrency was set to 0, defaulting to 1 thread");
	}
	mInfiniteTimer.expires_at(timer_clock::time_point::max());
	mInfiniteTimer.async_wait(bind(&IOServiceThreadPool::OnTimerExpiration, this, placeholders::_1));
	for(uint32_t i = 0; i < aConcurrency; ++i) {
		mThreads.push_back(new thread(bind(&IOServiceThreadPool::Run, this)));
	}
}

void IOServiceThreadPool::OnTimerExpiration(const boost::system::error_code& ec)
{

}

IOServiceThreadPool::~IOServiceThreadPool()
{
	this->Shutdown();
for(auto pThread: mThreads) {
		delete pThread;
	}
}

void IOServiceThreadPool::Shutdown()
{
	if(!mIsShutdown) {
		mIsShutdown = true;
		mInfiniteTimer.cancel();
for(auto pThread: mThreads) pThread->join();
	}
}

boost::asio::io_service* IOServiceThreadPool::GetIOService()
{
	return &mService;
}

void IOServiceThreadPool::Run()
{
	size_t num = 0;

	mOnThreadStart();

	do {
		try {
			num = mService.run();
		}
		catch(const std::exception& ex) {
			num = 0;
									
			LOG_BLOCK(LogLevel::Error, "Unhandled exception in thread pool: " << ex.what());			
		}
	}
	while(num > 0);

	mOnThreadExit();
}

}
