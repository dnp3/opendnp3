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

#include "opendnp3/master/ITaskCallback.h"
#include "opendnp3/master/IMasterApplication.h"

namespace opendnp3
{

/**
 * A generic interface for defining master request/response style tasks
 */
class IMasterTask
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
	

	IMasterTask(IMasterApplication& app, openpal::MonotonicTimestamp expiration, openpal::Logger logger, ITaskCallback* pCallback, int userId);


	virtual ~IMasterTask();

	/**
	* 
	* Overridable for auth tasks
	*/
	virtual bool IsAuthTask() const { return false; }

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
	* The time when this task can run again.
	*/
	openpal::MonotonicTimestamp ExpirationTime() const;		

	/**
	 * Build a request APDU.	 
	 */
	virtual void BuildRequest(APDURequest& request, uint8_t seq) = 0;

	/**
	 * Handler for responses	 
	 */	
	ResponseResult OnResponse(const APDUResponseHeader& response, const openpal::ReadBufferView& objects, openpal::MonotonicTimestamp now);
	
	/**
	 * Called when a response times out	 
	 */
	void OnResponseTimeout(openpal::MonotonicTimestamp now);

	/**
	* Called when the layer closes while the task is executing.	
	*/
	void OnLowerLayerClose(openpal::MonotonicTimestamp now);

	/**
	* Called when a task is discared before it can run b/c the session went offline
	*/
	virtual void OnTaskDiscarded(openpal::MonotonicTimestamp now) {}

	/**
	* Called when the task first starts, before the first request is formatted
	*/
	void OnStart();			

	/**
	* Demand that the task run immediately by setting the expiration to 0
	*/
	void Demand() { expiration = 0; }
	
	protected:

	// called during OnStart() to initialize any state for a new run
	virtual void Initialize() {}

	// called when _OnResponse() returns ERROR. Use this to reconfigure the state.
	virtual void OnResponseError(openpal::MonotonicTimestamp now) = 0;

	// called when _OnResponse() returns OK_FINAL. Use this to reconfigure the state.
	virtual void OnResponseOK(openpal::MonotonicTimestamp now) = 0;

	virtual ResponseResult _OnResponse(const APDUResponseHeader& response, const openpal::ReadBufferView& objects) = 0;

	virtual void _OnResponseTimeout(openpal::MonotonicTimestamp now) = 0;

	virtual void _OnLowerLayerClose(openpal::MonotonicTimestamp now) = 0;

	virtual bool IsEnabled() const = 0;

	virtual MasterTaskType GetTaskType() const = 0;

	IMasterApplication* pApplication;
	bool disabled;
	openpal::MonotonicTimestamp expiration;
	openpal::Logger logger;

	// Validation helpers for various behaviors to avoid deep inheritance
	bool ValidateSingleResponse(const APDUResponseHeader& header);
	bool ValidateNullResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects);	
	bool ValidateNoObjects(const openpal::ReadBufferView& objects);
	bool ValidateInternalIndications(const APDUResponseHeader& header);

	void NotifyResult(TaskCompletion result);

	private:

	IMasterTask();

	ITaskCallback* pCallback;
	int userId;
};

}

#endif
