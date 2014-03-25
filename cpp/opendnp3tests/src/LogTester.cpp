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

LogTester::LogTester() : root(this, levels::ALL), logger(root.GetLogger("LogTester"))
{

}

void LogTester::Log( const LogEntry& entry )
{
	mBuffer.push(entry);
}

bool LogTester::PopOneEntry(int32_t filter)
{
	if (mBuffer.size() == 1)
	{
		if (mBuffer.front().GetFilters().IsSet(filter))
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
		bool match = mBuffer.front().GetFilters().IsSet(filter);
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
		if(mBuffer.front().GetErrorCode() > max) max = le.GetErrorCode();
		mBuffer.pop();
	}

	return max;
}

void LogTester::Log(const std::string& arLocation, const std::string& arMessage)
{
	logger.Log(levels::EVENT, arLocation, arMessage);
}

int LogTester::NextErrorCode()
{
	LogEntry le;
	while(!mBuffer.empty())
	{
		le = mBuffer.front();
		mBuffer.pop();
		if(le.GetErrorCode() >= 0)
		{
			return le.GetErrorCode();
		}
	}
	return -1;
}

bool LogTester::GetNextEntry(LogEntry& arEntry)
{
	if(mBuffer.empty()) return false;
	else
	{
		arEntry = mBuffer.front();
		mBuffer.pop();
		return true;
	}
}

void LogTester::Pop(openpal::ILogBase* pLog)
{
	LogEntry entry;
	while (GetNextEntry(entry))
	{
		pLog->Log(entry);
	}
}

openpal::Logger LogTester::GetLogger(const std::string& id)
{
	return root.GetLogger(id);
}

bool LogTester::IsLogErrorFree()
{
	return ClearLog() < 0;
}

}

