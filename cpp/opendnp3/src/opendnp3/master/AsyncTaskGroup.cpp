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
#include "AsyncTaskGroup.h"

#include "AsyncTaskBase.h"
#include "AsyncTaskPeriodic.h"
#include "AsyncTaskNonPeriodic.h"
#include "AsyncTaskContinuous.h"

#include <openpal/IExecutor.h>
#include <openpal/Location.h>
#include <openpal/Bind.h>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

AsyncTaskGroup::AsyncTaskGroup(IExecutor* apExecutor) :
	mIsRunning(false),
	mShutdown(false),
	mpExecutor(apExecutor),
	mpTimer(nullptr)
{

}

AsyncTaskGroup::~AsyncTaskGroup()
{
	this->Shutdown();
	tasks.Foreach([](AsyncTaskBase * p){ delete p; });	
}

AsyncTaskBase* AsyncTaskGroup::Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const openpal::Function1<AsyncTaskBase*>& callback, const std::string& arName)
{
	assert(!tasks.IsFull());

	AsyncTaskBase* pTask;
	if(aPeriod.GetMilliseconds() < 0)
		pTask = new AsyncTaskNonPeriodic(aRetryDelay, aPriority, callback, this, arName);
	else
		pTask = new AsyncTaskPeriodic(aPeriod, aRetryDelay, aPriority, callback, this, arName);

	tasks.Add(pTask);	
	return pTask;
}

void AsyncTaskGroup::ResetTasks(int aMask)
{
	tasks.Foreach(
		[&](AsyncTaskBase * p)
		{ 
		if (!p->IsRunning() && (p->GetFlags() & aMask)) p->Reset();
		}
	);	
}

AsyncTaskContinuous* AsyncTaskGroup::AddContinuous(int aPriority, const openpal::Function1<AsyncTaskBase*>& callback, const std::string& arName)
{
	assert(!tasks.IsFull());
	AsyncTaskContinuous* pTask = new AsyncTaskContinuous(aPriority, callback, this, arName);
	tasks.Add(pTask);
	return pTask;
}

bool AsyncTaskGroup::Remove(AsyncTaskBase* pTask)
{
	auto pNode = tasks.RemoveFirst([&](AsyncTaskBase * p) { return (p == pTask); });
	
	if (pNode)
	{
		delete pNode->value;
		return true;
	}
	else
	{
		return false;
	}
}

void AsyncTaskGroup::Shutdown()
{
	if(mpTimer)
	{
		mpTimer->Cancel();
		mpTimer = nullptr;
	}

	mShutdown = true;
}

void AsyncTaskGroup::Enable()
{
	tasks.Foreach(
		[&](AsyncTaskBase * p)
		{
			p->SilentEnable();
		}
	);
	this->CheckState();
}

void AsyncTaskGroup::Disable()
{
	tasks.Foreach(
		[&](AsyncTaskBase * p)
		{
			p->SilentDisable();
		}
	);
	this->CheckState();
}

void AsyncTaskGroup::Enable(int mask)
{
	tasks.Foreach(
		[&](AsyncTaskBase * p)
		{
			if ((p->GetFlags() & mask) != 0)
			{
				p->SilentEnable();
			}
		}
	);	
	this->CheckState();
}

void AsyncTaskGroup::Disable(int mask)
{
	tasks.Foreach(
		[&](AsyncTaskBase * p)
		{
			if ((p->GetFlags() & mask) != 0)
			{
				p->SilentDisable();
			}
		}
	);	
	this->CheckState();
}

AsyncTaskBase* AsyncTaskGroup::GetNext(const MonotonicTimestamp& arTime)
{
	this->Update(arTime);

	AsyncTaskBase* pMax = nullptr;
	tasks.Foreach(
		[&](AsyncTaskBase * p)
		{
			if (pMax == nullptr)
			{
				pMax = p;
			}
			else
			{				
				if (AsyncTaskBase::LessThanGroupLevel(pMax, p))
				{
					pMax = p;
				}
			}
		}
	);

	if (pMax && !pMax->IsRunning() && pMax->IsEnabled())
	{		
		return pMax;
	}
	else
	{
		return nullptr;
	}

	return pMax;
}

void AsyncTaskGroup::CheckState()
{
	if(!mShutdown)
	{
		auto now = mpExecutor->GetTime();
		AsyncTaskBase* pTask = GetNext(now);

		if(pTask == nullptr) return;
		if(pTask->NextRunTime().milliseconds == MonotonicTimestamp::Max().milliseconds) return;

		if(pTask->NextRunTime().milliseconds <= now.milliseconds)
		{
			mIsRunning = true;
			pTask->Dispatch();
		}
		else
		{
			this->RestartTimer(pTask->NextRunTime());
		}
	}
}

void AsyncTaskGroup::OnCompletion()
{
	assert(mIsRunning);
	mIsRunning = false;
	this->CheckState();
}


openpal::MonotonicTimestamp AsyncTaskGroup::GetCurrentTime() const
{
	return mpExecutor->GetTime();
}

void AsyncTaskGroup::Update(const MonotonicTimestamp& time)
{
	tasks.Foreach(
		[&](AsyncTaskBase * p) { p->UpdateTime(time); }		
	);	
}

void AsyncTaskGroup::RestartTimer(const openpal::MonotonicTimestamp& arTime)
{
	if(mpTimer)
	{
		if(mpTimer->ExpiresAt().milliseconds != arTime.milliseconds)
		{
			mpTimer->Cancel();
			mpTimer = nullptr;
		}
	}

	if (mpTimer == nullptr)
	{
		auto lambda = [this]() { this->OnTimerExpiration(); };
		mpTimer = mpExecutor->Start(arTime, Bind(lambda));
	}
}

void AsyncTaskGroup::OnTimerExpiration()
{
	mpTimer = nullptr;
	this->CheckState();
}

} // end ns
