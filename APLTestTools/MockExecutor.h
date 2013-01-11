
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
#ifndef __MOCK_EXECUTOR_H_
#define __MOCK_EXECUTOR_H_


#include <APL/IExecutor.h>
#include <map>
#include <queue>

namespace apl
{

class MockTimer;

/** @section desc Test class that doles out MockTimer* */
class MockExecutor : public IExecutor
{
	friend class MockTimer;

public:
	MockExecutor();
	~MockExecutor();

	// Implement IExecutor
	ITimer* Start(const std::chrono::high_resolution_clock::time_point&, const std::function<void ()>&);
	ITimer* Start(std::chrono::high_resolution_clock::duration, const std::function<void ()>&);
	void Post(const std::function<void ()>&);
	void PostSync(const std::function<void ()>&);

	/** Turns the auto-post feature on/off. When Auto post is on, Post() is executed synchronously */
	void SetAutoPost(bool aAutoPost) {
		this->mPostIsSynchronous = aAutoPost;
	}

	/**	Call the next (by expiration time) without caring about the time requirement.
		@returns true if a timer was dispatched */
	bool DispatchOne();

	/** Calls DispatchOne() up to some maximum number of times continuing while
		there are still events to dispatch

		@return the number of events dispatched
	*/
	size_t Dispatch(size_t aMaximum = std::numeric_limits<size_t>::max());

	/** @returns The number of active, pending timers and post operations */
	size_t NumActive() {
		return mTimerMap.size() + mPostQueue.size();
	}
	
	std::chrono::high_resolution_clock::duration NextDurationTimer();


private:

	void Cancel(ITimer* apTimer);

	typedef std::deque<std::function<void ()>> PostQueue;
	typedef std::multimap<std::chrono::high_resolution_clock::time_point, MockTimer*> TimerMap;
	typedef std::deque<MockTimer*> TimerQueue;
	typedef std::deque<std::chrono::high_resolution_clock::duration> DurationTimerQueue;

	bool mPostIsSynchronous;
	PostQueue mPostQueue;
	TimerMap mTimerMap;
	TimerQueue mIdle;
	TimerQueue mAllTimers;
	DurationTimerQueue mDurationTimerQueue;

};

/** @section desc Test timer class used in conjunction with MockExecutor */
class MockTimer : public ITimer
{
	friend class MockExecutor;

public:
	MockTimer(MockExecutor*, const std::chrono::high_resolution_clock::time_point&, const std::function<void ()>&);

	//implement ITimer
	void Cancel();
	std::chrono::high_resolution_clock::time_point ExpiresAt();	

private:
	std::chrono::high_resolution_clock::time_point mTime;
	MockExecutor* mpSource;
	std::function<void ()> mCallback;
};

}

#endif

