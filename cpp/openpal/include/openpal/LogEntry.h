#ifndef __LOG_ENTRY_H_
#define __LOG_ENTRY_H_

#include "gen/LogLevel.h"

namespace openpal
{

/**
* An event recorded by the logging framework
*/
class LogEntry
{
	
public:

	LogEntry(): mErrorCode(-1)		
	{}

	LogEntry( LogLevel aLevel, const std::string& aDeviceName, const std::string& aLocation, const std::string& aMessage, int aErrorCode);

	/// @return The name of the logger that recorded the message
	const std::string&	GetDeviceName() const {
		return mDeviceName;
	}

	/// @return The place in the source code where the message was recorded
	const std::string&	GetLocation() const {
		return mLocation;
	}

	/// @return body of the log message
	const std::string&	GetMessage() const {
		return mMessage;
	}

	/// @return the log level of the message
	LogLevel GetLogLevel() const {
		return mLogLevel;
	}
	

	/// @return the error code associated with the message
	int	GetErrorCode() const {
		return mErrorCode;
	}

private:	

	LogLevel		mLogLevel;
	std::string		mDeviceName;
	std::string		mLocation;
	std::string		mMessage;	
	int				mErrorCode;	
};

}

#endif
