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
#include "AsyncTaskNonPeriodic.h"

#include "AsyncTaskGroup.h"

#include <chrono>

using namespace std::chrono;

namespace apl
{


AsyncTaskNonPeriodic::AsyncTaskNonPeriodic(millis_t aRetryDelay, int aPriority, const TaskHandler& arCallback, AsyncTaskGroup* apGroup, const std::string& arName) :
	AsyncTaskBase(aPriority, arCallback, apGroup, high_resolution_clock::time_point::min(), arName),
	mRetryDelay(aRetryDelay)
{

}

void AsyncTaskNonPeriodic::_OnComplete(bool aSuccess)
{
	high_resolution_clock::time_point now = mpGroup->GetUTC();
	if(aSuccess) {
		mIsComplete = true;
		mNextRunTime = high_resolution_clock::time_point::max();
	}
	else {
		mNextRunTime = now + milliseconds(mRetryDelay);
	}
}

} //end ns

