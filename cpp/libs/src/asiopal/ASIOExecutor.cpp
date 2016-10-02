/*
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
#include "asiopal/ASIOExecutor.h"

#include "asiopal/TimerASIO.h"
#include "asiopal/TimeConversions.h"


#include <asio.hpp>
#include <functional>
#include <cstdint>

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
	auto initiate = [this, &sync]()
	{
		this->InitiateShutdown(sync);
	};
	strand.post(initiate);
	sync.WaitForValue();
}

void ASIOExecutor::BlockFor(const std::function<void()>& action)
{
	if (strand.running_in_this_thread())
	{
		action();
	}
	else
	{
		Synchronized<bool> sync;
		auto pointer = &sync;
		auto lambda = [action, pointer]()
		{
			action();
			pointer->SetValue(true);
		};
		strand.post(lambda);
		sync.WaitForValue();
	}
}

void ASIOExecutor::InitiateShutdown(Synchronized<bool>& handler)
{
	pShutdownSignal = &handler;
	this->CheckForShutdown();
}

int64_t ASIOExecutor::BoundedMillis(int64_t ms)
{
	const int64_t maximum = INT64_MAX / (1000 * 1000);
	return (ms > maximum) ? maximum : ms;
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
	return TimeConversions::Convert(steady_clock_t::now());
}

openpal::ITimer* ASIOExecutor::Start(const openpal::TimeDuration& delay, const openpal::action_t& runnable)
{
	const auto now = steady_clock_t::now();
	const auto max_ms = std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock_t::time_point::max() - now).count();
	const auto expiration = (delay.milliseconds > max_ms) ? steady_clock_t::time_point::max() : (now + std::chrono::milliseconds(delay.milliseconds));

	return Start(expiration, runnable);
}

openpal::ITimer* ASIOExecutor::Start(const openpal::MonotonicTimestamp& time, const openpal::action_t& runnable)
{
	return Start(TimeConversions::Convert(time), runnable);
}

openpal::ITimer* ASIOExecutor::Start(const steady_clock_t::time_point& tp, const openpal::action_t& runnable)
{
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_at(tp);
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

void ASIOExecutor::Post(const openpal::action_t& runnable)
{
	strand.post(runnable);
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

void ASIOExecutor::StartTimer(TimerASIO* pTimer, const openpal::action_t& runnable)
{
	auto callback = [runnable, this, pTimer](const std::error_code & ec)
	{
		this->OnTimerCallback(ec, pTimer, runnable);
	};
	pTimer->timer.async_wait(strand.wrap(callback));
}

void ASIOExecutor::OnTimerCallback(const std::error_code& ec, TimerASIO* pTimer, const openpal::action_t& runnable)
{
	activeTimers.erase(pTimer);
	idleTimers.push_back(pTimer);
	if (!(ec || pTimer->canceled))
	{
		runnable();
	}
	this->CheckForShutdown();
}

} //end namespace

