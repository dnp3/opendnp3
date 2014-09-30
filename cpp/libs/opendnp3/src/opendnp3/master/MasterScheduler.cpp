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
	auto pTask = PopNextTask();
	this->pCurrentTask = pTask;
	return pTask;
}

int MasterScheduler::Compare(const MonotonicTimestamp& now, IMasterTask* lhs, IMasterTask* rhs)
{
	// if they're both enabled we compare based on
	// blocking/priority/expiration time
	if (lhs->IsEnabled() && rhs->IsEnabled())
	{
		if ((lhs->Priority() > rhs->Priority()) && rhs->BlocksLowerPriority())
		{
			return 1; // rhs greater
		}
		else if (rhs->Priority() < lhs->Priority() && lhs->BlocksLowerPriority())
		{
			return -1; // lhs greater
		}
		else // equal priority or neither task blocks lower priority tasks, compare based on time
		{
			auto tlhs = lhs->ExpirationTime();
			auto trhs = rhs->ExpirationTime();
			auto lhsExpired = tlhs.milliseconds <= now.milliseconds;
			auto rhsExpired = trhs.milliseconds <= now.milliseconds;

			if (lhsExpired && rhsExpired)
			{
				// both expired, compare based on priority				
				return lhs->Priority() - rhs->Priority();
			}
			else
			{
				if (tlhs.milliseconds < trhs.milliseconds)
				{
					return -1;
				}
				else if (trhs.milliseconds < tlhs.milliseconds)
				{
					return 1;
				}
				else
				{
					// if equal times, compare based on priority
					return lhs->Priority() - rhs->Priority();
				}
			}			
		}
	}
	else 
	{
		// always prefer the enabled task over the one that isn't
		return rhs->IsEnabled() ? 1 : -1;
	}
}

std::vector<IMasterTask*>::iterator MasterScheduler::GetNextTask()
{
	auto runningBest = tasks.begin();
	
	if (!tasks.empty())	
	{
		auto now = pExecutor->GetTime();
		auto current = tasks.begin();
		++current;

		for (; current != tasks.end(); ++current)
		{
			auto cmp = Compare(now, *runningBest, *current);
			if (cmp > 0)
			{
				runningBest = current;
			}
		}
	}
	

	return runningBest;
}

IMasterTask* MasterScheduler::PopNextTask()
{		
	auto elem = GetNextTask();

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

