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
#ifndef OPENDNP3_TASKCONFIG_H
#define OPENDNP3_TASKCONFIG_H

#include "TaskId.h"
#include "ITaskCallback.h"
#include "opendnp3/app/User.h"

namespace opendnp3
{

/**
*	Object containing multiple fields for configuring tasks
*/
class TaskConfig
{
public:

	TaskConfig(TaskId taskId_, ITaskCallback* pCallback_, User user_) :
		taskId(taskId_),
		pCallback(pCallback_),
		user(user_)		
	{}	

	static TaskConfig Default()
	{
		return TaskConfig(TaskId::Undefined(), nullptr, User::Default());
	}

	///  --- syntax sugar for building configs -----

	static TaskConfig With(ITaskCallback& callback)
	{
		return TaskConfig(TaskId::Undefined(), &callback, User::Default());
	}

private:

	TaskConfig() = delete;

public:

	TaskId taskId;		
	ITaskCallback* pCallback;
	User user;
};

}

#endif

