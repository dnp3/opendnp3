#ifndef __LOG_TESTER_H_
#define __LOG_TESTER_H_

#include <openpal/Logger.h>
#include <queue>

namespace opendnp3
{

class LogTester : public openpal::ILogBase
{

public:
	LogTester();

	void Log(const std::string& aLocation, const std::string& aMessage);

	void Log( const openpal::LogEntry& arEntry );

	int ClearLog();
	int NextErrorCode();
	bool GetNextEntry(openpal::LogEntry& arEntry);
	bool IsLogErrorFree();

	openpal::Logger mTestLogger;

protected:
	std::queue<openpal::LogEntry> mBuffer;

};


}

#endif
