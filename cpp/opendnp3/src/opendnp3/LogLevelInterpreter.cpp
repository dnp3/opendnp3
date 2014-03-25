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

#include "LogLevelInterpreter.h"
#include "LogLevels.h"

namespace opendnp3 
{	
	void ProcessFlags(std::ostringstream& ss, const std::string& id, uint32_t& count, uint32_t filter, const openpal::LogFilters& flags)
	{
		if (flags.IsSet(filter))
		{
			if (count > 0)
			{
				ss << "/";
			}
			ss << id;
			++count;
		}
	}

	std::ostringstream& AllFlags(std::ostringstream& ss, const openpal::LogFilters& filters)
	{
		uint32_t count = 0;
		ProcessFlags(ss, "V", count, levels::EVENT, filters);
		ProcessFlags(ss, "E", count, levels::ERR, filters);
		ProcessFlags(ss, "W", count, levels::WARN, filters);
		ProcessFlags(ss, "I", count, levels::INFO, filters);
		ProcessFlags(ss, "D", count, levels::DEBUG, filters);
		ProcessFlags(ss, "P", count, levels::INTERPRET, filters);
		ProcessFlags(ss, "C", count, levels::COMM, filters);
		return ss;
	}
	
}

