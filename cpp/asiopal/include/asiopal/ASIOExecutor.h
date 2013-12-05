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
#ifndef __ASIO_EXECUTOR_H_
#define __ASIO_EXECUTOR_H_

#include <openpal/Exception.h>
#include <openpal/IExecutor.h>
#include <openpal/Visibility.h>

#include <queue>

#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>


namespace asiopal
{

class TimerASIO;

class DLL_LOCAL ASIOExecutor : public openpal::IExecutor
{

public:
	ASIOExecutor(boost::asio::strand*);
	~ASIOExecutor();

	openpal::MonotonicTimestamp GetTime();

	openpal::ITimer* Start(const openpal::TimeDuration&, const std::function<void ()>&);
	openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const std::function<void ()>&);
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

