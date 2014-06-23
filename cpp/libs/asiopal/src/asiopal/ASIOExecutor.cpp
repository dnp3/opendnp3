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
#include "ASIOExecutor.h"

#include "TimerASIO.h"

#include <asio.hpp>
#include <functional>

using namespace std;

namespace asiopal
{

ASIOExecutor::ASIOExecutor(asio::io_service& service) :
	strand(service),
	numActiveTimers(0),	
	paused(false),
	resumed(false)
{

}

ASIOExecutor::~ASIOExecutor()
{
	for(auto pTimer : allTimers) delete pTimer;
}

openpal::MonotonicTimestamp ASIOExecutor::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

openpal::ITimer* ASIOExecutor::Start(const openpal::TimeDuration& arDelay, const openpal::Runnable& runnable)
{	
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_from_now(std::chrono::milliseconds(arDelay.GetMilliseconds()));
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

openpal::ITimer* ASIOExecutor::Start(const openpal::MonotonicTimestamp& arTime, const openpal::Runnable& runnable)
{	
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_at(std::chrono::steady_clock::time_point(std::chrono::milliseconds(arTime.milliseconds)));
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

void ASIOExecutor::Post(const openpal::Runnable& runnable)
{
	strand.post([runnable]() { runnable.Apply(); });
}

std::function<void()> ASIOExecutor::Wrap(const std::function<void()>& handler)
{
	return strand.wrap(handler);
}

void ASIOExecutor::Pause()
{
	if (!strand.running_in_this_thread())
	{
		assert(!strand.running_in_this_thread());
		std::unique_lock<std::mutex> lock(mutex);
		strand.post([this]()
		{
			this->OnPause();
		});
		condition.wait(lock, [this]()
		{
			return this->paused;
		});
	}
}

void ASIOExecutor::Resume()
{
	if (!strand.running_in_this_thread())
	{
		std::unique_lock<std::mutex> lock(mutex);
		this->resumed = true;
		condition.notify_one();
		condition.wait(lock, [this]()
		{
			return !this->paused;
		});
	}
}

void ASIOExecutor::OnPause()
{
	assert(strand.running_in_this_thread());
	std::unique_lock<std::mutex> lock(mutex);
	this->paused = true;
	condition.notify_one();
	condition.wait(lock, [this]()
	{
		return this->resumed;
	});

	// reset the state here, and tell pausing thread we're done
	this->paused = false;
	this->resumed = false;
	condition.notify_one();
}

TimerASIO* ASIOExecutor::GetTimer()
{
	TimerASIO* pTimer;
	if(idleTimers.size() == 0)
	{
		pTimer = new TimerASIO(strand);
		allTimers.push_back(pTimer);
	}
	else
	{
		pTimer = idleTimers.front();
		idleTimers.pop_front();
	}

	pTimer->canceled = false;
	return pTimer;
}

void ASIOExecutor::StartTimer(TimerASIO* pTimer, const openpal::Runnable& runnable)
{
	++numActiveTimers;
	pTimer->timer.async_wait(
	    strand.wrap(
			[runnable, this, pTimer](const std::error_code& ec){ this->OnTimerCallback(ec, pTimer, runnable); }	        
	    )
	);
}

void ASIOExecutor::OnTimerCallback(const std::error_code& ec, TimerASIO* pTimer, const openpal::Runnable& runnable)
{
	--numActiveTimers;
	idleTimers.push_back(pTimer);
	if (!(ec || pTimer->canceled))
	{
		runnable.Apply();
	}
}

} //end namespace

