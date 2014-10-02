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
#ifndef OPENDNP3_IMASTERTASK_H
#define OPENDNP3_IMASTERTASK_H

#include <openpal/logging/Logger.h>
#include <openpal/executor/MonotonicTimestamp.h>
#include <openpal/executor/IExecutor.h>

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDURequest.h"

#include "opendnp3/master/MasterParams.h"
#include "opendnp3/master/TaskResult.h"
#include "opendnp3/gen/TaskState.h"

#include <functional>

namespace opendnp3
{

typedef std::function<void(APDURequest&, uint8_t seq)> APDUBuilder;

/**
 * A generic interface for defining master request/response style tasks
 */
class IMasterTask
{
	
public:		

	/**	
	*
	* @return	the name of the task
	*/
	virtual char const* Name() const = 0;		

	/**
	* The task's priority. Lower numbers are higher priority.
	*/
	virtual int Priority() const = 0;

	/**
	* Allows tasks to enter a blocking mode where lower priority
	* tasks cannot run until this task completes
	*/
	virtual bool BlocksLowerPriority() const = 0;

	/**
	* Indicates if the task should be rescheduled (true) or discarded
	* after a single execution (false)
	*/
	virtual bool IsRecurring() const = 0;

	/**
	* Configures the task to run ASAP
	*/
	virtual void Demand() = 0;

	/**
	* The time when this task can run again.
	*/
	virtual openpal::MonotonicTimestamp ExpirationTime() const = 0;

	/**
	 * Build a request APDU.
	 *	
	 */
	virtual void BuildRequest(APDURequest& request, uint8_t seq) = 0;

	/**
	 * Handler for responses
	 *	 	 	
	 */
	virtual TaskResult OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now) = 0;
	
	/**
	 * Called when a response times out	 
	 */
	virtual void OnResponseTimeout(const openpal::MonotonicTimestamp& now) = 0;

	/**
	* Called when the layer closes while the task is executing.
	* The task can always be deleted when this event happens.
	*/
	virtual void OnLowerLayerClose(const openpal::MonotonicTimestamp& now) = 0;

	/*
	* Helper function that determines if the tasks is enabled. Setting the expiration time to max (infinity)
	* disables the task.
	*/
	bool IsEnabled() const { return !ExpirationTime().IsMax(); }


	void SetTaskCallback(const std::function<void(TaskState)>& callback_)
	{
		callback = callback_;
	}

	void Notify(openpal::IExecutor* pExecutor, TaskState state)
	{
		if (callback)
		{
			auto lambda = [this, state](){
				this->callback(state);
			};
			pExecutor->PostLambda(lambda);
		}		
	}

	private:

	std::function<void(TaskState)> callback;

};

}

#endif
