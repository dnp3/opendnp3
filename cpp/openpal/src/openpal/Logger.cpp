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
#include <openpal/Logger.h>

#include <openpal/LogBase.h>

#include <assert.h>
#include <sstream>

using namespace std;

namespace openpal
{

Logger::Logger(ILogBase* pLog_, uint32_t filters_, const std::string& name_)
	:
	filters(filters_),
	pLog(pLog_),
	name(name_)
{

}

Logger Logger::GetSubLogger(std::string subName, uint32_t filters_) const
{
	std::ostringstream oss;
	oss << name << "." << subName;
	return Logger(pLog, filters, oss.str());
}

Logger Logger::GetSubLogger(std::string aSubName) const
{
	return this->GetSubLogger(aSubName, filters);
}

void Logger::Log( const LogEntry& entry)
{
	if (this->IsEnabled(entry.GetFlags()))
	{
		pLog->Log(entry);
	}
}

void Logger::Log(uint32_t flags, const std::string& location, const std::string& message, int errorCode)
{
	if(this->IsEnabled(flags))
	{
		LogEntry le(flags, name, location, message, errorCode);
		pLog->Log(le);
	}
}

}

