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
#ifndef OPENDNP3_IMASTERTASK_H
#define OPENDNP3_IMASTERTASK_H

#include <openpal/logging/Logger.h>
#include <openpal/executor/MonotonicTimestamp.h>
#include <openpal/executor/IExecutor.h>

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDURequest.h"

#include "opendnp3/master/TaskContext.h"
#include "opendnp3/master/TaskConfig.h"
#include "opendnp3/master/IMasterApplication.h"
#include "opendnp3/master/TaskBehavior.h"

namespace opendnp3
{

/**
 * A generic interface for defining master request/response style tasks
 */
class IMasterTask : private openpal::Uncopyable
{

public:

	enum class ResponseResult : uint8_t
	{
		/// The response was bad, the task has failed
		ERROR_BAD_RESPONSE,

		/// The response was good and the task is complete
		OK_FINAL,

		/// The response was good and the task should repeat the format, transmit, and await response sequence
		OK_REPEAT,

		/// The response was good and the task should continue executing. Restart the response timer, and increment expected SEQ#.
		OK_CONTINUE
	};

	IMasterTask(const std::shared_ptr<TaskContext>& context, IMasterApplication& app, const TaskBehavior& behavior, const openpal::Logger& logger, TaskConfig config);

	virtual ~IMasterTask();

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
	* Indicates if the task should be rescheduled (true) or discarded
	* after a single execution (false)
	*/
	virtual bool IsRecurring() const = 0;

	/**
	* The time when this task can run again.
	*/
	openpal::MonotonicTimestamp ExpirationTime() const;

	/**
	* Helper to test if the task is expired
	*/
	bool IsExpired(const openpal::MonotonicTimestamp& now) const
	{
		return now.milliseconds >= this->ExpirationTime().milliseconds;
	}

	/**
	* The time when this task expires if it is unable to start
	*/
	openpal::MonotonicTimestamp StartExpirationTime() const;

	/**
	 * Build a request APDU.
	 *
	 * Return false if some kind of internal error prevents the task for formatting the request.
	 */
	virtual bool BuildRequest(APDURequest& request, uint8_t seq) = 0;

	/**
	 * Handler for responses
	 */
	ResponseResult OnResponse(const APDUResponseHeader& response, const openpal::RSlice& objects, openpal::MonotonicTimestamp now);

	/**
	 * Called when a response times out
	 */
	void OnResponseTimeout(openpal::MonotonicTimestamp now);

	/**
	* Called when the layer closes while the task is executing.
	*/
	void OnLowerLayerClose(openpal::MonotonicTimestamp now);

	/**
	* The start timeout expired before the task could be run
	*/
	void OnStartTimeout(openpal::MonotonicTimestamp now);

	/**
	* Called when the master is unable to format the request associated with the task
	*/
	void OnMessageFormatError(openpal::MonotonicTimestamp now);

	/**
	* Called when the task first starts, before the first request is formatted
	*/
	void OnStart();

	/**
	* Set the expiration time to minimum. The scheduler must also be informed
	*/
	void SetMinExpiration();

	/**
	* Check if the task is blocked from executing by another task
	*/
	bool IsBlocked() const
	{
		return this->context->IsBlocked(*this);
	}

protected:

	// called during OnStart() to initialize any state for a new run
	virtual void Initialize() {}

	virtual ResponseResult ProcessResponse(const APDUResponseHeader& response, const openpal::RSlice& objects) = 0;

	void CompleteTask(TaskCompletion completion, openpal::MonotonicTimestamp now);

	virtual void OnTaskComplete(TaskCompletion result, openpal::MonotonicTimestamp now) {}

	virtual bool IsEnabled() const
	{
		return true;
	}

	virtual MasterTaskType GetTaskType() const = 0;

	const std::shared_ptr<TaskContext> context;
	IMasterApplication* const application;
	openpal::Logger logger;

	// Validation helpers for various behaviors to avoid deep inheritance
	bool ValidateSingleResponse(const APDUResponseHeader& header);
	bool ValidateNullResponse(const APDUResponseHeader& header, const openpal::RSlice& objects);
	bool ValidateNoObjects(const openpal::RSlice& objects);
	bool ValidateInternalIndications(const APDUResponseHeader& header);


private:

	/**
	* Allows tasks to enter a blocking mode where lower priority
	* tasks cannot run until this task completes
	*/
	virtual bool BlocksLowerPriority() const = 0;

	IMasterTask();

	TaskConfig config;
	TaskBehavior behavior;

};

}

#endif
