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
	outstationNeedsTime(false),
	pTimer(nullptr),
	pCurrentTask(nullptr),
	pStartupTask(nullptr)
{

}

void MasterScheduler::Schedule(IMasterTask& task, const openpal::TimeDuration& delay)
{	
	auto expiration = pExecutor->GetTime().Add(delay);
	this->periodicTasks.Add(TaskRecord(task, expiration));
	if (blockingTask.IsEmpty() && !this->pCurrentTask)
	{
		this->StartOrRestartTimer(expiration);
	}	
}

void MasterScheduler::SetBlocking(IMasterTask& task, const openpal::TimeDuration& delay)
{
	auto expiration = pExecutor->GetTime().Add(delay);
	this->blockingTask.Set(TaskRecord(task, expiration));
	this->CancelScheduleTimer();
	this->StartTimer(expiration);
}

bool MasterScheduler::Demand(IMasterTask& task)
{
	if (this->IsTaskActive(&task))
	{
		return true;
	}
	else
	{
		// TODO
		return false;
	}
}

IMasterTask* MasterScheduler::Start(const MasterParams& params)
{	
	auto pTask = FindTaskToStart(params);
	this->pCurrentTask = pTask;
	return pTask;
}

IMasterTask* MasterScheduler::FindTaskToStart(const MasterParams& params)
{		
	auto now = pExecutor->GetTime();	

	if (blockingTask.IsSet())
	{
		auto record = blockingTask.Get();		
		if (record.expiration.milliseconds <= now.milliseconds)
		{
			blockingTask.Clear();
			return record.pTask;
		}
		else
		{
			this->StartOrRestartTimer(record.expiration);
			return nullptr;
		}
	}
	else
	{
		// nothing blocking, so look at the startup sequence next
		auto startup = GetStartupTask(params);		
		if (startup)
		{
			return startup;
		}
		else
		{			
			auto time = GetTimeSyncTask(params);
			if (time)
			{
				return time;
			}
			else
			{
				return GetPeriodicTask(params, now);
			}			
		}
	}
}

IMasterTask* MasterScheduler::GetStartupTask(const MasterParams& params)
{
	if (pStartupTask)
	{
		auto pTask = CurrentOrNextEnabledTask(pStartupTask, params, *(this->pStaticTasks));		
		
		// advance by a single node, as the config may change dynamically
		pStartupTask = pTask ? pTask->Next(*(this->pStaticTasks)) : nullptr;
		return pTask;
	}
	else
	{
		return nullptr;
	}
}

IMasterTask* MasterScheduler::GetTimeSyncTask(const MasterParams& params)
{
	if (outstationNeedsTime)
	{
		switch (params.timeSyncMode)
		{
			case(TimeSyncMode::SerialTimeSync) :
				outstationNeedsTime = false;
				return &pStaticTasks->serialTimeSync;
				break;
			default:
				return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}


IMasterTask* MasterScheduler::CurrentOrNextEnabledTask(IMasterTask* pCurrent, const MasterParams& params, MasterTasks& tasks)
{
	if (pCurrent && pCurrent->Enabled(params))
	{
		return pCurrent;
	}
	else
	{
		return NextEnabledTask(pCurrent, params, tasks);
	}		
}

IMasterTask* MasterScheduler::NextEnabledTask(IMasterTask* pCurrent, const MasterParams& params, MasterTasks& tasks)
{	 
	if (pCurrent)
	{
		auto pNode = pCurrent->Next(tasks);
		
		while (pNode)
		{
			if (pNode->Enabled(params))
			{
				return pNode;
			}

			pNode = pNode->Next(tasks);
		}

		return pNode;
	}
	else
	{
		return nullptr;
	}	
}


IMasterTask* MasterScheduler::GetPeriodicTask(const MasterParams& params, const openpal::MonotonicTimestamp& now)
{
	auto pNode = this->GetEarliestExpirationTime();
	if (pNode)
	{
		if (pNode->value.expiration.milliseconds <= now.milliseconds)
		{
			periodicTasks.Adapter().Remove(pNode);
			return pNode->value.pTask;
		}
		else
		{
			this->StartOrRestartTimer(pNode->value.expiration);
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

ListNode<TaskRecord>* MasterScheduler::GetEarliestExpirationTime()
{
	openpal::ListNode<TaskRecord>* pNode = nullptr;
	// TODO - turn this min function into a reusable algo.
	auto iterator = periodicTasks.Adapter().Iterate();
	while (iterator.HasNext())
	{
		auto pCurrent = iterator.Next();
		if (pNode)
		{
			if (pCurrent->value.expiration < pNode->value.expiration)
			{
				pNode = pCurrent;
			}
		}
		else
		{
			pNode = pCurrent;
		}
	}
	return pNode;
}

void MasterScheduler::ScheduleCommand(const CommandErasure& action)
{
	// TODO
	this->ReportFailure(action, CommandResult::NO_COMMS);
}

PollTask* MasterScheduler::AddPollTask(const PollTask& pt)
{		
	if (periodicTasks.IsFull())
	{
		return nullptr;
	}
	else
	{		
		auto result = pollTasks.AddAndGetPointer(pt);
		if (result)
		{
			if (isOnline)
			{
				this->Schedule(result->value, pt.GetPeriod());
			}			
			return &result->value;
		}
		else
		{
			return nullptr;
		}		
	}	
}


void MasterScheduler::OnRestartDetected(const MasterParams& params)
{
	auto pRestartTask = &pStaticTasks->clearRestartTask;
	if (!this->IsTaskActive(pRestartTask))
	{
		pStartupTask = pRestartTask;
		this->CancelScheduleTimer(); // we have an active task now		
	}
}

void MasterScheduler::OnNeedTimeDetected(const MasterParams& params)
{
	auto pTime1 = &pStaticTasks->serialTimeSync;

	if (!IsTaskActive(pTime1))
	{
		outstationNeedsTime = true;
	}
}

bool MasterScheduler::IsTaskActive(IMasterTask* pTask)
{
	auto isEqual = [pTask] (const TaskRecord& tr) { return tr.pTask == pTask; };
	if (blockingTask.IsSetAnd(isEqual))
	{
		return true;
	}
	else
	{
		return pCurrentTask == pTask;
	}	
}

void MasterScheduler::OnLowerLayerUp(const MasterParams& params)
{
	if (!isOnline)
	{
		isOnline = true;
		pStartupTask = &pStaticTasks->disableUnsol;
		auto now = pExecutor->GetTime();
		auto addToPeriodic = [this, now](PollTask& pt) 
		{ 
			this->periodicTasks.Add(TaskRecord(pt, now.Add(pt.GetPeriod()))); 
		};
		pollTasks.Foreach(addToPeriodic);
		pCallback->OnPendingTask();
	}	
}



void MasterScheduler::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
		blockingTask.Clear();
		pCurrentTask = nullptr;
		periodicTasks.Clear();
		this->CancelScheduleTimer();
	}	
}

void MasterScheduler::ReportFailure(const CommandErasure& action, CommandResult result)
{
	ConstantCommandProcessor processor(CommandResponse::NoResponse(result), pExecutor);
	action.Apply(processor);
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
	pTimer = pExecutor->Start(timeout, Runnable::Bind(callback));
}

void MasterScheduler::StartTimer(const openpal::MonotonicTimestamp& expiration)
{
	auto callback = [this](){ this->OnTimerExpiration(); };
	pTimer = pExecutor->Start(expiration, Runnable::Bind(callback));
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

