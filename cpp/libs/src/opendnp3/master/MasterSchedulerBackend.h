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
#ifndef OPENDNP3_MASTERSCHEDULERBACKEND_H
#define OPENDNP3_MASTERSCHEDULERBACKEND_H

#include "opendnp3/master/IMasterTaskRunner.h"
#include "opendnp3/master/IMasterScheduler.h"

#include "openpal/executor/TimerRef.h"

#include <vector>
#include <memory>

namespace opendnp3
{

class MasterSchedulerBackend final : public IMasterScheduler
{

	// Tasks are associated with a particular runner
	struct Record
	{
		Record() = default;

		Record(
		    const std::shared_ptr<IMasterTask>& task,
		    IMasterTaskRunner& runner
		) :
			task(task),
			runner(&runner)
		{}

		operator bool()
		{
			return task && runner;
		}

		void Clear()
		{
			this->task.reset();
			this->runner = nullptr;
		}

		bool BelongsTo(const IMasterTaskRunner& runner) const
		{
			return this->runner == &runner;
		}

		std::shared_ptr<IMasterTask> task;
		IMasterTaskRunner* runner = nullptr;

	};

public:

	explicit MasterSchedulerBackend(const std::shared_ptr<openpal::IExecutor>& executor);

	void Shutdown();

	// ------- implement IMasterScheduler --------

	virtual void Add(const std::shared_ptr<IMasterTask>& task, IMasterTaskRunner& runner) override;

	virtual void SetRunnerOffline(const IMasterTaskRunner& runner) override;

	virtual bool CompleteCurrentFor(const IMasterTaskRunner& runner) override;

	virtual void Demand(const std::shared_ptr<IMasterTask>& task) override;

	virtual void Evaluate() override;

private:

	bool taskCheckPending = false;

	Record current;
	std::vector<Record> tasks;

	void PostCheckForTaskRun();

	bool CheckForTaskRun();

	void RestartTimeoutTimer();

	void TimeoutTasks();

	const std::shared_ptr<openpal::IExecutor> executor;
	openpal::TimerRef taskTimer;
	openpal::TimerRef taskStartTimeout;


	enum class Comparison : uint8_t
	{
		LEFT,
		RIGHT,
		SAME
	};

	static Comparison GetBestTaskToRun(const openpal::MonotonicTimestamp& now, const Record& left, const Record& right);

	static Comparison CompareEnabledStatus(const Record& left, const Record& right);

	static Comparison CompareBlockedStatus(const Record& left, const Record& right);

	static Comparison ComparePriority(const Record& left, const Record& right);

	static Comparison CompareTime(const openpal::MonotonicTimestamp& time, const Record& left, const Record& right);
};

}



#endif
