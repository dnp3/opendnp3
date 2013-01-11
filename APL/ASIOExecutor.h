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
#ifndef __ASIO_EXECUTOR_H_
#define __ASIO_EXECUTOR_H_

#include "IExecutor.h"
#include "Exception.h"

#include <queue>
#include <iostream>

namespace boost
{

namespace asio
{
class io_service;
}
namespace system
{
class error_code;
}

}

namespace apl
{

class TimerASIO;

class ASIOExecutor : public IExecutor
{

public:
	ASIOExecutor(boost::asio::io_service*);
	~ASIOExecutor();

	ITimer* Start(std::chrono::high_resolution_clock::duration, const std::function<void ()>&);
	ITimer* Start(const std::chrono::high_resolution_clock::time_point&, const std::function<void ()>&);
	void Post(const std::function<void ()>&);

private:

	TimerASIO* GetTimer();
	void StartTimer(TimerASIO*, const std::function<void ()>&);

	boost::asio::io_service* mpService;

	typedef std::deque<TimerASIO*> TimerQueue;

	TimerQueue mAllTimers;
	TimerQueue mIdleTimers;

	void OnTimerCallback(const boost::system::error_code&, TimerASIO*, std::function<void ()>);
};
}

#endif

