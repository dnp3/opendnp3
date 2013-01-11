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
#include "LogTester.h"

#include <APL/LogToStdio.h>

namespace apl
{

LogTester::LogTester(bool aImmediate) :
	mLog(),
	mpTestLogger(mLog.GetLogger(LEV_DEBUG, "LogTester")),
	mBuffer(100)
{
	mLog.AddLogSubscriber(&mBuffer);
	if(aImmediate) mLog.AddLogSubscriber(LogToStdio::Inst());
}

int LogTester::ClearLog()
{
	int max = -1;
	LogEntry le;
	while(mBuffer.ReadLog(le)) {
		if(le.GetErrorCode() > max) max = le.GetErrorCode();
	}

	return max;
}

void LogTester::Log(const std::string& arLocation, const std::string& arMessage)
{
	mpTestLogger->Log(LEV_EVENT, arLocation, arMessage);
}

int LogTester::NextErrorCode()
{
	LogEntry le;
	while(mBuffer.ReadLog(le)) {
		if(le.GetErrorCode() >= 0) return le.GetErrorCode();
	}
	return -1;
}

bool LogTester::GetNextEntry(LogEntry& arEntry)
{
	return mBuffer.ReadLog(arEntry);
}

bool LogTester::IsLogErrorFree()
{
	return ClearLog() < 0;
}

}

