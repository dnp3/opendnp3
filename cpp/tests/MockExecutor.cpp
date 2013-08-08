
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

#include "MockExecutor.h"

using namespace openpal;
using namespace std::chrono;

namespace opendnp3
{

MockExecutor::MockExecutor() : mPostIsSynchronous(false)
{

}

MockExecutor::~MockExecutor()
{
for(auto pTimer: mAllTimers) delete pTimer;
}

timer_clock::duration MockExecutor::NextDurationTimer()
{
	if(!mDurationTimerQueue.empty()) {
		auto ret = mDurationTimerQueue.front();
		mDurationTimerQueue.pop_front();
		return ret;
	}
	else return timer_clock::duration::min();
}

bool MockExecutor::DispatchOne()
{
	if(mPostQueue.size() > 0) {
		auto callback = mPostQueue.front();
		mPostQueue.pop_front();
		callback();
		return true;
	}
	else {
		TimerMap::iterator front = mTimerMap.begin();

		if(front != mTimerMap.end()) {

			MockTimer* pTimer = front->second;
			mIdle.push_back(pTimer);
			mTimerMap.erase(front);

			// do the callback last so that if it does something to
			// the timer itself we're safe
			pTimer->mCallback();

			return true;
		}
	}

	return false;
}

size_t MockExecutor::Dispatch(size_t aMaximum)
{
	size_t num = 0;
	while(num < aMaximum && this->DispatchOne()) ++num;
	return num;
}

void MockExecutor::Post(const std::function<void ()>& arHandler)
{
	if(mPostIsSynchronous) arHandler();
	else mPostQueue.push_back(arHandler);
}

void MockExecutor::PostSync(const std::function<void ()>& arHandler)
{
	this->Dispatch();
	arHandler();
}

ITimer* MockExecutor::Start(timer_clock::duration aDelay, const std::function<void ()>& arCallback)
{
	timer_clock::time_point time =  timer_clock::now() + aDelay;
	mDurationTimerQueue.push_back(aDelay);
	return Start(time, arCallback);
}

ITimer* MockExecutor::Start(const timer_clock::time_point& arTime, const std::function<void ()>& arCallback)
{
	MockTimer* pTimer;
	if(mIdle.size() > 0) {
		pTimer = mIdle.front();
		mIdle.pop_front();
		pTimer->mCallback = arCallback;
	}
	else {
		pTimer = new MockTimer(this, arTime, arCallback);
		mAllTimers.push_back(pTimer);
	}

	mTimerMap.insert(std::pair<timer_clock::time_point, MockTimer*>(arTime, pTimer));
	return pTimer;
}

void MockExecutor::Cancel(ITimer* apTimer)
{
	for(TimerMap::iterator i = mTimerMap.begin(); i != mTimerMap.end(); ++i) {
		if(i->second == apTimer) {
			mIdle.push_back(i->second);
			mTimerMap.erase(i);
			return;
		}
	}
}

MockTimer::MockTimer(MockExecutor* apSource, const timer_clock::time_point& arTime, const std::function<void ()>& arCallback) :
	mTime(arTime),
	mpSource(apSource),
	mCallback(arCallback)
{

}

void MockTimer::Cancel()
{
	mpSource->Cancel(this);
}

timer_clock::time_point MockTimer::ExpiresAt()
{
	return mTime;
}

}


