
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

#include "TrackingTaskGroup.h"

#include "AsyncTaskGroup.h"
#include "AsyncTaskContinuous.h"

namespace opendnp3
{

TrackingTaskGroup::TrackingTaskGroup(AsyncTaskGroup* apGroup) : mpGroup(apGroup)
{}

void TrackingTaskGroup::ResetTasks(int aMask)
{
for(AsyncTaskBase * p: mTaskVec) {
		if(!p->IsRunning() && (p->GetFlags() & aMask)) p->Reset();
	}
}

TrackingTaskGroup::~TrackingTaskGroup()
{
	//remove all the tasks that were created
for(auto pTask: mTaskVec) mpGroup->Remove(pTask);
}

AsyncTaskBase* TrackingTaskGroup::Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskBase* pTask = mpGroup->Add(aPeriod, aRetryDelay, aPriority, arCallback, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

AsyncTaskContinuous* TrackingTaskGroup::AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskContinuous* pTask = mpGroup->AddContinuous(aPriority, arCallback, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

}


