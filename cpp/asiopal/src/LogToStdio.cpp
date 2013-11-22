
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

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
	oss << ToNormalizedString(time) << openpal::LogTypes::GetLevelString( arEntry.GetFilterLevel()) << " - "
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
