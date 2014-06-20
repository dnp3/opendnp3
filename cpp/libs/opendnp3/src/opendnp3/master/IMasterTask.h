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
#ifndef __I_MASTER_TASK_H_
#define __I_MASTER_TASK_H_

#include <openpal/logging/Logger.h>

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDURequest.h"

#include "opendnp3/master/MasterParams.h"
#include "opendnp3/master/TaskResult.h"
#include "opendnp3/master/IMasterScheduler.h"

namespace opendnp3
{

enum class TaskState
{
	IDLE,
	PENDING,
	SCHEDULED,
	RUNNING,
	FAILED
};

/**
 * A generic interface for defining master request/response style tasks
 */
class IMasterTask
{
	friend class MasterScheduler;
	friend class MasterTasks;

public:

	IMasterTask();

	TaskState GetState() const { return state; }	

	enum class TaskPriority : int
	{		
		COMMAND,
		DISABLE_UNSOL,
		CLEAR_RESTART_IIN,
		TIME_SYNC,
		STARTUP_INTEGRITY,
		ENABLE_UNSOL,
		POLL				
	};

	struct Ordering
	{
		static bool IsLessThan(IMasterTask* lhs, IMasterTask* rhs)
		{
			return lhs->Priority() < rhs->Priority();
		}
	};


	/**
	* Returns the name of the task.
	*
	* @return	the name of the task
	*/
	virtual char const* Name() const = 0;

	/**
	* Flag that indicates if sequencing matters for lower priority tasks
	*/
	virtual bool IsSequenced() const = 0;

	/**
	* The priority of the task where higher numbers have higher proiority
	*/
	virtual TaskPriority Priority() const = 0;

	/**
	 * Build a request APDU.
	 *	
	 */
	virtual void BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq) = 0;

	/**
	 * Handler for responses
	 *	 	 	
	 */
	virtual TaskStatus OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler) = 0;
	
	/**
	 * Called when a response times out
	 */
	void OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler);

	/**
	* Called when the layer closes while the task is executing
	*/
	void OnLowerLayerClose();

	protected:

	/**
	* Called when the layer closes. Overridable to perform cleanup.
	*/
	virtual void _OnLowerLayerClose() {}

	virtual void _OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler) = 0;


	void SetState(TaskState state_) { state = state_; }

	TaskState state;


};

} //end ns


#endif
