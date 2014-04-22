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
#ifndef __LOG_ROOT_H_
#define __LOG_ROOT_H_

#include "LogEntry.h"
#include "Logger.h"
#include "Uncopyable.h"

namespace openpal
{

class ILogBase;

/**
* Interface that represents a distinct logger with a name and running level
*/
class LogRoot : Uncopyable
{
	static const uint8_t MAX_ID_SIZE = 20;

public:

	LogRoot(ILogBase* apLog, char const* id_, const LogFilters& filters);	

	void Log(const LogFilters& filters, int subType, char const* location, char const* message, int errorCode);	

	Logger GetLogger(int subType = -1);

	bool IsEnabled(const LogFilters& rhs) const;

	void SetFilters(const LogFilters& filters_);

	const LogFilters& GetFilters() const;

private:
	
	ILogBase*		pLog;
	char			id[MAX_ID_SIZE];
	LogFilters		filters;   // bit field describing what is being logged
	
};

}

#endif
