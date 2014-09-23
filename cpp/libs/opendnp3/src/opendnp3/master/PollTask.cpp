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

#include "PollTask.h"

namespace opendnp3
{

PollTask::PollTask()
{}

PollTask::PollTask(const Builder& builder_, const openpal::TimeDuration& period_, ISOEHandler* pSOEHandler_, openpal::Logger* pLogger_) :
	PollTaskBase(pSOEHandler_, pLogger_),
	builder(builder_),
	period(period_)
{}
		
void PollTask::BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq)
{		
	rxCount = 0;
	builder(request);
	request.SetFunction(FunctionCode::READ);
	request.SetControl(AppControlField::Request(seq));

	this->NotifyState(PollState::RUNNING);
}

openpal::TimeDuration PollTask::GetPeriod() const
{
	return period;
}

void PollTask::OnFailure(const MasterParams& params, IMasterScheduler& scheduler)
{
	this->NotifyState(PollState::FAILURE);
	if (this->period.GetMilliseconds() >= 0)
	{
		scheduler.Schedule(*this, params.taskRetryPeriod);		
	}
	else
	{
		scheduler.Schedule(*this, this->period);
	}
	
}

void PollTask::OnSuccess(const MasterParams& params, IMasterScheduler& scheduler)
{
	this->NotifyState(PollState::SUCCESS);
	scheduler.Schedule(*this, this->period);	
}

} //end ns



