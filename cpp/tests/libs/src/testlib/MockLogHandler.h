/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __LOG_TESTER_H_
#define __LOG_TESTER_H_

#include <openpal/logging/LogRoot.h>

#include <string>
#include <queue>

namespace testlib
{

class LogRecord
{
public:

	LogRecord();
	LogRecord(const openpal::LogEntry& entry);

	std::string		id;
	openpal::LogFilters		filters;
	std::string		location;
	std::string		message;
	int				errorCode;
};

class MockLogHandler : public openpal::ILogHandler
{

public:
	MockLogHandler(uint32_t filters = ~0);

	void WriteToStdIo();

	void Log(const std::string& location, const std::string& msg);

	void Log( const openpal::LogEntry& entry);

	int32_t PopFilter();

	bool PopOneEntry(int32_t filter);

	bool PopUntil(int32_t filter);

	bool PopErrorCode(int code);

	int ClearLog();
	int NextErrorCode();
	bool GetNextEntry(LogRecord& record);
	bool IsLogErrorFree();

	void Pop(openpal::ILogHandler& log);

	openpal::LogRoot root;

protected:

	bool outputToStdIO;
	std::deque<LogRecord> messages;

};


}

#endif
