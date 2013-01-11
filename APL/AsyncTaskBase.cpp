//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "AsyncTaskBase.h"


#include "Exception.h"
#include "AsyncTaskGroup.h"
#include "Location.h"

namespace apl
{

AsyncTaskBase::AsyncTaskBase(int aPriority,
                             const TaskHandler& arCallback,
                             AsyncTaskGroup* apGroup,
							 const std::chrono::high_resolution_clock::time_point& arInitialTime,
                             const std::string& arName) :
	mName(arName),
	mIsEnabled(false),
	mIsComplete(false),
	mIsExpired(false),
	mIsRunning(false),
	mPriority(aPriority),
	mHandler(arCallback),
	mpGroup(apGroup),
	mNextRunTime(arInitialTime),
	M_INITIAL_TIME(arInitialTime),
	mFlags(0)
{

}

void AsyncTaskBase::Disable()
{
	this->SilentDisable();
	mpGroup->CheckState();
}

void AsyncTaskBase::Enable()
{
	this->SilentEnable();
	mpGroup->CheckState();
}

void AsyncTaskBase::SilentEnable()
{
	mIsEnabled = true;
}

void AsyncTaskBase::SilentDisable()
{
	this->Reset();
	mIsEnabled = false;
}

void AsyncTaskBase::Dispatch()
{
	if(mIsRunning) throw InvalidStateException(LOCATION, "Running");
	if(!mIsEnabled) throw InvalidStateException(LOCATION, "Disabled");

	mIsRunning = true;
	mIsComplete = false;
	mIsExpired = false;
	mHandler(this);
}

void AsyncTaskBase::AddDependency(const AsyncTaskBase* apTask)
{
	if(apTask == this)
		throw ArgumentException(LOCATION, "Self-dependency not allowed");

	if(apTask->IsDependency(this))
		throw ArgumentException(LOCATION, "Circular dependencies not allowed");

	mDependencies.push_back(apTask);
}

bool AsyncTaskBase::IsDependency(const AsyncTaskBase* apTask) const
{
	for(const AsyncTaskBase * p: mDependencies) {
		if(p == apTask) return true;
		if(p->IsDependency(apTask)) return true;
	}

	return false;
}

void AsyncTaskBase::OnComplete(bool aSuccess)
{
	if(!mIsRunning) {
		throw InvalidStateException(LOCATION, "Not Running");
	}
	mIsRunning = false;

	this->_OnComplete(aSuccess);

	mpGroup->OnCompletion();
}

void AsyncTaskBase::Reset()
{
	mIsComplete = mIsExpired = mIsRunning = false;
	mNextRunTime = M_INITIAL_TIME;
	this->_Reset();
}

void AsyncTaskBase::UpdateTime(const std::chrono::high_resolution_clock::time_point& arTime)
{
	if(arTime >= mNextRunTime) {
		mIsComplete = false;
		mIsExpired = true;
	}
	else {
		mIsExpired = false;
	}
}

bool AsyncTaskBase::LessThan(const AsyncTaskBase* l, const AsyncTaskBase* r)
{
	// if a dependency is incomplete, it always blocks
	if(l->IsDependency(r) && !r->IsComplete()) return true;
	if(r->IsDependency(l) && !l->IsComplete()) return false;

	if(!l->IsEnabled() && r->IsEnabled()) return true;
	if(!r->IsEnabled() && l->IsEnabled()) return false;

	if(l->IsExpired()) {
		if(r->IsExpired()) { //if they're both expired, resolve using priority
			return l->Priority() < r->Priority();
		}
		else {
			return false; // left expired but right is not
		}
	}
	else if(r->IsExpired()) {
		return true; // right expired but left is not
	}
	else { // if they're both not expired, the one with the lowest run time is higher
		return l->NextRunTime() > r->NextRunTime();
	}
}

bool AsyncTaskBase::LessThanGroupLevelNoString(const AsyncTaskBase* l, const AsyncTaskBase* r)
{
	// Only one task can be running at a time at the group level;
	if(l->IsRunning()) return false;
	if(r->IsRunning()) return true;

	// if dependencies aren't an issue, compare based upon other criteria
	return LessThan(l, r);
}

bool AsyncTaskBase::LessThanGroupLevel(const AsyncTaskBase* l, const AsyncTaskBase* r)
{
	return LessThanGroupLevelNoString(l, r);
}



} //end ns

