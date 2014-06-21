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

ASIOExecutor::ASIOExecutor(asio::strand* apStrand) :
	mpStrand(apStrand),
	mNumActiveTimers(0),
	mIsShuttingDown(false),
	paused(false),
	resumed(false)
{

}

ASIOExecutor::~ASIOExecutor()
{
	//this->Shutdown();
	for(auto pTimer : mAllTimers) delete pTimer;
}

openpal::MonotonicTimestamp ASIOExecutor::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

openpal::ITimer* ASIOExecutor::Start(const openpal::TimeDuration& arDelay, const openpal::Runnable& runnable)
{
	assert(!mIsShuttingDown);
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_from_now(std::chrono::milliseconds(arDelay.GetMilliseconds()));
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

openpal::ITimer* ASIOExecutor::Start(const openpal::MonotonicTimestamp& arTime, const openpal::Runnable& runnable)
{
	assert(!mIsShuttingDown);
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_at(std::chrono::steady_clock::time_point(std::chrono::milliseconds(arTime.milliseconds)));
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

void ASIOExecutor::Post(const openpal::Runnable& runnable)
{
	mpStrand->post([runnable]() { runnable.Apply(); });
}

void ASIOExecutor::Pause()
{
	if (!mpStrand->running_in_this_thread())
	{
		assert(!mpStrand->running_in_this_thread());
		std::unique_lock<std::mutex> lock(mutex);
		mpStrand->post([this]()
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
	if (!mpStrand->running_in_this_thread())
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
	assert(mpStrand->running_in_this_thread());
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
	if(mIdleTimers.size() == 0)
	{
		pTimer = new TimerASIO(mpStrand);
		mAllTimers.push_back(pTimer);
	}
	else
	{
		pTimer = mIdleTimers.front();
		mIdleTimers.pop_front();
	}

	pTimer->canceled = false;
	return pTimer;
}

void ASIOExecutor::StartTimer(TimerASIO* pTimer, const openpal::Runnable& runnable)
{
	++mNumActiveTimers;
	pTimer->timer.async_wait(
	    mpStrand->wrap(
			[runnable, this, pTimer](const std::error_code& ec){ this->OnTimerCallback(ec, pTimer, runnable); }	        
	    )
	);
}

void ASIOExecutor::OnTimerCallback(const std::error_code& ec, TimerASIO* apTimer, const openpal::Runnable& runnable)
{
	--mNumActiveTimers;
	mIdleTimers.push_back(apTimer);
	if (!(ec || apTimer->canceled))
	{
		runnable.Apply();
	}
}

} //end namespace

