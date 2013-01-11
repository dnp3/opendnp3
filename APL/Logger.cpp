//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "Logger.h"
#include <assert.h>
#include "Log.h"
#include <iostream>
using namespace std;


namespace apl
{
Logger::Logger(EventLog* apLog, FilterLevel aFilter, const std::string& aName)
	:
	mLevel(0),
	mpLog(apLog),
	mName(aName),
	mVarName(aName)
{
	this->SetFilterLevel(aFilter);
}

void Logger::SetFilterLevel(FilterLevel aFilter)
{
	mLevel = LogTypes::FilterLevelToMask(aFilter);
}

Logger* Logger::GetSubLogger(std::string aSubName, int aFilterBits)
{
	std::ostringstream oss;
	oss << mName << "." << aSubName;
	Logger* pLogger = mpLog->GetLogger(LEV_WARNING, oss.str());
	pLogger->SetVarName(mVarName);
	pLogger->mLevel = aFilterBits;
	return pLogger;
}

Logger* Logger::GetSubLogger(std::string aSubName, FilterLevel aFilter)
{
	return this->GetSubLogger(aSubName, LogTypes::FilterLevelToMask(aFilter));
}

Logger* Logger::GetSubLogger(std::string aSubName)
{
	return this->GetSubLogger(aSubName, this->mLevel);
}

void Logger::Log( const LogEntry& arEntry)
{
	if(this->IsEnabled(arEntry.GetFilterLevel())) mpLog->Log(arEntry);
}

void Logger::Log( FilterLevel aFilterLevel, const std::string& arLocation, const std::string& aMessage, int aErrorCode)
{
	if(this->IsEnabled(aFilterLevel)) {
		LogEntry le(aFilterLevel, mName, arLocation, aMessage, aErrorCode);
		mpLog->Log(le);
	}
}

void Logger::Set(const std::string& aVar, int aValue)
{
	mpLog->SetVar(mVarName, aVar, aValue);
}

}

