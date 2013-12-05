#include <asiopal/LogToStdio.h>

#include <ctime>

#include <chrono>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace asiopal
{

LogToStdio LogToStdio::mInstance;

LogToStdio::LogToStdio() : mPrintLocation(false)
{}

void LogToStdio::SetPrintLocation(bool aPrintLocation)
{
	mPrintLocation = aPrintLocation;
}

void LogToStdio::Log(const openpal::LogEntry& arEntry)
{
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	auto time = std::chrono::high_resolution_clock::now();
	ostringstream oss;
	oss << ToNormalizedString(time) << openpal::LogLevelToString( arEntry.GetLogLevel()) << " - "
	    << arEntry.GetDeviceName();
	if(mPrintLocation && !arEntry.GetLocation().empty()) oss << " - " << arEntry.GetMessage();
	oss << " - " << arEntry.GetMessage();

	if(arEntry.GetErrorCode() != -1) oss << " - " << arEntry.GetErrorCode();

	std::unique_lock<std::mutex> lock(mMutex);
	std::cout << oss.str() << std::endl;
#endif
}

std::string LogToStdio::ToNormalizedString(const std::chrono::high_resolution_clock::time_point& arTime)
{
	std::time_t t = std::chrono::high_resolution_clock::to_time_t(arTime);
	std::string ts = ctime(&t);    // convert to calendar time
	ts.resize(ts.size() - 6);      // skip trailing newline anbd year. TODO - find a better, safer impl!
	ostringstream oss;
	auto us = duration_cast<microseconds>(arTime.time_since_epoch()).count();
	auto fractional = us % (1000 * 1000);
	oss << ts << "." << fractional;
	return oss.str();
}


} //end ns
