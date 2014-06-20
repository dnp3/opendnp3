/**
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
#include "LogTester.h"

#include <opendnp3/LogLevels.h>

using namespace openpal;

namespace opendnp3
{

LogRecord::LogRecord() :
	id(),
	filters(0),	
	location(),
	message(),
	errorCode(-1)
{}

LogRecord::LogRecord(const LogEntry& entry) : 
	id(entry.GetId()), 
	filters(entry.GetFilters()), 
	location(entry.GetLocation()),
	message(entry.GetMessage()),
	errorCode(entry.GetErrorCode())
{

}

LogTester::LogTester() : root(this, "test", levels::NORMAL), logger(root.GetLogger())
{

}


void LogTester::Log( const LogEntry& entry )
{
	mBuffer.push(entry);
}

int32_t LogTester::PopFilter()
{
	if (mBuffer.size() > 0)
	{
		auto flags = mBuffer.front().filters.GetBitfield();
		mBuffer.pop();
		return flags;	
	}
	else
	{
		return 0;
	}
}

bool LogTester::PopOneEntry(int32_t filter)
{
	if (mBuffer.size() == 1)
	{
		if (mBuffer.front().filters.IsSet(filter))
		{
			mBuffer.pop();
			return true;
		}
		else return false;
	}
	else return false;
}

bool LogTester::PopUntil(int32_t filter)
{
	while (!mBuffer.empty())
	{
		bool match = mBuffer.front().filters.IsSet(filter);
		mBuffer.pop();
		if (match)
		{
			return true;
		}
	}

	return false;
}

int LogTester::ClearLog()
{
	int max = -1;
	LogEntry le;
	while(!mBuffer.empty())
	{
		if(mBuffer.front().errorCode > max) max = le.GetErrorCode();
		mBuffer.pop();
	}

	return max;
}

void LogTester::Log(const std::string& location, const std::string& message)
{
	logger.Log(flags::EVENT, location.c_str(), message.c_str());
}

int LogTester::NextErrorCode()
{
	LogRecord rec;
	while(!mBuffer.empty())
	{
		rec = mBuffer.front();
		mBuffer.pop();
		if (rec.errorCode >= 0)
		{
			return rec.errorCode;
		}
	}
	return -1;
}

bool LogTester::GetNextEntry(LogRecord& record)
{
	if(mBuffer.empty()) return false;
	else
	{
		record = mBuffer.front();
		mBuffer.pop();
		return true;
	}
}

void LogTester::Pop(openpal::ILogHandler* pLog)
{
	LogRecord record;
	while (GetNextEntry(record))
	{
		LogEntry le(record.id.c_str(), record.filters, record.location.c_str(), record.message.c_str(), record.errorCode);
		pLog->Log(le);
	}
}

openpal::Logger LogTester::GetLogger()
{
	return root.GetLogger();
}

bool LogTester::IsLogErrorFree()
{
	return ClearLog() < 0;
}

}

