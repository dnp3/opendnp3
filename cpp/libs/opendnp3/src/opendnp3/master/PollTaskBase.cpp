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

PollTaskBase::PollTaskBase(ISOEHandler* pSOEHandler_, openpal::Logger* pLogger_) :
	pSOEHandler(pSOEHandler_),
	pLogger(pLogger_),	
	rxCount(0)
{
	
}
	
TaskStatus PollTaskBase::OnResponse(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler)
{
	if (header.control.FIR)
	{
		if (rxCount > 0)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected FIR frame");
			this->OnFailure(params, scheduler);
			return TaskStatus::FAIL;
		}
		else
		{			
			return ProcessMeasurements(header, objects, params, scheduler);
		}
	}
	else
	{
		if (rxCount > 0)
		{			
			return ProcessMeasurements(header, objects, params, scheduler);
		}
		else
		{	
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected non-FIR frame");			
			this->OnFailure(params, scheduler);			
			return TaskStatus::FAIL;
		}
	}
}

void PollTaskBase::OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler)
{	
	this->OnFailure(params, scheduler);
}
	
TaskStatus PollTaskBase::ProcessMeasurements(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler)
{	
	++rxCount;

	if (MeasurementHandler::ProcessMeasurements(objects, pLogger, pSOEHandler))
	{	
		if (header.control.FIN)
		{						
			this->OnSuccess(params, scheduler);
			return TaskStatus::SUCCESS;
		}
		else
		{
			return TaskStatus::CONTINUE;
		}		
	}
	else
	{			
		this->OnFailure(params, scheduler);
		return TaskStatus::FAIL;
	}
}

} //end ns
