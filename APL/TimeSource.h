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
#ifndef __TIME_SOURCE_H_
#define __TIME_SOURCE_H_

#include "ITimeSource.h"
#include "Singleton.h"
#include "Types.h"

namespace apl
{
class TimeSource : public ITimeSource
{
	MACRO_SINGLETON_INSTANCE(TimeSource)

	// Implement ITimeSource
	std::chrono::high_resolution_clock::time_point GetUTC();	
};

class MockTimeSource : public ITimeSource
{
public:

	MockTimeSource();

	// Implement ITimeSource
	std::chrono::high_resolution_clock::time_point GetUTC() {
		return mTime;
	}
	std::chrono::high_resolution_clock::time_point GetTimeStampUTC();

	void SetTime(const std::chrono::high_resolution_clock::time_point& arTime) {
		mTime = arTime;
	}
	
	void Advance(const std::chrono::high_resolution_clock::duration& arDuration);
	void SetToNow();

private:

	std::chrono::high_resolution_clock::time_point mTime;
};

/** Maintains an external time by keeping an offset from system time.
*/
class TimeSourceSystemOffset : public  ITimeManager
{
	public:
		TimeSourceSystemOffset();

		std::chrono::high_resolution_clock::time_point GetUTC();
		void SetTime(const std::chrono::high_resolution_clock::time_point& arTime);

	private:
		std::chrono::high_resolution_clock::duration mOffset;
};

class MockTimeManager : public ITimeManager
{
public:
	MockTimeManager() : mTime() {}

	std::chrono::high_resolution_clock::time_point GetUTC() {
		return mTime;
	}

	void SetTime(const std::chrono::high_resolution_clock::time_point& arTime) {
		mTime = arTime;
	}

private:
	std::chrono::high_resolution_clock::time_point mTime;
};

}

#endif

