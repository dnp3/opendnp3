
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
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "AsyncTaskGroup.h"

#include "AsyncTaskBase.h"
#include "AsyncTaskPeriodic.h"
#include "AsyncTaskNonPeriodic.h"
#include "AsyncTaskContinuous.h"
#include "AsyncTaskScheduler.h"
#include "Exception.h"
#include "Location.h"
#include "IExecutor.h"

#include <algorithm>

using namespace std::chrono;

namespace apl
{

AsyncTaskGroup::AsyncTaskGroup(IExecutor* apExecutor, ITimeSource* apTimeSrc) :
	mIsRunning(false),
	mShutdown(false),
	mpExecutor(apExecutor),
	mpTimeSrc(apTimeSrc),
	mpTimer(NULL)
{

}

AsyncTaskGroup::~AsyncTaskGroup()
{
	this->Shutdown();

	for(AsyncTaskBase * p: mTaskVec) delete p;
}

AsyncTaskBase* AsyncTaskGroup::Add(millis_t aPeriod, millis_t aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskBase* pTask;
	if(aPeriod >= 0)
		pTask = new AsyncTaskPeriodic(aPeriod, aRetryDelay, aPriority, arCallback, this, arName);
	else
		pTask = new AsyncTaskNonPeriodic(aRetryDelay, aPriority, arCallback, this, arName);

	mTaskVec.push_back(pTask);
	return pTask;
}

void AsyncTaskGroup::ResetTasks(int aMask)
{
	for(AsyncTaskBase* p: mTaskVec) {
		if(!p->IsRunning() && (p->GetFlags() & aMask)) p->Reset();
	}
}

AsyncTaskContinuous* AsyncTaskGroup::AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskContinuous* pTask = new AsyncTaskContinuous(aPriority, arCallback, this, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

void AsyncTaskGroup::Remove(AsyncTaskBase* apTask)
{
	for(TaskVec::iterator i = mTaskVec.begin(); i != mTaskVec.end(); ++i) {	
		if(*i == apTask) {
			delete *i;
			mTaskVec.erase(i);
			return;
		}
	}
	throw ArgumentException(LOCATION, "Task not found");
}

void AsyncTaskGroup::Shutdown()
{
	if(mpTimer) {
		mpTimer->Cancel();
		mpTimer = NULL;
	}

	mShutdown = true;
}

void AsyncTaskGroup::Enable()
{
	for(AsyncTaskBase *p: mTaskVec) {
		p->SilentEnable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Disable()
{
	for(AsyncTaskBase * p: mTaskVec) {
		p->SilentDisable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Enable(int aMask)
{
	for(AsyncTaskBase * p: mTaskVec) {
		if((p->GetFlags() & aMask) != 0) p->SilentEnable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Disable(int aMask)
{
	for(AsyncTaskBase * p: mTaskVec) {
		if((p->GetFlags() & aMask) != 0) p->SilentDisable();
	}
	this->CheckState();
}

AsyncTaskBase* AsyncTaskGroup::GetNext(const std::chrono::high_resolution_clock::time_point& arTime)
{
	this->Update(arTime);
	TaskVec::iterator max = max_element(mTaskVec.begin(), mTaskVec.end(), AsyncTaskBase::LessThanGroupLevel);

	AsyncTaskBase* pRet = NULL;
	if(max != mTaskVec.end()) {
		AsyncTaskBase* p = *max;
		if(!p->IsRunning() && p->IsEnabled()) pRet = p;
	}

	return pRet;
}

void AsyncTaskGroup::CheckState()
{
	if(!mShutdown) {
		high_resolution_clock::time_point now = GetUTC();
		AsyncTaskBase* pTask = GetNext(now);

		if(pTask == NULL) return;
		if(pTask->NextRunTime() == high_resolution_clock::time_point::max()) return;

		if(pTask->NextRunTime() <= now) {
			mIsRunning = true;
			pTask->Dispatch();
		}
		else {
			this->RestartTimer(pTask->NextRunTime());
		}
	}
}

void AsyncTaskGroup::OnCompletion()
{
	if(!mIsRunning) throw InvalidStateException(LOCATION, "Not running");
	mIsRunning = false;
	this->CheckState();
}

std::chrono::high_resolution_clock::time_point AsyncTaskGroup::GetUTC() const
{
	return mpTimeSrc->GetUTC();
}

void AsyncTaskGroup::Update(const std::chrono::high_resolution_clock::time_point& arTime)
{
	for(AsyncTaskBase * p: mTaskVec) {
		p->UpdateTime(arTime);
	}
}

void AsyncTaskGroup::RestartTimer(const std::chrono::high_resolution_clock::time_point& arTime)
{
	if(mpTimer != NULL) {
		if(mpTimer->ExpiresAt() != arTime) {
			mpTimer->Cancel();
			mpTimer = NULL;
		}
	}

	if(mpTimer == NULL)
		mpTimer = mpExecutor->Start(arTime, std::bind(&AsyncTaskGroup::OnTimerExpiration, this));
}

void AsyncTaskGroup::OnTimerExpiration()
{
	mpTimer = NULL;
	this->CheckState();
}

} // end ns
