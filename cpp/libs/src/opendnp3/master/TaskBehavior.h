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

#ifndef OPENDNP3_TASKBEHAVIOR_H
#define OPENDNP3_TASKBEHAVIOR_H

#include "openpal/executor/MonotonicTimestamp.h"

namespace opendnp3
{

/**
*   All of the configuration parameters that control how the task wil behave
*/
class TaskBehavior
{

public:

	static TaskBehavior SingleExecutionNoRetry();

	static TaskBehavior SingleExecutionNoRetry(const openpal::MonotonicTimestamp startExpiration);


	static TaskBehavior ImmediatePeriodic(
	    const openpal::TimeDuration& period,
	    const openpal::TimeDuration& minRetryDelay,
	    const openpal::TimeDuration& maxRetryDelay
	);

	static TaskBehavior SingleImmediateExecutionWithRetry(
	    const openpal::TimeDuration& minRetryDelay,
	    const openpal::TimeDuration& maxRetryDelay
	);

	static TaskBehavior ReactsToIINOnly();

	/**
	* Called when the task succeeds. Resets the retry timeout to the minimum, and returns the new expiration time
	*/
	void OnSuccess(const openpal::MonotonicTimestamp& now);

	/**
	* Called when the task fails due to a response timeout
	*/
	void OnResponseTimeout(const openpal::MonotonicTimestamp& now);

	/**
	* return the current expiration time
	*/
	openpal::MonotonicTimestamp GetExpiration() const
	{
		return expiration;
	}

	/**
	* return the time after which the task should fail if it hasn't start running
	*/
	openpal::MonotonicTimestamp GetStartExpiration() const
	{
		return startExpiration;
	}

	/**
	* reset to the initial state
	*/
	void Reset();

	/**
	* Disable the task
	*/
	void Disable();

private:

	openpal::TimeDuration CalcNextRetryTimeout() const;

	TaskBehavior() = delete;

	TaskBehavior(
	    const openpal::TimeDuration& period,
	    const openpal::MonotonicTimestamp& expiration,
	    const openpal::TimeDuration& minRetryDelay,
	    const openpal::TimeDuration& maxRetryDelay,
	    const openpal::MonotonicTimestamp& startExpiration
	);

	const openpal::TimeDuration period;
	const openpal::TimeDuration minRetryDelay;
	const openpal::TimeDuration maxRetryDelay;
	const openpal::MonotonicTimestamp startExpiration;

	// permanently disable the task
	bool disabled = false;

	// The tasks current expiration time
	openpal::MonotonicTimestamp expiration;

	// The current retry delay
	openpal::TimeDuration currentRetryDelay;
};

}

#endif

