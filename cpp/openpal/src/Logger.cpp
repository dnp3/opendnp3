#include <openpal/Logger.h>

#include <openpal/LogBase.h>

#include <assert.h>
#include <sstream>

using namespace std;

namespace openpal
{

int Logger::LogLevelToMask(LogLevel aFilter)
{
	//since LogLevel is a power of 2 (single bit), subtracting 1 will
	//set all the bits below the set bit.
	//set the filter bit and all the bits below it
	return LogLevelToType(aFilter) | (LogLevelToType(aFilter) - 1);
}
	
Logger::Logger(ILogBase* apLog, int aLevel, const std::string& aName)
	:
	mLevel(aLevel),
	mpLog(apLog),
	mName(aName)
{

}

Logger::Logger(ILogBase* apLog, LogLevel aLevel, const std::string& aName)
	:
	mLevel(LogLevelToMask(aLevel)),
	mpLog(apLog),
	mName(aName)
{

}

Logger Logger::GetSubLogger(std::string aSubName, LogLevel aLevel) const
{
	std::ostringstream oss;
	oss << mName << "." << aSubName;
	return Logger(mpLog, aLevel, oss.str());
}

Logger Logger::GetSubLogger(std::string aSubName, int aLevel) const
{
	std::ostringstream oss;
	oss << mName << "." << aSubName;
	return Logger(mpLog, aLevel, oss.str());
}

Logger Logger::GetSubLogger(std::string aSubName) const
{
	return this->GetSubLogger(aSubName, mLevel);
}

void Logger::Log( const LogEntry& arEntry)
{
	if(this->IsEnabled(arEntry.GetLogLevel())) mpLog->Log(arEntry);
}

void Logger::Log( LogLevel aLogLevel, const std::string& arLocation, const std::string& aMessage, int aErrorCode)
{
	if(this->IsEnabled(aLogLevel)) {
		LogEntry le(aLogLevel, mName, arLocation, aMessage, aErrorCode);
		mpLog->Log(le);
	}
}

}

