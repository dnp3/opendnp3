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

#include "IntegrityPoll.h"

#include "opendnp3/app/PointClass.h"
#include "opendnp3/app/APDUParser.h"

#include "opendnp3/master/MeasurementHandler.h"

#include "opendnp3/objects/Group60.h"
#include "opendnp3/LogLevels.h"

#include <openpal/LogMacros.h>

namespace opendnp3
{

IntegrityPoll::IntegrityPoll(ISOEHandler* pSOEHandler_, openpal::Logger* pLogger_, const MasterParams& params) :
	pSOEHandler(pSOEHandler_),
	pLogger(pLogger_),
	pParams(&params),
	rxCount(0)
{
	
}

char const* IntegrityPoll::Name() const
{
	return "Integrity Poll";
}
	
IMasterTask::TaskPriority IntegrityPoll::Priority() const
{
	return IMasterTask::TaskPriority::POLL;
}
	
void IntegrityPoll::BuildRequest(APDURequest& request)
{
	rxCount = 0;

	request.SetFunction(FunctionCode::READ);
	auto writer = request.GetWriter();
	if (pParams->intergrityClassMask & CLASS_3) writer.WriteHeader(Group60Var4::ID, QualifierCode::ALL_OBJECTS);
	if (pParams->intergrityClassMask & CLASS_2) writer.WriteHeader(Group60Var3::ID, QualifierCode::ALL_OBJECTS);
	if (pParams->intergrityClassMask & CLASS_1) writer.WriteHeader(Group60Var2::ID, QualifierCode::ALL_OBJECTS);
	if (pParams->intergrityClassMask & CLASS_0) writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS);
}
	
TaskStatus IntegrityPoll::OnResponse(const APDUResponseRecord& response, IMasterScheduler& scheduler)
{
	if (response.control.FIR)
	{
		if (rxCount > 0)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected FIR frame");
			scheduler.ScheduleLater(this, pParams->taskRetryPeriod);
			return TaskStatus::FAIL;
		}
		else
		{
			return ProcessMeasurements(response, scheduler);
		}
	}
	else
	{
		if (rxCount > 0)
		{
			return ProcessMeasurements(response, scheduler);
		}
		else
		{			
			scheduler.ScheduleLater(this, pParams->taskRetryPeriod);
			SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected non-FIR frame");
			return TaskStatus::FAIL;
		}
	}
}

void IntegrityPoll::OnResponseTimeout(IMasterScheduler& scheduler)
{
	scheduler.ScheduleLater(this, pParams->taskRetryPeriod);
}
	
TaskStatus IntegrityPoll::ProcessMeasurements(const APDUResponseRecord& response, IMasterScheduler& scheduler)
{
	++rxCount;
	MeasurementHandler handler(*pLogger, pSOEHandler);
	auto result = APDUParser::ParseTwoPass(response.objects, &handler, pLogger);
	if (result == APDUParser::Result::OK)
	{
		if (response.control.FIN)
		{
			if (pParams->integrityPeriod > 0)
			{
				scheduler.ScheduleLater(this, pParams->integrityPeriod);
			}

			return TaskStatus::SUCCESS;
		}
		else
		{
			return TaskStatus::CONTINUE;
		}		
	}
	else
	{
		scheduler.ScheduleLater(this, pParams->taskRetryPeriod);
		return TaskStatus::FAIL;
	}
}

} //end ns
