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

#include "LogRoot.h"

#include "LogEntry.h"
#include "LogBase.h"

namespace openpal
{

LogRoot::LogRoot(ILogBase* pLog_, char const* id_, const LogFilters& filters_) : 
	pLog(pLog_), id(id_), filters(filters_)
{}

void LogRoot::Log(const LogFilters& filters, int subType, char const* location, char const* message, int errorCode)
{
	if(pLog)
	{
		LogEntry le(id, filters, subType, location, message, errorCode);
		pLog->Log(le);	
	}	
}

Logger LogRoot::GetLogger(int subType)
{
	return Logger(this, subType);
}

bool LogRoot::IsEnabled(const LogFilters& rhs) const
{
	return pLog && (this->filters & rhs);
}

void LogRoot::SetFilters(const LogFilters& filters_)
{
	filters = filters_;
}

const LogFilters& LogRoot::GetFilters() const
{
	return filters;
}

}
