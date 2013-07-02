
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __ASYNC_TASK_INTERFACES_H_
#define __ASYNC_TASK_INTERFACES_H_

#include <functional>

#include <opendnp3/Visibility.h>

namespace opendnp3
{

class DLL_LOCAL ITask
{
public:

	virtual void OnComplete(bool aSuccess) = 0;

	// Enable the task and notify the task group which might execute another task
	virtual void Enable() = 0;

	// Disable ''
	virtual void Disable() = 0;

	// Enable without notifying the task group
	virtual void SilentEnable() = 0;

	// Diable ''
	virtual void SilentDisable() = 0;
};

typedef std::function<void (ITask*)> TaskHandler;

}

#endif
