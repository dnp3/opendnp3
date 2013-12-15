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
#ifndef __TIME_SYNC_HANDLER_H_
#define __TIME_SYNC_HANDLER_H_

#include "HeaderHandlerBase.h"

#include <openpal/LoggableMacros.h>

namespace opendnp3
{

/**
 * Dedicated class for processing response data in the master.
 */
class TimeSyncHandler : public HeaderHandlerBase
{

public:
	
	/**
	* @param arLogger the Logger that the loader should use for message reporting
	*/
	TimeSyncHandler(openpal::Logger& aLogger) : 
		HeaderHandlerBase(aLogger), 
		valid(false), 
		timeOut(0)
	{}	

	std::string HandlerName() const final { return "TimeSyncHandler"; }

	virtual void _OnCountOf(const Iterable<Group52Var2>& times) final
	{
		if(times.Count() == 1)
		{
			valid = true;
			times.foreach([this](const Group52Var2& obj) { timeOut = obj.time16; });
		}
		else
		{
			LOG_BLOCK(openpal::LogLevel::Warning, "Ignoring unexpected time delay count of " << times.Count());
		}
	}

	bool GetTimeDelay(uint16_t& time) 
	{
		if(valid) time = timeOut;
		return valid;		
	}

private:
	bool valid;
	uint16_t timeOut;

};

}

/* vim: set ts=4 sw=4: */

#endif

