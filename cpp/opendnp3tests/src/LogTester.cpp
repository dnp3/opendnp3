#include "LogTester.h"

using namespace openpal;

namespace opendnp3
{

LogTester::LogTester() : mTestLogger(this, LogLevel::Debug, "LogTester")
{

}

void LogTester::Log( const LogEntry& arEntry )
{
	mBuffer.push(arEntry);
}

int LogTester::ClearLog()
{
	int max = -1;
	LogEntry le;
	while(!mBuffer.empty()) {
		if(mBuffer.front().GetErrorCode() > max) max = le.GetErrorCode();
		mBuffer.pop();
	}

	return max;
}

void LogTester::Log(const std::string& arLocation, const std::string& arMessage)
{
	mTestLogger.Log(LogLevel::Event, arLocation, arMessage);
}

int LogTester::NextErrorCode()
{
	LogEntry le;
	while(!mBuffer.empty()) {
		le = mBuffer.front();
		mBuffer.pop();
		if(le.GetErrorCode() >= 0) {
			return le.GetErrorCode();
		}
	}
	return -1;
}

bool LogTester::GetNextEntry(LogEntry& arEntry)
{
	if(mBuffer.empty()) return false;
	else {
		arEntry = mBuffer.front();
		mBuffer.pop();
		return true;
	}
}

bool LogTester::IsLogErrorFree()
{
	return ClearLog() < 0;
}

}

