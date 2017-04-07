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
#include "MasterSchedulerBackend.h"

#include <algorithm>

using namespace openpal;

namespace opendnp3
{

MasterSchedulerBackend::MasterSchedulerBackend(const std::shared_ptr<openpal::IExecutor>& executor) :
	executor(executor)
{

}

void MasterSchedulerBackend::Add(const std::shared_ptr<IMasterTask>& task, const std::shared_ptr<IMasterTaskRunner>& runner)
{
	this->tasks.push_back(Record(task, runner));
	this->CheckForTaskRun();
}

void MasterSchedulerBackend::RemoveTasks(const std::shared_ptr<IMasterTaskRunner>& runner)
{
	auto ownedByRunner = [&runner](const Record & record) -> bool { return record.runner == runner;  };

	// move erase idiom
	this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), ownedByRunner), this->tasks.end());
}

bool MasterSchedulerBackend::Complete()
{
	if (!this->current) return false;

	if (this->current.task->IsRecurring())
	{
		this->Add(this->current.task, this->current.runner);
	}

	this->current.Clear();
	return true;
}

void MasterSchedulerBackend::PostCheckForTaskRun()
{
	if (!this->taskCheckPending)
	{
		this->taskCheckPending = true;
		this->executor->Post([this]()
		{
			this->CheckForTaskRun();
		});
	}
}

bool MasterSchedulerBackend::CheckForTaskRun()
{
	this->taskCheckPending = false;

	if (this->current) return false;

	const auto now = this->executor->GetTime();

	// try to find a task that can run
	auto best_task = this->tasks.begin();
	if (best_task == this->tasks.end()) return false;
	auto other = ++best_task;

	while (other != this->tasks.end())
	{
		if (ShouldOtherRunBeforeCurrent(now, *best_task, *other))
		{
			best_task = other;
		}

		++other;
	}

	this->current = *best_task;
	this->tasks.erase(best_task);
	this->current.runner->Run(this->current.task);
	return true;
}

bool MasterSchedulerBackend::ShouldOtherRunBeforeCurrent(const openpal::MonotonicTimestamp& now, const Record& current, const Record& other)
{
	const auto BOTH_TASKS_ENABLED = IsEnabled(current) && IsEnabled(other);

	// if both tasks aren't enabled, only run the other task if it is enabled
	if (!BOTH_TASKS_ENABLED)
	{
		return IsEnabled(other);
	}

	const auto FROM_SAME_RUNNER = current.runner == other.runner;
	const auto CURRENT_HAS_BETTER_PRIORITY = current.task->Priority() < other.task->Priority();

	// If they're form the same runner, the current task is blocking, and the current task has better priority return false
	if (FROM_SAME_RUNNER && current.task->BlocksLowerPriority() && CURRENT_HAS_BETTER_PRIORITY)
	{
		return false;
	}

	// find out which tasks are expired
	const auto CURRENT_EXPIRED = current.task->ExpirationTime().milliseconds <= now.milliseconds;
	const auto OTHER_EXPIRED = other.task->ExpirationTime().milliseconds <= now.milliseconds;
	const auto OTHER_HAS_BETTER_PRIORITY = other.task->Priority() < current.task->Priority();

	if (CURRENT_EXPIRED) return OTHER_EXPIRED && OTHER_HAS_BETTER_PRIORITY;

	if (OTHER_EXPIRED) return true;

	// both tasks are not expired, so compare based on time
	// if the expirations times are equal, run other if it has better priority
	if (other.task->ExpirationTime() == current.task->ExpirationTime()) return OTHER_HAS_BETTER_PRIORITY;

	// only run other if its expiration time comes before the current
	return other.task->ExpirationTime() < current.task->ExpirationTime();
}

bool MasterSchedulerBackend::IsEnabled(const Record& record)
{
	return !record.task->ExpirationTime().IsMax();
}

}


