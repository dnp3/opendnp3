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

#include <openpal/executor/MonotonicTimestamp.h>

#include <algorithm>

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
	pTimer(nullptr),
	pCurrentTask(nullptr)	
{

}

IMasterTask* MasterScheduler::Start()
{	
	auto pTask = NextTask();
	this->pCurrentTask = pTask;
	return pTask;
}

bool MasterScheduler::TaskLessThan(IMasterTask* lhs, IMasterTask* rhs)
{
	if (lhs->IsEnabled())
	{
		if (rhs->IsEnabled())
		{
			
		}
		else
		{

		}
	}
}

IMasterTask* MasterScheduler::NextTask()
{
	auto elem = std::max_element(tasks.begin(), tasks.end(), TaskLessThan);

	if (elem == tasks.end())
	{
		return nullptr;
	}
	else
	{
		tasks.erase(elem);
		return *elem;
	}	
}

bool MasterScheduler::CanTaskRun(IMasterTask& task, tasks::TaskBitmask bitmask)
{
	return false;
}

void MasterScheduler::ProcessRxIIN(const IINField& iin, const MasterParams& params)
{
	/*
	if (iin.IsSet(IINBit::DEVICE_RESTART))
	{		
		if (!this->IsTaskActive(&pStaticTasks->clearRestartTask))
		{
			scheduledTaskMask |= tasks::CLEAR_RESTART_SEQUENCE;				
		}
	}

	if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && params.integrityOnEventOverflowIIN)
	{		
		if (!this->IsTaskActive(&pStaticTasks->startupIntegrity))
		{
			scheduledTaskMask |= tasks::STARTUP_INTEGRITY;
		}
	}

	if (iin.IsSet(IINBit::NEED_TIME))
	{		
		if (!this->IsTaskActive(&pStaticTasks->serialTimeSync))
		{
			scheduledTaskMask |= tasks::TIME_SYNC;
		}
	}
	
	if (scheduledTaskMask)
	{
		this->CancelScheduleTimer();
	}
	*/
}

void MasterScheduler::OnLowerLayerUp()
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
		this->CancelScheduleTimer();		
		pCurrentTask = nullptr;

		// TODO - fail all tasks		
	}	
}

void MasterScheduler::ReportFailure(const CommandErasure& action, CommandResult result)
{
	ConstantCommandProcessor processor(CommandResponse::NoResponse(result), pExecutor);
	action(processor);
}

void MasterScheduler::OnTimerExpiration()
{
	pTimer = nullptr;
	pCallback->OnPendingTask();
}

void MasterScheduler::StartOrRestartTimer(const openpal::MonotonicTimestamp& expiration)
{
	if (pTimer)
	{
		if (pTimer->ExpiresAt() > expiration)
		{
			this->CancelScheduleTimer();
			this->StartTimer(expiration);
		}
	}
	else
	{
		this->StartTimer(expiration);
	}
}

void MasterScheduler::StartTimer(const openpal::TimeDuration& timeout)
{
	auto callback = [this](){ this->OnTimerExpiration(); };
	pTimer = pExecutor->Start(timeout, Action0::Bind(callback));
}

void MasterScheduler::StartTimer(const openpal::MonotonicTimestamp& expiration)
{
	auto callback = [this](){ this->OnTimerExpiration(); };
	pTimer = pExecutor->Start(expiration, Action0::Bind(callback));
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

