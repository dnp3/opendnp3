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
#include "MasterSchedulerNew.h"

#include "opendnp3/master/TaskComparison.h"

#include <openpal/executor/MonotonicTimestamp.h>

#include <algorithm>

using namespace openpal;

namespace opendnp3
{

MasterSchedulerNew::MasterSchedulerNew(const std::shared_ptr<openpal::IExecutor>& executor) :
	executor(executor),
	taskStartTimeoutTimer(*executor)
{

}

void MasterSchedulerNew::Schedule(const std::shared_ptr<IMasterTask>& task)
{
	tasks.push_back(task);
	this->RecalculateTaskStartTimeout();
}

std::vector<std::shared_ptr<IMasterTask>>::iterator MasterSchedulerNew::GetNextTask(const MonotonicTimestamp& now)
{
	auto runningBest = this->tasks.begin();

	if (!this->tasks.empty())
	{
		auto current = this->tasks.begin();
		++current;

		for (; current != this->tasks.end(); ++current)
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

std::shared_ptr<IMasterTask> MasterSchedulerNew::GetNext(const MonotonicTimestamp& now, MonotonicTimestamp& next)
{
	auto elem = GetNextTask(now);

	if (elem == this->tasks.end())
	{
		next = MonotonicTimestamp::Max();
		return nullptr;
	}
	else
	{
		const bool EXPIRED = (*elem)->ExpirationTime().milliseconds <= now.milliseconds;

		if (EXPIRED)
		{
			std::shared_ptr<IMasterTask> ret = *elem;
			this->tasks.erase(elem);
			return ret;
		}
		else
		{
			next = (*elem)->ExpirationTime();
			return nullptr;
		}
	}
}

void MasterSchedulerNew::Shutdown(const MonotonicTimestamp& now)
{
	this->taskStartTimeoutTimer.Cancel();
	this->tasks.clear();
}

void MasterSchedulerNew::CheckTaskStartTimeout()
{
	auto isTimedOut = [now = this->executor->GetTime()](const std::shared_ptr<IMasterTask>& task) -> bool
	{
		// TODO - make this functionality a method on the task itself

		if (task->IsRecurring() || task->StartExpirationTime() > now)
		{
			return false;
		}

		task->OnStartTimeout(now);

		return true;
	};

	// erase-remove idion (https://en.wikipedia.org/wiki/Erase-remove_idiom)
	this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), isTimedOut), this->tasks.end());
}

void MasterSchedulerNew::RecalculateTaskStartTimeout()
{
	auto min = MonotonicTimestamp::Max();

	for(auto& task : this->tasks)
	{
		if (!task->IsRecurring() && (task->StartExpirationTime() < min))
		{
			min = task->StartExpirationTime();
		}
	}

	auto callback = [this]()
	{
		this->CheckTaskStartTimeout();
	};

	this->taskStartTimeoutTimer.Restart(min, callback);

}

}

