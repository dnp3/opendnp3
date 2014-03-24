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
#ifndef __LOG_ENTRY_H_
#define __LOG_ENTRY_H_

#include <cstdint>
#include <string>

namespace openpal
{

/**
* An event recorded by the logging framework
*/
class LogEntry
{

public:

	LogEntry(): errorCode(-1)
	{}

	LogEntry(uint32_t flags, const std::string& name, const std::string& location, const std::string& message, int errorCode);

	/// @return The name of the logger that recorded the message
	const std::string&	GetName() const
	{
		return name;
	}

	/// @return The place in the source code where the message was recorded
	const std::string&	GetLocation() const
	{
		return location;
	}

	/// @return body of the log message
	const std::string&	GetMessage() const
	{
		return message;
	}

	/// @return the log level of the message
	uint32_t GetFlags() const
	{
		return flags;
	}

	/// @return the error code associated with the message
	int	GetErrorCode() const
	{
		return errorCode;
	}

private:

	uint32_t		flags;
	std::string		name;
	std::string		location;
	std::string		message;
	int				errorCode;
};

}

#endif
