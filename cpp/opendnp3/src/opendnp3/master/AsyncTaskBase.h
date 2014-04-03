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
#ifndef __ASYNC_TASK_BASE_H_
#define __ASYNC_TASK_BASE_H_

#include "opendnp3/master/AsyncTaskInterfaces.h"

#include <openpal/Uncopyable.h>
#include <openpal/MonotonicTimestamp.h>
#include <openpal/StaticList.h>
#include <openpal/Function1.h>



namespace opendnp3
{

class AsyncTaskGroup;

/**
 * Asynchronous task. Task execution order is controlled by the period, retry,
 * priority (for resolving ties) and task dependencies.
 *
 */
class AsyncTaskBase : public ITask, private openpal::Uncopyable
{
	friend class AsyncTaskGroup;
	friend class TrackingTaskGroup;

public:

	virtual ~AsyncTaskBase() {}

	// Implements ITaskCompletion
	void OnComplete(bool aSuccess, bool silent);

	// void SetStatusCallback(const std::function<void (bool)>& callback);

	// Modify this task's depth to make it dependent on the argument
	bool AddDependency(const AsyncTaskBase* apTask);
	bool IsDependency(const AsyncTaskBase*) const;

	// Do the task now if it is enabled, regardless of periodicity
	void Demand();

	void SetFlags(int aFlags)
	{
		mFlags = aFlags;
	}
	int GetFlags()
	{
		return mFlags;
	}

	void Enable();  // Enable the task and notify the task group which might execute another task
	void Disable(); // Disable ''

	void SilentEnable(); // Enable without notifying the task group
	void SilentDisable();	

	static bool LessThan(const AsyncTaskBase* l, const AsyncTaskBase* r);
	static bool LessThanGroupLevel(const AsyncTaskBase* l, const AsyncTaskBase* r);
	static bool LessThanGroupLevelNoString(const AsyncTaskBase* l, const AsyncTaskBase* r);

protected:

	AsyncTaskBase(
	    int aPriority,
		const openpal::Function1<AsyncTaskBase*>& handler,
	    AsyncTaskGroup* apGroup,
	    const openpal::MonotonicTimestamp& arInitialTime);

	// optional NVII function for special bookkeeping
	virtual void _OnComplete(bool aSuccess) {}
	//virtual void _Reset() {}


	// Reset the task to it's original state
	void Reset();
	
	openpal::StaticList<const AsyncTaskBase*, uint16_t, 16> dependencies;

	// Run the task if it is not currently executing
	virtual void Dispatch();

	// Update the task's completion and expired status
	// base upon the input time
	void UpdateTime(const openpal::MonotonicTimestamp& arTime);

	bool IsEnabled() const
	{
		return mIsEnabled;
	}

	// @returns priority used to resolve ties
	int Priority() const
	{
		return mPriority;
	}

	// @returns the completion status of the task
	bool IsComplete() const
	{
		return mIsComplete;
	}

	// @return wether the task is expired
	bool IsExpired() const
	{
		return mIsExpired;
	}

	// @return wether the task is running
	bool IsRunning() const
	{
		return mIsRunning;
	}

	// @returns max_date_time if the task is currently running or will not run again
	openpal::MonotonicTimestamp NextRunTime() const
	{
		return mNextRunTime;
	}
	
	bool mIsEnabled;								// Tasks can be enabled or disabled
	bool mIsComplete;								// Every task has a flag that
	// executes it's completion status
	bool mIsExpired;								// Indicate wether the time from
	// the last UpdateTime call >=
	// mNextRunTime
	bool mIsRunning;								// Every task has an execution
	// status
	int mPriority;									// Every task has a pr
	openpal::Function1<AsyncTaskBase*> handler;				// Every task has a handler for
	// executing the task
	AsyncTaskGroup* mpGroup;						// owning task group
	openpal::MonotonicTimestamp mNextRunTime;		// next execution time for the task
	const openpal::MonotonicTimestamp M_INITIAL_TIME;
	int mFlags;

	//std::function<void (bool)> callback;
};

}

#endif
