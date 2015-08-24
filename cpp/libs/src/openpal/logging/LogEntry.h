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
#ifndef OPENPAL_LOGENTRY_H
#define OPENPAL_LOGENTRY_H

#include <cstdint>

#include "LogFilters.h"

namespace openpal
{

/**
* An event recorded by the logging framework
*/
class LogEntry
{

public:

	LogEntry();

	LogEntry(char const* alias, const LogFilters& filters, char const* location, char const* message, int errorCode);

	/// @return The alias of the logger that recorded the message
	char const*	GetAlias() const
	{
		return alias;
	}
	
	/// @return The place in the source code where the message was recorded
	char const*	GetLocation() const
	{
		return location;
	}

	/// @return body of the log message
	char const* GetMessage() const
	{
		return message;
	}

	/// @return the log level of the message
	const LogFilters& GetFilters() const
	{
		return filters;
	}
	
	/// @return the error code associated with the message
	int	GetErrorCode() const
	{
		return errorCode;
	}

private:

	char const*		alias;
	LogFilters		filters;		
	char const*		location;
	char const*		message;
	int				errorCode;
};

}

#endif
