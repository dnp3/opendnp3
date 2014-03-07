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
#ifndef __ASYNC_TASK_GROUP_H_
#define __ASYNC_TASK_GROUP_H_

#include "opendnp3/Uncopyable.h"
#include "opendnp3/master/AsyncTaskInterfaces.h"

#include <openpal/MonotonicTimestamp.h>

#include <set>
#include <queue>
#include <string>

namespace openpal
{
class IExecutor;
class ITimer;
}

namespace opendnp3
{

class AsyncTaskBase;
class AsyncTaskPeriodic;
class AsyncTaskNonPeriodic;
class AsyncTaskContinuous;
class ITimeSource;

/**
 A collection of related tasks with optional dependencies
*/
class AsyncTaskGroup : private Uncopyable
{
	friend class AsyncTaskBase;
	friend class AsyncTaskContinuous;

public:

	AsyncTaskGroup(openpal::IExecutor*);
	~AsyncTaskGroup();

	AsyncTaskBase* Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	AsyncTaskContinuous* AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	bool Remove(AsyncTaskBase* apTask);

	void Shutdown();

	void Enable();
	void Disable();

	void Enable(int aMask);
	void Disable(int aMask);

	void ResetTasks(int aMask);

	void CheckState();

	bool IsRunning() {
		return mIsRunning;
	}

	openpal::MonotonicTimestamp GetCurrentTime() const;

private:

	void OnCompletion();
	void RestartTimer(const openpal::MonotonicTimestamp& arTime);
	void OnTimerExpiration();
	void Update(const  openpal::MonotonicTimestamp& arTime);
	AsyncTaskBase* GetNext(const  openpal::MonotonicTimestamp& arTime);

	bool mIsRunning;
	bool mShutdown;
	openpal::IExecutor* mpExecutor;
	openpal::ITimer* mpTimer;

	typedef std::vector< AsyncTaskBase* > TaskVec;
	TaskVec mTaskVec;
};

}

#endif
