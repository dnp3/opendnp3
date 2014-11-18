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

#include "IMasterTask.h"

using namespace openpal;

namespace opendnp3
{

IMasterTask::IMasterTask(bool enabled_) : IMasterTask(enabled_, MonotonicTimestamp::Max())
{}

IMasterTask::IMasterTask() : IMasterTask(false)
{}

IMasterTask::IMasterTask(bool enabled_, openpal::MonotonicTimestamp expiration_) : enabled(enabled_), expiration(expiration_), pCallback(nullptr)
{}

openpal::MonotonicTimestamp IMasterTask::ExpirationTime() const
{
	return this->IsEnabled() ? expiration : MonotonicTimestamp::Max();
}
	
void IMasterTask::OnStart()
{
	if (pCallback)
	{
		pCallback->OnStart();
	}
	
	this->_OnStart();
}

	
void IMasterTask::SetTaskCallback(ITaskCallback* pCallback_)
{
	pCallback = pCallback_;
}

}

