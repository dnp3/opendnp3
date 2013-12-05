#ifndef __LOGGABLE_MACROS_H_
#define __LOGGABLE_MACROS_H_

#include "Location.h"

#include <sstream>

//macro to remove boiler-plate code for logging messages
#define LOG_BLOCK(severity, string) ERROR_BLOCK(severity, string, -1)

#define EXCEPTION_BLOCK(severity, ex) ERROR_BLOCK(severity, ex.GetErrorString(), ex.ErrorCode())

//macro to remove boiler-plate code for logging messages with an error code
#define ERROR_BLOCK(severity, string, code) ERROR_LOGGER_BLOCK(this->mLogger, severity, string, code)

#define LOGGER_BLOCK(logger, severity, string) ERROR_LOGGER_BLOCK(logger, severity, string, -1)

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define ERROR_LOGGER_BLOCK(logger, severity, string, code)\
	if(logger.IsEnabled(severity)){\
		std::ostringstream somecrazyname_oss;\
		somecrazyname_oss << string;\
		logger.Log(severity, LOCATION, somecrazyname_oss.str(), code);\
	}
#else
#define ERROR_LOGGER_BLOCK(logger, severity, string, code)\
	if(logger.IsEnabled(severity)){\
		logger->Log(severity, LOCATION, "", code); \
	}
#endif

#endif
