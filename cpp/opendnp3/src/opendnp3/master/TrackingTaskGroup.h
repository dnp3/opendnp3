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
#ifndef __TRACKING_TASK_GROUP_H_
#define __TRACKING_TASK_GROUP_H_

#include <openpal/TimeDuration.h>
#include <openpal/Uncopyable.h>
#include <openpal/StaticList.h>

#include "AsyncTaskInterfaces.h"

#include <string>

namespace opendnp3
{

class AsyncTaskGroup;
class AsyncTaskContinuous;
class AsyncTaskBase;

/**
 Tracks all tasks that are created and releases them on destruction
*/
class TrackingTaskGroup
{

public:

	TrackingTaskGroup(AsyncTaskGroup* apGroup);
	~TrackingTaskGroup();

	void ResetTasks(int aMask);

	AsyncTaskBase* Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName = "");
	AsyncTaskContinuous* AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName = "");


private:

	AsyncTaskGroup* mpGroup;

	// TODO make this size configurable
	openpal::StaticList<AsyncTaskBase*, uint32_t, 100> tasks;
};

}

#endif
