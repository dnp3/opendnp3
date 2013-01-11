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
#ifndef __LOG_TESTER_H_
#define __LOG_TESTER_H_

#include <APL/Log.h>

namespace apl
{

class LogTester
{
public:
	LogTester(bool aImmediate = false);

	void Log(const std::string& aLocation, const std::string& aMessage);

	int ClearLog();
	int NextErrorCode();
	bool GetNextEntry(LogEntry& arEntry);
	bool IsLogErrorFree();

	EventLog mLog;

protected:
	Logger* mpTestLogger;
	LogEntryCircularBuffer mBuffer;


};


}

#endif
