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

void MasterSchedulerBackend::Add(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner)
{
	this->tasks.push_back(Record(task, runner));
	this->CheckForTaskRun();
}

void MasterSchedulerBackend::RemoveTasksFor(const IMasterTaskRunner& runner)
{
	auto isOwnedByRunner = [&](const Record & record) -> bool { return record.runner == &runner;  };

	// move erase idiom
	this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), isOwnedByRunner), this->tasks.end());
}

bool MasterSchedulerBackend::CompleteCurrentFor(const IMasterTaskRunner& runner, bool reschedule)
{
	// no active task
	if (!this->current) return false;

	// active task not for this runner
	if (this->current.runner != &runner) return false;

	if (reschedule && this->current.task->IsRecurring())
	{
		this->Add(this->current.task, *this->current.runner);
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
		if (GetBestTaskToRun(now, *best_task, *other) == Comparison::RIGHT)
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

MasterSchedulerBackend::Comparison MasterSchedulerBackend::GetBestTaskToRun(const openpal::MonotonicTimestamp& now, const Record& left, const Record& right)
{
	const auto BEST_ENABLED = CompareEnabledStatus(left, right);

	if(BEST_ENABLED != Comparison::SAME)
	{
		// if one task is disabled, return the other task
		return BEST_ENABLED;
	}

	const auto EARLIEST_EXPIRATION = CompareTime(now, left, right);
	const auto BEST_PRIORITY = ComparePriority(left, right);

	if (left.runner == right.runner)
	{
		switch (BEST_PRIORITY)
		{
		case(Comparison::LEFT):
			if (left.task->BlocksLowerPriority()) return Comparison::LEFT;
			break;
		case(Comparison::RIGHT):
			if (right.task->BlocksLowerPriority()) return Comparison::RIGHT;
			break;
		default:
			break;
		}
	}

	// if the expiration times are the same, break based on priority, otherwise go with the expiration time
	return (EARLIEST_EXPIRATION == Comparison::SAME) ? BEST_PRIORITY : EARLIEST_EXPIRATION;
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareTime(const openpal::MonotonicTimestamp& now, const Record& left, const Record& right)
{
	// if tasks are already expired, the effective expiration time is NOW
	const auto leftTime = (now > left.task->ExpirationTime()) ? left.task->ExpirationTime() : now;
	const auto rightTime = (now > right.task->ExpirationTime()) ? right.task->ExpirationTime() : now;

	if (leftTime > rightTime)
	{
		return Comparison::LEFT;
	}
	else if (rightTime > leftTime)
	{
		return Comparison::RIGHT;
	}
	else
	{
		return Comparison::SAME;
	}
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareEnabledStatus(const Record& left, const Record& right)
{
	if (left.task->ExpirationTime().IsMax()) // left is disabled, check the right
	{
		return right.task->ExpirationTime().IsMax() ? Comparison::SAME : Comparison::RIGHT;
	}
	else if(right.task->ExpirationTime().IsMax()) // left is enabled, right is disabled
	{
		return Comparison::LEFT;
	}
	else
	{
		// both tasks are enabled
		return Comparison::SAME;
	}
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::ComparePriority(const Record& left, const Record& right)
{
	if (left.task->Priority() < right.task->Priority())
	{
		return Comparison::LEFT;
	}
	else if (right.task->Priority() < right.task->Priority())
	{
		return Comparison::RIGHT;
	}
	else
	{
		return Comparison::SAME;
	}
}

}


