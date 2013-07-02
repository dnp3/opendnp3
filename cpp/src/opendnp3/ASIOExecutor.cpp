
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

#include "ASIOExecutor.h"

#include "TimerASIO.h"

#include <opendnp3/Exception.h>
#include <opendnp3/Location.h>

#include <boost/asio.hpp>
#include <functional>

using namespace std;

namespace opendnp3
{

ASIOExecutor::ASIOExecutor(boost::asio::strand* apStrand) :
	mpStrand(apStrand),
	mNumActiveTimers(0),
	mIsShuttingDown(false)
{

}

ASIOExecutor::~ASIOExecutor()
{
	this->Shutdown();
for(auto pTimer: mAllTimers) delete pTimer;
}

ITimer* ASIOExecutor::Start(timer_clock::duration aDelay, const function<void ()>& arCallback)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mIsShuttingDown) MACRO_THROW_EXCEPTION(InvalidStateException, "Can't start a timer while executor is shutting down");
	TimerASIO* pTimer = GetTimer();
	pTimer->mTimer.expires_from_now(aDelay);
	this->StartTimer(pTimer, arCallback);
	return pTimer;
}

ITimer* ASIOExecutor::Start(const timer_clock::time_point& arTime, const function<void ()>& arCallback)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mIsShuttingDown) MACRO_THROW_EXCEPTION(InvalidStateException, "Can't start a timer while executor is shutting down");
	TimerASIO* pTimer = GetTimer();
	pTimer->mTimer.expires_at(arTime);
	this->StartTimer(pTimer, arCallback);
	return pTimer;
}

void ASIOExecutor::Post(const std::function<void ()>& arHandler)
{
	mpStrand->post(arHandler);
}

TimerASIO* ASIOExecutor::GetTimer()
{
	TimerASIO* pTimer;
	if(mIdleTimers.size() == 0) {
		pTimer = new TimerASIO(mpStrand);
		mAllTimers.push_back(pTimer);
	}
	else {
		pTimer = mIdleTimers.front();
		mIdleTimers.pop_front();
	}

	pTimer->mCanceled = false;
	return pTimer;
}

void ASIOExecutor::Shutdown()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mIsShuttingDown = true;
	while(mNumActiveTimers) {
		mCondition.wait(lock);
	}
}

void ASIOExecutor::StartTimer(TimerASIO* apTimer, const std::function<void ()>& arCallback)
{
	++mNumActiveTimers;
	apTimer->mTimer.async_wait(
	        mpStrand->wrap(
	                std::bind(&ASIOExecutor::OnTimerCallback, this, std::placeholders::_1, apTimer, arCallback)
	        )
	);
}

void ASIOExecutor::OnTimerCallback(const boost::system::error_code& ec, TimerASIO* apTimer, std::function<void ()> aCallback)
{
	bool callback = false;
	{
		std::lock_guard<std::mutex> lock(mMutex);
		--mNumActiveTimers;
		mIdleTimers.push_back(apTimer);
		if(! (ec || apTimer->mCanceled) ) callback = true;
		if(mNumActiveTimers == 0) mCondition.notify_all();
	}
	if(callback) aCallback();
}

} //end namespace

