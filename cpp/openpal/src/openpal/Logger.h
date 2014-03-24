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
#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <string>
#include <vector>
#include <map>

#include "LogEntry.h"
#include "LogBase.h"


namespace openpal
{

/**
* Interface that represents a distinct logger with a name and running level
*/
class Logger
{

public:

	Logger(ILogBase* apLog, uint32_t filters, const std::string& aName);

	void Log(uint32_t flags, const std::string& location, const std::string& message, int errorCode = -1);

	void Log( const LogEntry& arEntry);

	const std::string& GetName() const
	{
		return name;
	}

	// functions for manipulating filter levels
	inline bool IsEnabled(uint32_t flags)
	{
		return (filters & flags) != 0;
	}

	inline void SetFilters(uint32_t filters_)
	{
		filters = filters_;
	}

	uint32_t GetFilters() const
	{
		return filters;
	}
	
	Logger GetSubLogger(std::string aName) const;
	Logger GetSubLogger(std::string subName, uint32_t aLevel) const;

private:

	uint32_t			filters;   // bit field describing what is being logged
	ILogBase*			pLog;
	std::string			name;
};

}

#endif

