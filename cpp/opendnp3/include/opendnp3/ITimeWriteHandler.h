#ifndef __I_TIME_WRITE_HANDLER_H_
#define __I_TIME_WRITE_HANDLER_H_

#include <openpal/UTCTimestamp.h>

#include <functional>

namespace opendnp3
{

class ITimeWriteHandler
{
public:
	virtual ~ITimeWriteHandler() {}

	virtual void WriteAbsoluteTime(openpal::UTCTimestamp aTimestamp) = 0;
};

class FunctionTimeWriteHandler : public ITimeWriteHandler
{
public:
	FunctionTimeWriteHandler(std::function<void (openpal::UTCTimestamp aTimestamp)> aCallback) :
		mCallback(aCallback)
	{}

	void WriteAbsoluteTime(openpal::UTCTimestamp aTimestamp) {
		mCallback(aTimestamp);
	}

private:
	std::function<void (openpal::UTCTimestamp aTimestamp)> mCallback;

};

class NullTimeWriteHandler : public FunctionTimeWriteHandler
{
public:
	static ITimeWriteHandler* Inst();

private:
	static NullTimeWriteHandler mInstance;

	NullTimeWriteHandler() : FunctionTimeWriteHandler([](openpal::UTCTimestamp) {}) {}
};

}

#endif

