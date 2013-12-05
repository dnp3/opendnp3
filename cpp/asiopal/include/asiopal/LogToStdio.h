#ifndef __LOG_TO_STDIO_H_
#define __LOG_TO_STDIO_H_

#include <mutex>
#include <openpal/LogBase.h>

namespace asiopal
{

/**
* Singleton class that prints all log messages to the console
*
*
*/
class LogToStdio : public openpal::ILogBase
{

public:
	static LogToStdio* Inst() {
		return &mInstance;
	}

	void Log( const openpal::LogEntry& arEntry );	
	void SetPrintLocation(bool aPrintLocation);

protected:
	LogToStdio();

private:
	std::string ToNormalizedString(const std::chrono::high_resolution_clock::time_point& arTime);
	static LogToStdio mInstance;
	bool mPrintLocation;
	std::mutex mMutex;
};

}

#endif
