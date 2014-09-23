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

#ifndef ASIODNP3_MULTIDROPTASKLOCK_H
#define ASIODNP3_MULTIDROPTASKLOCK_H

#include <opendnp3/master/ITaskLock.h>

#include <set>
#include <deque>

namespace asiodnp3
{

class MultidropTaskLock: public opendnp3::ITaskLock
{
	public:

	MultidropTaskLock();

	
	virtual bool Acquire(opendnp3::IScheduleCallback&) override final;

	
	virtual void Release(opendnp3::IScheduleCallback&) override final;

	
	virtual void OnLayerUp() override final;

	
	virtual void OnLayerDown() override final;


	private:

	bool AddIfNotContained(opendnp3::IScheduleCallback&);

	std::set<opendnp3::IScheduleCallback*> callbackSet;
	std::deque<opendnp3::IScheduleCallback*> callbackQueue;

	bool isOnline;

	opendnp3::IScheduleCallback* pActive;
};

}

#endif
