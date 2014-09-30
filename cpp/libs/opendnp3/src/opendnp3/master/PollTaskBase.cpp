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
		const APDUBuilder& builder_,
		const std::string& name_,
		ISOEHandler* pSOEHandler_,
		openpal::Logger* pLogger_) :	
	name(name_),
	pSOEHandler(pSOEHandler_),
	pLogger(pLogger_),
	rxCount(0)	
{
	
}

void PollTaskBase::BuildRequest(APDURequest& request, uint8_t seq)
{
	rxCount = 0;
	builder(request, seq);
	request.SetFunction(FunctionCode::READ);
	request.SetControl(AppControlField::Request(seq));	
}

virtual const char* PollTaskBase::Name() const override final;

	
TaskState PollTaskBase::OnResponse(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{
	if (header.control.FIR)
	{
		if (rxCount > 0)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected FIR frame");
			this->OnFailure(now);
			return TaskState::SCHEDULED;
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
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected non-FIR frame");			
			this->OnFailure(now);			
			return TaskState::SCHEDULED;
		}
	}
}

bool PollTaskBase::OnResponseTimeout(const openpal::MonotonicTimestamp& now)
{	
	return false;
}

TaskState PollTaskBase::ProcessMeasurements(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{	
	++rxCount;

	if (MeasurementHandler::ProcessMeasurements(objects, pLogger, pSOEHandler))
	{	
		if (header.control.FIN)
		{									
			return TaskState::COMPLETE;
		}
		else
		{
			return TaskState::CONTINUE;
		}		
	}
	else
	{		
		// TODO - reschedule
		TaskState::SCHEDULED;
	}
}

} //end ns
