#ifndef __LOG_BASE_H_
#define __LOG_BASE_H_

#include "LogEntry.h"

namespace openpal
{

/**
* Callback interface for log messages
*/
class ILogBase
{
public:
	virtual ~ILogBase() {}

	/**
	* Callback function for log messages
	* @param arEntry the log message
	*/
	virtual void Log( const LogEntry& arEntry ) = 0;
};

}

#endif

