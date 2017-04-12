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

#include "PeriodicTask.h"

namespace opendnp3
{

PeriodicTask::PeriodicTask(IMasterApplication& app, const PeriodicTaskConfig& pconfig, const openpal::Logger& logger, TaskConfig config) :
	IMasterTask(app, pconfig.initialExpiration, logger, config),
	retry(retry)
{}


IMasterTask::TaskState PeriodicTask::OnTaskComplete(TaskCompletion completion, openpal::MonotonicTimestamp now)
{
	switch (completion)
	{

	// retry immediately when the comms come back online
	case(TaskCompletion::FAILURE_NO_COMMS):
		return TaskState::Immediately();

	// back-off exponentially using the task retry
	case(TaskCompletion::FAILURE_RESPONSE_TIMEOUT):
		return TaskState::Retry(retry.GetRetryOnTimeout(now));

	case(TaskCompletion::SUCCESS):
		{
			retry.OnSuccess();
			return period.IsNegative() ? TaskState::Infinite() : TaskState::Retry(now.Add(period));
		}

	default:
		//anything else disables the task like format errors, or repsonse timeouts
		return TaskState::Disabled();
	}
}

}


