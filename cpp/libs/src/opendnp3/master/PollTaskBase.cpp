/*
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

using namespace openpal;

namespace opendnp3
{

PollTaskBase::PollTaskBase(const std::shared_ptr<TaskContext>& context, IMasterApplication& application, ISOEHandler& handler, const TaskBehavior& behavior, openpal::Logger logger, TaskConfig config) :
	IMasterTask(context, application, behavior, logger, config),
	handler(&handler)
{

}

void PollTaskBase::Initialize()
{
	this->rxCount = 0;
}

IMasterTask::ResponseResult PollTaskBase::ProcessResponse(const APDUResponseHeader& header, const openpal::RSlice& objects)
{
	if (header.control.FIR)
	{
		if (this->rxCount > 0)
		{
			SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected FIR frame");
			return ResponseResult::ERROR_BAD_RESPONSE;
		}
		else
		{
			return ProcessMeasurements(header, objects);
		}
	}
	else
	{
		if (this->rxCount > 0)
		{
			return ProcessMeasurements(header, objects);
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected non-FIR frame");
			return ResponseResult::ERROR_BAD_RESPONSE;
		}
	}
}

IMasterTask::ResponseResult PollTaskBase::ProcessMeasurements(const APDUResponseHeader& header, const openpal::RSlice& objects)
{
	++rxCount;

	if (MeasurementHandler::ProcessMeasurements(objects, logger, handler) == ParseResult::OK)
	{
		return header.control.FIN ? ResponseResult::OK_FINAL : ResponseResult::OK_CONTINUE;
	}
	else
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
}

} //end ns
