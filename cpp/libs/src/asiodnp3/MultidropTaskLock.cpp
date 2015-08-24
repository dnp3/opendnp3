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

#include "MultidropTaskLock.h"

using namespace opendnp3;

namespace asiodnp3
{

MultidropTaskLock::MultidropTaskLock() : isOnline(false), pActive(nullptr)
{

}

bool MultidropTaskLock::Acquire(IScheduleCallback& callback)
{
	if (isOnline)
	{
		if (pActive)
		{
			if (&callback == pActive)
			{
				return true;
			}
			else
			{
				this->AddIfNotContained(callback);
				return false;
			}			
		}
		else
		{
			pActive = &callback;
			return true;
		}
	}
	else
	{
		return false;
	}
}
	
void MultidropTaskLock::Release(IScheduleCallback& callback)
{
	if (isOnline && pActive == &callback)
	{
		pActive = nullptr;
		
		if (!callbackQueue.empty())
		{
			pActive = callbackQueue.front();
			callbackQueue.pop_front();
			callbackSet.erase(pActive);			
			pActive->OnPendingTask();
		}
	}
}
	
void MultidropTaskLock::OnLayerUp()
{
	if (!isOnline)
	{
		isOnline = true;
	}
}
	
void MultidropTaskLock::OnLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
		pActive = nullptr;
		callbackSet.clear();
		callbackQueue.clear();
	}
}

bool MultidropTaskLock::AddIfNotContained(IScheduleCallback& callback)
{
	auto result = callbackSet.find(&callback);
	if (result == callbackSet.end())
	{
		callbackSet.insert(&callback);
		callbackQueue.push_back(&callback);
		return true;
	}
	else
	{
		return false;
	}
}

}

