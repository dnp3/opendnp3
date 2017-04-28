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
	executor(executor),
	taskTimer(*executor),
	taskStartTimeout(*executor)
{

}

void MasterSchedulerBackend::Shutdown()
{
	this->tasks.clear();
	this->current.Clear();
	this->taskTimer.Cancel();
	this->taskStartTimeout.Cancel();
}

void MasterSchedulerBackend::Add(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner)
{
	this->tasks.push_back(Record(task, runner));
	this->PostCheckForTaskRun();
}

void MasterSchedulerBackend::SetRunnerOffline(const IMasterTaskRunner& runner)
{
	const auto now = this->executor->GetTime();

	auto checkForOwnership = [now, &runner](const Record & record) -> bool
	{
		if (record.BelongsTo(runner))
		{
			if (!record.task->IsRecurring())
			{
				record.task->OnLowerLayerClose(now);
			}

			return true;
		}
		else
		{
			return false;
		}
	};

	if (this->current && checkForOwnership(this->current)) this->current.Clear();

	// move erase idiom
	this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), checkForOwnership), this->tasks.end());

	this->PostCheckForTaskRun();
}

bool MasterSchedulerBackend::CompleteCurrentFor(const IMasterTaskRunner& runner)
{
	// no active task
	if (!this->current) return false;

	// active task not for this runner
	if (!this->current.BelongsTo(runner)) return false;

	if (this->current.task->IsRecurring())
	{
		this->Add(this->current.task, *this->current.runner);
	}

	this->current.Clear();

	this->PostCheckForTaskRun();

	return true;
}

void MasterSchedulerBackend::Demand(const std::shared_ptr<IMasterTask>& task)
{
	auto callback = [this, task]()
	{
		task->SetMinExpiration();
		this->CheckForTaskRun();
	};

	this->executor->Post(callback);
}

void MasterSchedulerBackend::Evaluate()
{
	this->PostCheckForTaskRun();
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

	this->RestartTimeoutTimer();

	if (this->current) return false;

	const auto now = this->executor->GetTime();

	// try to find a task that can run
	auto current = this->tasks.begin();
	auto best_task = current;
	if (current == this->tasks.end()) return false;
	++current;

	while (current != this->tasks.end())
	{
		if (GetBestTaskToRun(now, *best_task, *current) == Comparison::RIGHT)
		{
			best_task = current;
		}

		++current;
	}

	// is the task runnable now?
	const auto IS_EXPIRED = now.milliseconds >= best_task->task->ExpirationTime().milliseconds;
	if (IS_EXPIRED)
	{
		this->current = *best_task;
		this->tasks.erase(best_task);
		this->current.runner->Run(this->current.task);

		return true;
	}
	else
	{
		auto callback = [this]()
		{
			this->CheckForTaskRun();
		};

		this->taskTimer.Restart(best_task->task->ExpirationTime(), callback);

		return false;
	}
}

void MasterSchedulerBackend::RestartTimeoutTimer()
{
	auto min = MonotonicTimestamp::Max();

	for (auto& record : this->tasks)
	{
		if (!record.task->IsRecurring() && (record.task->StartExpirationTime() < min))
		{
			min = record.task->StartExpirationTime();
		}
	}

	this->taskStartTimeout.Restart(min, [this]()
	{
		this->TimeoutTasks();
	});
}

void MasterSchedulerBackend::TimeoutTasks()
{
	// find the minimum start timeout value
	auto isTimedOut = [now = this->executor->GetTime()](const Record & record) -> bool
	{
		if (record.task->IsRecurring() || record.task->StartExpirationTime() > now)
		{
			return false;
		}

		record.task->OnStartTimeout(now);

		return true;
	};

	// erase-remove idion (https://en.wikipedia.org/wiki/Erase-remove_idiom)
	this->tasks.erase(std::remove_if(this->tasks.begin(), this->tasks.end(), isTimedOut), this->tasks.end());

	this->RestartTimeoutTimer();
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::GetBestTaskToRun(const openpal::MonotonicTimestamp& now, const Record& left, const Record& right)
{
	const auto BEST_ENABLED_STATUS = CompareEnabledStatus(left, right);

	if(BEST_ENABLED_STATUS != Comparison::SAME)
	{
		// if one task is disabled, return the other task
		return BEST_ENABLED_STATUS;
	}

	const auto BEST_BLOCKED_STATUS = CompareBlockedStatus(left, right);

	if (BEST_BLOCKED_STATUS != Comparison::SAME)
	{
		// if one task is blocked and the other isn't, return the unblocked task
		return BEST_BLOCKED_STATUS;
	}

	const auto EARLIEST_EXPIRATION = CompareTime(now, left, right);
	const auto BEST_PRIORITY = ComparePriority(left, right);

	// if the expiration times are the same, break based on priority, otherwise go with the expiration time
	return (EARLIEST_EXPIRATION == Comparison::SAME) ? BEST_PRIORITY : EARLIEST_EXPIRATION;
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareTime(const openpal::MonotonicTimestamp& now, const Record& left, const Record& right)
{
	// if tasks are already expired, the effective expiration time is NOW
	const auto leftTime = left.task->IsExpired(now) ? now : left.task->ExpirationTime();
	const auto rightTime = right.task->IsExpired(now) ? now : right.task->ExpirationTime();

	if (leftTime < rightTime)
	{
		return Comparison::LEFT;
	}
	else if (rightTime < leftTime)
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

MasterSchedulerBackend::Comparison MasterSchedulerBackend::CompareBlockedStatus(const Record& left, const Record& right)
{
	if (left.task->IsBlocked())
	{
		return right.task->IsBlocked() ? Comparison::SAME : Comparison::RIGHT;
	}
	else
	{
		return right.task->IsBlocked() ? Comparison::LEFT : Comparison::SAME;
	}
}

MasterSchedulerBackend::Comparison MasterSchedulerBackend::ComparePriority(const Record& left, const Record& right)
{
	if (left.task->Priority() < right.task->Priority())
	{
		return Comparison::LEFT;
	}
	else if (right.task->Priority() < left.task->Priority())
	{
		return Comparison::RIGHT;
	}
	else
	{
		return Comparison::SAME;
	}
}

}


