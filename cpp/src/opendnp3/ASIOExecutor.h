
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
#ifndef __ASIO_EXECUTOR_H_
#define __ASIO_EXECUTOR_H_

#include "IExecutor.h"

#include <opendnp3/Exception.h>
#include <opendnp3/Visibility.h>

#include <queue>

#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>


namespace opendnp3
{

class TimerASIO;

class DLL_LOCAL ASIOExecutor : public IExecutor
{

public:
	ASIOExecutor(boost::asio::strand*);
	~ASIOExecutor();

	ITimer* Start(timer_clock::duration, const std::function<void ()>&);
	ITimer* Start(const timer_clock::time_point&, const std::function<void ()>&);
	void Post(const std::function<void ()>&);

private:

	void Shutdown();

	TimerASIO* GetTimer();
	void StartTimer(TimerASIO*, const std::function<void ()>&);

	boost::asio::strand* mpStrand;

	typedef std::deque<TimerASIO*> TimerQueue;

	TimerQueue mAllTimers;
	TimerQueue mIdleTimers;

	size_t mNumActiveTimers;
	std::mutex mMutex;
	std::condition_variable mCondition;
	bool mIsShuttingDown;

	void OnTimerCallback(const boost::system::error_code&, TimerASIO*, std::function<void ()>);
};
}

#endif

