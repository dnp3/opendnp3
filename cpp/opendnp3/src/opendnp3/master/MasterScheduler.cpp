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

#include <openpal/MonotonicTimestamp.h>

using namespace openpal;

namespace opendnp3
{

MasterScheduler::DelayedTask::DelayedTask() : expiration(MonotonicTimestamp::Max()), pTask(nullptr)
{}

MasterScheduler::DelayedTask::DelayedTask(const openpal::MonotonicTimestamp& expiration_, IMasterTask* pTask_) :
	expiration(expiration_),
	pTask(pTask_)
{}

MasterScheduler::MasterScheduler(openpal::Logger* pLogger, ISOEHandler* pSOEHandler, IUTCTimeSource* pTimeSource, openpal::IExecutor& executor) :	
	tasks(pLogger, pSOEHandler, pTimeSource),	
	isOnline(false),
	isStartupComplete(false),
	pExecutor(&executor),
	pTimer(nullptr)
{

}

void MasterScheduler::ScheduleLater(IMasterTask* pTask, const openpal::TimeDuration& delay)
{	
	auto expiration = pExecutor->GetTime().Add(delay);	
	scheduledQueue.Enqueue(DelayedTask(expiration, pTask));
	
	if (pTimer)
	{		
		if (expiration < pTimer->ExpiresAt())
		{
			this->CancelAnyTimer();
			this->StartTimer(delay);
		}		
	}
	else
	{
		this->StartTimer(delay);
	}
}

void MasterScheduler::StartTimer(const openpal::TimeDuration& timeout)
{
	auto callback = [this](){ this->OnTimerExpiration(); };
	pTimer = pExecutor->Start(timeout, Bind(callback));
}

void MasterScheduler::Schedule(IMasterTask* pTask)
{	
	this->pendingQueue.Enqueue(pTask);
	this->expirationHandler.Run();	
}

void MasterScheduler::Demand(IMasterTask* pTask)
{
	auto equals = [pTask](const DelayedTask& dt) { return dt.pTask == pTask; };	
	if (scheduledQueue.RemoveFirst(equals))
	{
		this->Schedule(pTask);
	}
}

IMasterTask* MasterScheduler::Start()
{	
	if (commandActions.IsNotEmpty())
	{		
		// configure the command task		
		commandActions.Pop().Run(&tasks.commandTask);
		return &tasks.commandTask;
	}
	else
	{		
		if (pendingQueue.IsEmpty())
		{			
			return nullptr;			
		}
		else
		{
			auto pFront = pendingQueue.Peek();
			
			// Are there any sequenced tasks scheduled with priority >= pFront?
			auto match = [pFront](const DelayedTask& dt) { return dt.pTask->IsSequenced() && dt.pTask->Priority() <= pFront->Priority(); };

			return scheduledQueue.Contains(match) ? nullptr : pendingQueue.Pop();			
		}		
	}
}

void MasterScheduler::ResetTimerAndQueues()
{	
	this->CancelAnyTimer();	
	this->pendingQueue.Clear();
	this->scheduledQueue.Clear();
}

void MasterScheduler::SetExpirationHandler(const openpal::Runnable& runnable)
{
	this->expirationHandler = runnable;
}

void MasterScheduler::ScheduleCommand(const CommandErasure& action)
{
	if (commandActions.IsFull())
	{	
		this->ReportFailure(action, CommandResult::QUEUE_FULL);
	}
	else
	{
		commandActions.Enqueue(action);
		this->expirationHandler.Run();
	}
}

PollTask* MasterScheduler::AddPollTask(const PollTask& pt)
{
	auto pNode = pollTasks.AddAndGetPointer(pt);
	if (pNode)
	{		
		if (isOnline)
		{
			this->ScheduleLater(&pNode->value, pt.GetPeriod());
		}

		return &pNode->value;
	}
	else
	{
		return nullptr;
	}
}

void MasterScheduler::OnLowerLayerUp(const MasterParams& params)
{
	if (!isOnline)
	{
		isOnline = true;

		if (params.disableUnsolOnStartup)
		{
			this->Schedule(&tasks.disableUnsol);
		}

		if (params.startupIntergrityClassMask != 0)
		{
			this->Schedule(&tasks.startupIntegrity);
		}

		if (params.unsolClassMask & ALL_EVENT_CLASSES)
		{
			this->Schedule(&tasks.enableUnsol);
		}

		this->SchedulePollTasks(nullptr);
	}	
}



void MasterScheduler::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;

