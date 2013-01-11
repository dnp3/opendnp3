#ifndef __PHYSICAL_LAYER_SYNC_PROXY_H_
#define __PHYSICAL_LAYER_SYNC_PROXY_H_

#include <APL/Thread.h>
#include <APL/Threadable.h>
#include <APL/PhysicalLayerAsyncBase.h>

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <boost/system/error_code.hpp>

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace apl
{

class Logger;

/** Simple thread object that allows for non-blocking read of std input.
*/
class PhysicalLayerSyncProxy : public PhysicalLayerAsyncBase, private Threadable
{
public:
	PhysicalLayerSyncProxy(Logger*, boost::asio::io_service*);

	void DoOpen();
	void DoClose();
	void DoAsyncRead(uint8_t*, size_t);
	void DoAsyncWrite(const uint8_t*, size_t);

	static const boost::system::error_code mSuccessCode;
	static const boost::system::error_code mErrorCode;

protected:

	// Blocking read defined in base class
	virtual std::string Read() = 0;

	// Blocking write defined in base class
	virtual void Write(const std::string&) = 0;



private:

	void SignalStop();

	void Run();
	void Reset();
	void CheckForRead();

	boost::asio::io_service* mpService;

	bool mReading;
	size_t mNumToRead;
	uint8_t* mpBuffer;

	std::mutex mMutex;
	std::condition_variable mCondition;
	std::deque<std::string> mLineQueue;
	Thread mThread;

};
}

#endif

