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

#ifndef OPENDNP3_MASTERSCHEDULER_H
#define OPENDNP3_MASTERSCHEDULER_H

#include <openpal/executor/Function0.h>
#include <openpal/executor/IExecutor.h>
#include <openpal/container/Settable.h>

#include "opendnp3/master/MasterTasks.h"
#include "opendnp3/master/PollTask.h"
#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/IMasterScheduler.h"
#include "opendnp3/master/TaskRecord.h"
#include "opendnp3/master/IScheduleCallback.h"
#include "opendnp3/master/TaskBitmask.h"

#include <deque>
#include <list>
#include <vector>
#include <functional>

namespace opendnp3
{

class MasterScheduler : public IMasterScheduler
{		

public:

	typedef std::function<void (ICommandProcessor&)>  CommandErasure;

	MasterScheduler(	openpal::Logger* pLogger,
						MasterTasks& tasks,
						openpal::IExecutor& executor,			
						IScheduleCallback& callback
					);

	// ---------- Implement IMasterScheduler ----------- 
	
	virtual void Schedule(IMasterTask& task, const openpal::TimeDuration& delay) override final;

	virtual void SetBlocking(IMasterTask& task, const openpal::TimeDuration& delay) override final;
	
	virtual bool Demand(IMasterTask& task) override final;

	// ---------- other public functions ----------------

	/**
	* @return Task to start or nullptr if no tasks are available
	*/
	IMasterTask* Start(const MasterParams& params);

	/*
	* Startup
	*/
	void OnLowerLayerUp(const MasterParams& params);

	/**
	* Cleanup all existing tasks & cancel any timers
	*/
	void OnLowerLayerDown();	

	/*
	* Schedule a command to run
	*/
	void ScheduleUserTask(const openpal::Function0<IMasterTask*>& task);

	/**
	* Add a new poll to the scheduler
	* @return an id that can be used to 
	*/
	PollTask* AddPollTask(const PollTask& task);

	/*
	* Called when the master observes the IIN::DeviceRestart bit
	*/
	void ProcessRxIIN(const IINField& iin, const MasterParams& params);	

private:	

	IMasterTask* FindTaskToStart(const MasterParams& params);	

	IMasterTask* GetScheduledTask(const MasterParams& params);

	bool CanTaskRun(IMasterTask& task, tasks::TaskBitmask bitmask, const MasterParams& params);

	IMasterTask* GetPeriodicTask(const MasterParams& params, const openpal::MonotonicTimestamp& now);	

	void ReportFailure(const CommandErasure& action, CommandResult result);	
	
	void StartOrRestartTimer(const openpal::MonotonicTimestamp& expiration);

	void StartTimer(const openpal::TimeDuration& timeout);

	void StartTimer(const openpal::MonotonicTimestamp& expiration);

	void OnTimerExpiration();	

	bool CancelScheduleTimer();	

	bool IsTaskActive(IMasterTask* pTask);

	bool IsAnyTaskActive() const;

	// ----------- static configuration ---------

	openpal::IExecutor* pExecutor;
	IScheduleCallback* pCallback;
	MasterTasks* pStaticTasks;

	// ----------- dynamic state -----------

	bool isOnline;	
	int32_t scheduledTaskMask;

	openpal::ITimer* pTimer;
	IMasterTask* pCurrentTask;	

	openpal::Settable<TaskRecord> blockingTask;

	std::list<PollTask> pollTasks;	
	std::vector<TaskRecord> periodicTasks;
	std::deque<openpal::Function0<IMasterTask*>> userTasks;
	
};

}



#endif
