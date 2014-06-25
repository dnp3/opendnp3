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

#include "MasterScheduler.h"

#include "opendnp3/master/ConstantCommandProcessor.h"
#include "opendnp3/app/PointClass.h"

#include <openpal/executor/MonotonicTimestamp.h>

using namespace openpal;

namespace opendnp3
{

MasterScheduler::MasterScheduler(	openpal::Logger* pLogger, 
									MasterTasks& tasks,
									openpal::IExecutor& executor,
									IScheduleCallback& callback
									) :	
	pExecutor(&executor),
	pCallback(&callback),
	pStaticTasks(&tasks),
	isOnline(false),	
	modifiedSinceLastRead(false),
	pTimer(nullptr),
	pCurrentTask(nullptr)
{

}

void MasterScheduler::ScheduleLater(IMasterTask* pTask, const openpal::TimeDuration& delay)
{	
	
}

void MasterScheduler::Schedule(IMasterTask* pTask)
{	
}

void MasterScheduler::Demand(IMasterTask* pTask)
{
	
}

IMasterTask* MasterScheduler::Start()
{
	modifiedSinceLastRead = false;
	return FindTaskToStart();	 
}

IMasterTask* MasterScheduler::FindTaskToStart()
{		
	// TODO
	return nullptr;
}

void MasterScheduler::CheckForNotification()
{
	if (!modifiedSinceLastRead)
	{
		modifiedSinceLastRead = true;
		pCallback->OnPendingTask();
	}
}

void MasterScheduler::ScheduleCommand(const CommandErasure& action)
{
	// TODO
	this->ReportFailure(action, CommandResult::NO_COMMS);
}

PollTask* MasterScheduler::AddPollTask(const PollTask& pt)
{	
	// TODO
	return nullptr;
}


void MasterScheduler::OnRestartDetected(const MasterParams& params)
{
	// TODO
}

void MasterScheduler::OnNeedTimeDetected(const MasterParams& params)
{
	// TODO
}

void MasterScheduler::OnLowerLayerUp(const MasterParams& params)
{
	if (!isOnline)
	{
		isOnline = true;
		pCallback->OnPendingTask();
	}	
}



void MasterScheduler::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
				
	}	
}

void MasterScheduler::ReportFailure(const CommandErasure& action, CommandResult result)
{
	ConstantCommandProcessor processor(CommandResponse::NoResponse(result), pExecutor);
	action.Apply(processor);
}

void MasterScheduler::OnTimerExpiration()
{
	// TODO
}

void MasterScheduler::StartTimer(const openpal::TimeDuration& timeout)
{
	auto callback = [this](){ this->OnTimerExpiration(); };
	pTimer = pExecutor->Start(timeout, Runnable::Bind(callback));
}


bool MasterScheduler::CancelScheduleTimer()
{
	if (pTimer)
	{
		pTimer->Cancel();
		pTimer = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

}

