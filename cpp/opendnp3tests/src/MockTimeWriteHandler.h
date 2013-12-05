#ifndef __MOCK_TIME_WRITE_HANDLER_H_
#define __MOCK_TIME_WRITE_HANDLER_H_

#include <opendnp3/ITimeWriteHandler.h>

#include <functional>

namespace opendnp3
{

class MockTimeWriteHandler : public ITimeWriteHandler
{
public:
	MockTimeWriteHandler(std::function<void (openpal::UTCTimestamp)> aTimeWriteCallback) : mTimeWriteCallback(aTimeWriteCallback) {}

	void WriteAbsoluteTime(openpal::UTCTimestamp aTimestamp) {
		mTimeWriteCallback(aTimestamp);
	}

private:
	std::function<void (openpal::UTCTimestamp)> mTimeWriteCallback;
};

}

#endif

