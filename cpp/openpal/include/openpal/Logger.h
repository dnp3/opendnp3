#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <string>
#include <vector>
#include <map>

#include "LogEntry.h"
#include "LogBase.h"


namespace openpal
{

/**
* Interface that represents a distinct logger with a name and running level
*/
class Logger
{

public:

	static int LogLevelToMask(LogLevel);

	Logger(ILogBase* apLog, LogLevel aLevel, const std::string& aName);	

	void Log( LogLevel aLogLevel, const std::string& arLocation, const std::string& aMessage, int aErrorCode = -1);

	void Log( const LogEntry& arEntry);

	const std::string& GetName() const {
		return mName;
	}

	// functions for manipulating filter levels
	inline bool IsEnabled(LogLevel aFilter) {
		return (mLevel & LogLevelToType(aFilter)) != 0;
	}

	inline void SetFilters(int aLevel) {
		mLevel = aLevel;
	}

	int GetFilters() const {
		return mLevel;
	}

	Logger GetSubLogger(std::string aSubName, LogLevel aLevel) const;	
	Logger GetSubLogger(std::string aName) const;

private:

	Logger(ILogBase* apLog, int aLevel, const std::string& aName);
	Logger GetSubLogger(std::string aSubName, int aLevel) const;

	int					mLevel;   // bit field describing what is being logged
	ILogBase*			mpLog;
	std::string			mName;
};

}

#endif

