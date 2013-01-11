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

#include "ASIOExecutor.h"

#include "TimerASIO.h"

#include <boost/asio.hpp>

#include <functional>
#include <iostream>

using namespace std;

namespace apl
{

ASIOExecutor::ASIOExecutor(boost::asio::io_service* apService) :
	mpService(apService)
{

}

ASIOExecutor::~ASIOExecutor()
{
	for(auto pTimer: mAllTimers) delete pTimer;
}

ITimer* ASIOExecutor::Start(std::chrono::high_resolution_clock::duration aDelay, const function<void ()>& arCallback)
{
	TimerASIO* pTimer = GetTimer();
	pTimer->mTimer.expires_from_now(aDelay);
	this->StartTimer(pTimer, arCallback);
	return pTimer;
}

ITimer* ASIOExecutor::Start(const std::chrono::high_resolution_clock::time_point& arTime, const function<void ()>& arCallback)
{
	TimerASIO* pTimer = GetTimer();
	pTimer->mTimer.expires_at(arTime);
	this->StartTimer(pTimer, arCallback);
	return pTimer;
}

void ASIOExecutor::Post(const std::function<void ()>& arHandler)
{
	mpService->post(arHandler);
}

TimerASIO* ASIOExecutor::GetTimer()
{
	TimerASIO* pTimer;
	if(mIdleTimers.size() == 0) {
		pTimer = new TimerASIO(*mpService);
		mAllTimers.push_back(pTimer);
	}
	else {
		pTimer = mIdleTimers.front();
		mIdleTimers.pop_front();
	}

	pTimer->mCanceled = false;
	return pTimer;
}

void ASIOExecutor::StartTimer(TimerASIO* apTimer, const std::function<void ()>& arCallback)
{
	apTimer->mTimer.async_wait(std::bind(&ASIOExecutor::OnTimerCallback, this, std::placeholders::_1, apTimer, arCallback));
}

void ASIOExecutor::OnTimerCallback(const boost::system::error_code& ec, TimerASIO* apTimer, std::function<void ()> aCallback)
{
	mIdleTimers.push_back(apTimer);
	if(! (ec || apTimer->mCanceled) ) aCallback();
}

} //end namespace

