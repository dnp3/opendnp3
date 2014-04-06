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
#include "Logger.h"

#include "LogBase.h"
#include "LogRoot.h"

#include <assert.h>
#include <cstring>

#include "StringFormatting.h"

using namespace std;

namespace openpal
{

Logger::Logger(LogRoot* pRoot_, char const* name_) :
pRoot(pRoot_)
{
	strncpy(name, name_, MAX_NAME_SIZE);
}

Logger Logger::GetSubLogger(char const* subName) const
{
	char buffer[MAX_NAME_SIZE];
	SNPRINTF(buffer, MAX_NAME_SIZE, "%s - %s", name, subName);
	return Logger(pRoot, buffer);
}

bool Logger::IsEnabled(const LogFilters& filters) const
{
	return pRoot->IsEnabled(filters);
}

void Logger::Log( const LogEntry& entry)
{
	if (pRoot->IsEnabled(entry.GetFilters()))
	{
		pRoot->Log(entry);
	}
}

void Logger::Log(const LogFilters& filters, char const* location, char const* message, int errorCode)
{
	LogEntry le(filters, name, location, message, errorCode);
	this->Log(le);
}

}

