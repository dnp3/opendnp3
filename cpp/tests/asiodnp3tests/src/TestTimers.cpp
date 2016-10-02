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
#include <catch.hpp>

#include <openpal/logging/LogRoot.h>

#include <asiopal/ASIOExecutor.h>
#include <asiopal/ThreadPool.h>
#include <asiopal/TimeConversions.h>

#include <opendnp3/LogLevels.h>

#include <testlib/MockLogHandler.h>

#include <map>
#include <functional>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace openpal;
using namespace opendnp3;
using namespace asiopal;

class MockTimerHandler
{
public:
	MockTimerHandler() : mCount(0)
	{}

	void OnExpiration()
	{
		++mCount;
	}
	size_t GetCount()
	{
		return mCount;
	}

private:
	size_t mCount;
};

#define SUITE(name) "TimersTestSuite - " name

TEST_CASE(SUITE("Time conversions do not overflow"))
{
	const auto tp = TimeConversions::Convert(openpal::MonotonicTimestamp::Max());
	const auto max = asiopal::steady_clock_t::time_point::max();
	REQUIRE(tp == max);
}

TEST_CASE(SUITE("Test ordered dispatch"))
{
	const int NUM = 10000;

	testlib::MockLogHandler log;
	asiopal::ThreadPool pool(&log, levels::NORMAL, 4);
	asiopal::ASIOExecutor executor(pool.GetIOService());

	bool monotonic = true;
	int count = 0;
	int last = 0;

	for(int i = 1; i <= NUM; ++i)
	{
		auto lambda = [i, &count, &last, &monotonic]()
		{
			++count;
			if (i != (last + 1))
			{
				monotonic = false;
			}
			last = i;
		};

		executor.Post(lambda);
	}

	// post a null event to flush all prior posts
	auto nothing = []() { };
	executor.BlockFor(nothing);

	REQUIRE(count ==  NUM);
	REQUIRE(monotonic);
}


TEST_CASE(SUITE("expiration and reuse"))
{
	MockTimerHandler mth;
	auto pTimerHandler = &mth;
	asio::io_service service;
	ASIOExecutor exe(service);

	auto action = [pTimerHandler]()
	{
		pTimerHandler->OnExpiration();
	};

	ITimer* pT1 = exe.Start(TimeDuration::Milliseconds(1), action);
	REQUIRE(1 ==  service.run_one());
	REQUIRE(1 ==  mth.GetCount());
	ITimer* pT2 = exe.Start(TimeDuration::Milliseconds(1), action);
	service.reset();
	REQUIRE(1 ==  service.run_one());
	REQUIRE(pT1 ==  pT2); //The ASIO implementation should reuse timers
}

TEST_CASE(SUITE("cancelation"))
{
	MockTimerHandler mth;
	auto pTimerHandler = &mth;
	asio::io_service service;
	ASIOExecutor exe(service);
	auto action = [pTimerHandler]()
	{
		pTimerHandler->OnExpiration();
	};
	ITimer* pT1 = exe.Start(TimeDuration::Milliseconds(1), action);
	pT1->Cancel();
	REQUIRE(1 ==  service.run_one());
	REQUIRE(0 ==  mth.GetCount());
	ITimer* pT2 = exe.Start(TimeDuration::Milliseconds(1), action);
	service.reset();
	REQUIRE(1 ==  service.run_one());
	REQUIRE(pT1 ==  pT2);
}


TEST_CASE(SUITE("multiple outstanding"))
{
	MockTimerHandler mth1;
	MockTimerHandler mth2;
	auto pTimerHandler1 = &mth1;
	auto pTimerHandler2 = &mth2;

	asio::io_service service;
	ASIOExecutor ts(service);


	auto action1 = [pTimerHandler1]()
	{
		pTimerHandler1->OnExpiration();
	};
	auto action2 = [pTimerHandler2]()
	{
		pTimerHandler2->OnExpiration();
	};


	ITimer* pT1 = ts.Start(TimeDuration::Milliseconds(0), action1);
	ITimer* pT2 = ts.Start(TimeDuration::Milliseconds(100), action2);

	REQUIRE(pT1 != pT2);

	REQUIRE(1 ==  service.run_one());
	REQUIRE(1 ==  mth1.GetCount());
	REQUIRE(0 ==  mth2.GetCount());

	REQUIRE(1 ==  service.run_one());
	REQUIRE(1 ==  mth1.GetCount());
	REQUIRE(1 ==  mth2.GetCount());
}


