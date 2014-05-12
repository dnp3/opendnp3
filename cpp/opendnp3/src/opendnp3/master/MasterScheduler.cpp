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

using namespace openpal;

namespace opendnp3
{

MasterScheduler::DelayedTask::DelayedTask() : expiration(MonotonicTimestamp::Max()), pTask(nullptr)
{}

MasterScheduler::DelayedTask::DelayedTask(const openpal::MonotonicTimestamp& expiration_, IMasterTask* pTask_) :
	expiration(expiration_),
	pTask(pTask_)
{}

MasterScheduler::MasterScheduler(openpal::Logger* pLogger, openpal::IExecutor& executor) :
	commandTask(pLogger),
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
			this->CancelTimer();
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
	auto wasEmpty = this->pendingQueue.IsEmpty();
	this->pendingQueue.Enqueue(pTask);
	if (wasEmpty)
	{
		this->expirationHandler.Run();
	}
}

IMasterTask* MasterScheduler::Start()
{	
	if (commandActions.IsEmpty())
	{
		if (pendingQueue.IsEmpty())
		{
			return nullptr;
		}
		else
		{
			return pendingQueue.Pop();
		}
	}
	else
	{
		// configure the command task		
		commandActions.Pop().Run(&commandTask);
		return &commandTask;
	}
}

void MasterScheduler::Reset()
{
	this->CancelTimer();
	this->pendingQueue.Clear();
	this->scheduledQueue.Clear();
	while (commandActions.IsNotEmpty())
	{		
		this->ReportFailure(commandActions.Pop(), CommandResult::NO_COMMS);
	}
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

void MasterScheduler::ReportFailure(const CommandErasure& action, CommandResult result)
{
	ConstantCommandProcessor processor(CommandResponse::NoResponse(result), pExecutor);
	action.Run(&processor);
}

void MasterScheduler::OnTimerExpiration()
{
	pTimer = nullptr;
	auto now = pExecutor->GetTime();
	auto wasEmpty = this->pendingQueue.IsEmpty();
	
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

	if (wasEmpty && pendingQueue.IsNotEmpty())
	{
		this->expirationHandler.Run();
	}
}

bool MasterScheduler::CancelTimer()
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

