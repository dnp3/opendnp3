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

#include "opendnp3/master/TaskComparison.h"

#include <openpal/executor/MonotonicTimestamp.h>

#include <algorithm>

using namespace openpal;

namespace opendnp3
{

MasterScheduler::MasterScheduler(openpal::IExecutor& executor, IScheduleCallback& callback) :	
	pExecutor(&executor),
	pCallback(&callback),	
	isOnline(false),	
	pTimer(nullptr)	
{

}

void MasterScheduler::Schedule(openpal::ManagedPtr<IMasterTask> pTask)
{
	tasks.push_back(std::move(pTask));
}

openpal::ManagedPtr<IMasterTask> MasterScheduler::Start()
{		
	return PopNextTask();	
}

std::vector<openpal::ManagedPtr<IMasterTask>>::iterator MasterScheduler::GetNextTask(const MonotonicTimestamp& now)
{
	auto runningBest = tasks.begin();
	
	if (!tasks.empty())	
	{		
		auto current = tasks.begin();
		++current;

		for (; current != tasks.end(); ++current)
		{
			auto result = TaskComparison::SelectHigherPriority(now, **runningBest, **current);
			if (result == TaskComparison::Result::Right)
			{
				runningBest = current;
			}
		}		
	}	
	
	return runningBest;	
}

openpal::ManagedPtr<IMasterTask> MasterScheduler::PopNextTask()
{	
	auto now = pExecutor->GetTime();
	auto elem = GetNextTask(now);	

	if (elem == tasks.end())
	{
		return ManagedPtr<IMasterTask>();
	}
	else
	{
		if ((*elem)->ExpirationTime().milliseconds <= now.milliseconds) 
		{
			ManagedPtr<IMasterTask> ret(std::move(*elem));
			tasks.erase(elem);
			return ret;
		}
		else
		{
			if (!(*elem)->ExpirationTime().IsMax())
			{
				this->StartOrRestartTimer((*elem)->ExpirationTime());
			}			
			return ManagedPtr<IMasterTask>();
		}		
	}	
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

		auto now = pExecutor->GetTime();			
		
		for (auto& pTask : tasks)
		{
			pTask->OnTaskDiscarded(now);
		}
		
		tasks.clear();
	}	
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

