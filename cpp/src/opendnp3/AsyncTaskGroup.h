
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __ASYNC_TASK_GROUP_H_
#define __ASYNC_TASK_GROUP_H_


#include <opendnp3/Types.h>
#include <opendnp3/Uncopyable.h>

#include "AsyncTaskInterfaces.h"
#include "TimeSource.h"

#include <set>
#include <queue>
#include <chrono>
#include <string>

namespace opendnp3
{

class AsyncTaskBase;
class AsyncTaskPeriodic;
class AsyncTaskNonPeriodic;
class AsyncTaskContinuous;
class IExecutor;
class ITimeSource;
class ITimer;

/**
 A collection of related tasks with optional dependencies
*/
class AsyncTaskGroup : private Uncopyable
{
	friend class AsyncTaskBase;
	friend class AsyncTaskContinuous;

public:

	AsyncTaskGroup(IExecutor*, ITimeSource* = TimeSource::Inst());
	~AsyncTaskGroup();

	AsyncTaskBase* Add(millis_t aPeriod, millis_t aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	AsyncTaskContinuous* AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	void Remove(AsyncTaskBase* apTask);

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

	std::chrono::steady_clock::time_point GetUTC() const;

private:

	void OnCompletion();
	void RestartTimer(const std::chrono::steady_clock::time_point& arTime);
	void OnTimerExpiration();
	void Update(const std::chrono::steady_clock::time_point& arTime);
	AsyncTaskBase* GetNext(const std::chrono::steady_clock::time_point& arTime);

	bool mIsRunning;
	bool mShutdown;
	IExecutor* mpExecutor;
	ITimeSource* mpTimeSrc;
	ITimer* mpTimer;



	typedef std::vector< AsyncTaskBase* > TaskVec;
	TaskVec mTaskVec;
};

}

#endif
