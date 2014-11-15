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

#include "PollTaskBase.h"

#include "opendnp3/master/MeasurementHandler.h"

#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

PollTaskBase::PollTaskBase(
		const std::string& name_,
		ISOEHandler* pSOEHandler_,
		const openpal::Logger& logger_) :
	name(name_),
	pSOEHandler(pSOEHandler_),
	logger(logger_),
	rxCount(0)
{
	
}

const char* PollTaskBase::Name() const
{
	return name.empty() ? "user poll" : name.c_str();
}
	
IMasterTask::Result PollTaskBase::OnResponse(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{
	if (header.control.FIR)
	{
		if (rxCount > 0)
		{
			SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected FIR frame");
			this->OnFailure(now);
			return Result::FAILURE;
		}
		else
		{			
			return ProcessMeasurements(header, objects, now);
		}
	}
	else
	{
		if (rxCount > 0)
		{			
			return ProcessMeasurements(header, objects, now);
		}
		else
		{	
			SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected non-FIR frame");			
			this->OnFailure(now);			
			return Result::FAILURE;
		}
	}
}

void PollTaskBase::OnResponseTimeout(const openpal::MonotonicTimestamp& now)
{	
	this->OnFailure(now);	
}

IMasterTask::Result PollTaskBase::ProcessMeasurements(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{	
	++rxCount;

	if (MeasurementHandler::ProcessMeasurements(objects, &logger, pSOEHandler))
	{	
		if (header.control.FIN)
		{								
			this->OnSuccess(now);
			return Result::SUCCESS;
		}
		else
		{
			return Result::CONTINUE;
		}		
	}
	else
	{				
		this->OnFailure(now);
		return Result::FAILURE;
	}
}

} //end ns
