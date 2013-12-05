#include <openpal/LogEntry.h>

#include <sstream>

using namespace std;

namespace openpal
{


LogEntry::LogEntry( LogLevel aLevel, const std::string& aDeviceName, const std::string& aLocation, const std::string& aMessage, int aErrorCode)
	:
	mLogLevel(aLevel),
	mDeviceName(aDeviceName),
	mLocation(aLocation),
	mMessage(aMessage),	
	mErrorCode(aErrorCode)
{

}


}

