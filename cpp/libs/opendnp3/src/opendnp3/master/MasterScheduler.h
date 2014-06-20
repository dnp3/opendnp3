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

#ifndef __MASTER_SCHEDULER_H_
#define __MASTER_SCHEDULER_H_

#include <openpal/container/StaticPriorityQueue.h>
#include <openpal/executor/Function1.h>
#include <openpal/executor/IExecutor.h>

#include "opendnp3/master/MasterTasks.h"
#include "opendnp3/master/PollTask.h"
#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/IMasterScheduler.h"

#include "opendnp3/StaticSizeConfiguration.h"

namespace opendnp3
{

class MasterScheduler : public IMasterScheduler
{	
	enum class State
	{
		STARTUP,
		READY
	};

public:

	typedef openpal::Function1<ICommandProcessor*> CommandErasure;

	MasterScheduler(openpal::Logger* pLogger, ISOEHandler* pSOEHandler, openpal::IUTCTimeSource* pTimeSource, openpal::IExecutor& executor);

	// ---------- Implement IMasterScheduler ------------
	
	virtual void ScheduleLater(IMasterTask* pTask, const openpal::TimeDuration& delay) override final;
	
	virtual void Schedule(IMasterTask* pTask) override final;
	
	virtual void Demand(IMasterTask* pTask) override final;

	// ---------- other public functions ----------------

	/**
	* @return Task to start or nullptr if no tasks are available
	*/
	IMasterTask* Start();

	/*
	* Startup
	*/
	void OnLowerLayerUp(const MasterParams& params);

	/**
	* Cleanup all existing tasks & cancel any timers
	*/
	void OnLowerLayerDown();

	/**
	* Set the action that will be called when a scheduled task is ready to run
	*/
	void SetExpirationHandler(const openpal::Runnable& runnable);

	/*
	* Schedule a command to run
	*/
	void ScheduleCommand(const CommandErasure& action);	

	/**
	* Add a new poll to the scheduler
	*/
	PollTask* AddPollTask(const PollTask& pt);

	/*
	* Called when the master observes the IIN::DeviceRestart bit
	*/
	void OnRestartDetected(const MasterParams& params);

	/*
	* Called when the master observes the IIN::NeedTime bit
	*/
	void OnNeedTimeDetected(const MasterParams& params);

private:	

	IMasterTask* FindTaskToStart();

	void CheckForNotification();

	void ResetTimerAndQueues();

	bool IsStartupComplete();

	void SchedulePollTasks();

	void ReportFailure(const CommandErasure& action, CommandResult result);

	class DelayedTask
	{
	public:

		DelayedTask();

		DelayedTask(const openpal::MonotonicTimestamp& expiration_, IMasterTask* pTask_);

		openpal::MonotonicTimestamp expiration;
		IMasterTask* pTask;
	};

	struct TimeBasedOrdering
	{
		static bool IsLessThan(const DelayedTask& lhs, const DelayedTask& rhs)
		{
			return lhs.expiration.milliseconds < rhs.expiration.milliseconds;
		}
	};

	void StartTimer(const openpal::TimeDuration& timeout);

	void OnTimerExpiration();	

	bool CancelAnyTimer();
	
	MasterTasks tasks;
			
	bool isOnline;
	bool isStartupComplete;
	bool modifiedSinceLastRead;
	openpal::IExecutor* pExecutor;
	openpal::ITimer* pTimer;
	openpal::Runnable expirationHandler;

	/// Tasks that are scheduled to execute ASAP
	openpal::StaticPriorityQueue<IMasterTask*, uint16_t, sizes::MAX_MASTER_TASKS, IMasterTask::Ordering> pendingQueue;
	
	/// Tasks that are scheduled to execute sometime in the future
	openpal::StaticPriorityQueue<DelayedTask, uint16_t, sizes::MAX_MASTER_TASKS, TimeBasedOrdering> scheduledQueue;	
	
	/// All pending command actions
	openpal::StaticQueue<CommandErasure, uint8_t, sizes::MAX_COMMAND_QUEUE_SIZE> commandActions;

	/// All poll tasks that have been configured
	openpal::StaticLinkedList<PollTask, uint8_t, sizes::MAX_MASTER_POLL_TASKS> pollTasks;
};

}



#endif