		this->ResetTimerAndQueues();

		while (commandActions.IsNotEmpty())
		{
			this->ReportFailure(commandActions.Pop(), CommandResult::NO_COMMS);
		}
	}	
}

void MasterScheduler::SchedulePollTasks(IMasterTask* pCurrent)
{
	auto schedule = [this, pCurrent](PollTask& pt) { 
		if (&pt != pCurrent)
		{
			this->ScheduleLater(&pt, pt.GetPeriod());
		}		
	};
	pollTasks.Foreach(schedule);
}

void MasterScheduler::OnRestartDetected(IMasterTask* pCurrentTask, const MasterParams& params)
{
	if (pCurrentTask != &tasks.clearRestartTask && !tasks.clearRestartTask.IsFailed())
	{		
		this->ResetTimerAndQueues();

		this->Schedule(&tasks.clearRestartTask);

		if (params.startupIntergrityClassMask != 0)
		{
			this->Schedule(&tasks.startupIntegrity);
		}

		if (params.unsolClassMask & ALL_EVENT_CLASSES)
		{
			this->Schedule(&tasks.enableUnsol);
		}

		this->SchedulePollTasks(pCurrentTask);		
	}	
}

void MasterScheduler::OnNeedTimeDetected(IMasterTask* pCurrentTask, const MasterParams& params)
{
	if (params.timeSyncMode == TimeSyncMode::SerialTimeSync)
	{
		if (pCurrentTask != &tasks.serialTimeSync && !this->IsPendingOrScheduled(&tasks.serialTimeSync))
		{
			tasks.serialTimeSync.Reset();
			this->Schedule(&tasks.serialTimeSync);
			this->expirationHandler.Run();
		}
	}	
}

void MasterScheduler::ReportFailure(const CommandErasure& action, CommandResult result)
{
	ConstantCommandProcessor processor(CommandResponse::NoResponse(result), pExecutor);
	action.Run(&processor);
}

void MasterScheduler::OnTimerExpiration()
{
	pTimer = nullptr;
	auto now = pExecutor->GetTime();	
	
	// move all expired tasks to the run queue
	while (scheduledQueue.IsNotEmpty() && scheduledQueue.Peek().expiration.milliseconds <= now.milliseconds)
	{		
		pendingQueue.Enqueue(scheduledQueue.Pop().pTask);
	}

	if (scheduledQueue.IsNotEmpty())
	{
		auto next = scheduledQueue.Peek().expiration;
		auto callback = [this](){ this->OnTimerExpiration(); };
		pTimer = pExecutor->Start(next, Bind(callback));
	}	

	if (pendingQueue.IsNotEmpty())
	{
		this->expirationHandler.Run();
	}
}

bool MasterScheduler::CancelAnyTimer()
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

bool MasterScheduler::IsPending(IMasterTask* pTask)
{
	auto same = [pTask](IMasterTask* pElem) { return pTask == pElem; };
	return this->pendingQueue.Contains(same);
}

bool MasterScheduler::IsScheduled(IMasterTask* pTask)
{
	auto same = [pTask](const DelayedTask& dt) { return pTask == dt.pTask; };
	return this->scheduledQueue.Contains(same);
}

bool MasterScheduler::IsPendingOrScheduled(IMasterTask* pTask)
{
	return IsPending(pTask) || IsScheduled(pTask);
}

}

