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

#include "MasterTaskList.h"

#include "opendnp3/app/ClassMask.h"
#include "opendnp3/master/ConstantCommandProcessor.h"

namespace opendnp3
{

MasterTaskList::MasterTaskList(ISOEHandler* pSOEHandler_, openpal::Logger* pLogger_, const MasterParams& params) :
	pParams(&params),
	enableUnsol(this, pLogger_),
	disableUnsol(this, pLogger_),
	startupIntegrity(this, pSOEHandler_, pLogger_)	
{
	
}

void MasterTaskList::Initialize()
{
	startupTasks.Clear();

	if (pParams->disableUnsolOnStartup)
	{
		this->startupTasks.Enqueue(&disableUnsol);
	}

	if (pParams->startupIntergrityClassMask != 0)
	{
		this->startupTasks.Enqueue(&startupIntegrity);
	}
	
	if (pParams->unsolClassMask & ALL_EVENT_CLASSES)
	{
		this->startupTasks.Enqueue(&enableUnsol);
	}
}

void MasterTaskList::ScheduleNext(IMasterScheduler& scheduler)
{
	if (startupTasks.IsEmpty())
	{
		// TODO - startup complete...
	}
	else
	{
		scheduler.Schedule(startupTasks.Pop());
	}
}

}

