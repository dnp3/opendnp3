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
#ifndef __ASYNC_TASK_NON_PERIODIC_H_
#define __ASYNC_TASK_NON_PERIODIC_H_

#include "AsyncTaskInterfaces.h"
#include "AsyncTaskBase.h"

#include <openpal/Uncopyable.h>

namespace opendnp3
{

class AsyncTaskGroup;

/**
	@section desc Asynchronous task. Task execution order is controlled by the period, retry, priority (for resolving ties)
	and task dependencies.
*/

class AsyncTaskNonPeriodic : public AsyncTaskBase
{
	friend class AsyncTaskGroup;

public:

	virtual ~AsyncTaskNonPeriodic() {}

private:

	// Implements ITaskCompletion
	void _OnComplete(bool aSuccess);


	/**
		@param aRetryDelay Retry period of the task in milliseconds. < 0 => no retry.
		@param aPriority Tie break between non-dependent tasks. Lower is higher priority.
		@param arCallback Bound function used for starting the asynchronous task.
		@param apGroup Associated task group
		@param arName Name associated with the task
	*/
	AsyncTaskNonPeriodic(openpal::TimeDuration aRetryDelay, int aPriority, const openpal::Function1<AsyncTaskBase*>& callback, AsyncTaskGroup* apGroup);

	openpal::TimeDuration mRetryDelay;
};

}

#endif
