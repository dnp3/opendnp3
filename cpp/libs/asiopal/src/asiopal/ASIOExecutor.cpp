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

#include "SteadyClock.h"

using namespace std;

namespace asiopal
{

ASIOExecutor::ASIOExecutor(asio::io_service& service) : 
	strand(service),
	pShutdownSignal(nullptr)
{

}

ASIOExecutor::~ASIOExecutor()
{
	for (auto pTimer : allTimers)
	{
		delete pTimer;
	}
}

void ASIOExecutor::WaitForShutdown()
{
	Synchronized<bool> sync;
	auto initiate = [this, &sync]() { this->InitiateShutdown(sync); };
	strand.post(initiate);
	sync.WaitForValue();
}

void ASIOExecutor::InitiateShutdown(Synchronized<bool>& handler)
{		
	pShutdownSignal = &handler;
	this->CheckForShutdown();
}

void ASIOExecutor::CheckForShutdown()
{
	if (pShutdownSignal)
	{
		if (activeTimers.empty())
		{
			// send the final shutdown signal via the strand to ensure all post events are flushed
			auto finalpost = [this]() 
			{ 
				this->pShutdownSignal->SetValue(true); 
			};

			strand.post(finalpost);
		}
	}
}

openpal::MonotonicTimestamp ASIOExecutor::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(asiopal_steady_clock::now().time_since_epoch()).count();
}

openpal::ITimer* ASIOExecutor::Start(const openpal::TimeDuration& delay, const openpal::Action0& runnable)
{	
	auto expiration = asiopal_steady_clock::now() + std::chrono::milliseconds(delay.GetMilliseconds());
	return Start(expiration, runnable);
}

openpal::ITimer* ASIOExecutor::Start(const openpal::MonotonicTimestamp& time, const openpal::Action0& runnable)
{	
	asiopal_steady_clock::time_point expiration(std::chrono::milliseconds(time.milliseconds));
	return Start(expiration, runnable);
}

openpal::ITimer* ASIOExecutor::Start(const asiopal_steady_clock::time_point& tp, const openpal::Action0& runnable)
{
	TimerASIO* pTimer = GetTimer();	
	pTimer->timer.expires_at(tp);
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

void ASIOExecutor::Post(const openpal::Action0& runnable)
{
	auto captured = [runnable]() { runnable.Apply(); };
	strand.post(captured);
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
	
	activeTimers.insert(pTimer);
	pTimer->canceled = false;
	return pTimer;
}

void ASIOExecutor::StartTimer(TimerASIO* pTimer, const openpal::Action0& runnable)
{	
	auto callback = [runnable, this, pTimer](const std::error_code& ec){ this->OnTimerCallback(ec, pTimer, runnable); };
	pTimer->timer.async_wait(strand.wrap(callback));
}

void ASIOExecutor::OnTimerCallback(const std::error_code& ec, TimerASIO* pTimer, const openpal::Action0& runnable)
{	
	activeTimers.erase(pTimer);
	idleTimers.push_back(pTimer);
	if (!(ec || pTimer->canceled))
	{
		runnable.Apply();
	}
	this->CheckForShutdown();
}

} //end namespace

