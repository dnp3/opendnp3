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
#include <catch.hpp>

#include <openpal/logging/LogRoot.h>

#include <asiopal/LogFanoutHandler.h>
#include <asiopal/ASIOExecutor.h>
#include <asiopal/IOServiceThreadPool.h>

#include <opendnp3/LogLevels.h>

#include <map>
#include <functional>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace openpal;
using namespace opendnp3;
using namespace asiopal;

class TimerTestObject
{
public:
	TimerTestObject() :
		log(),
		pool(&log, levels::NORMAL, 1),
		strand(pool.GetIOService()),
		exe(&strand),
		mLast(-1),
		mNum(0),
		mMonotonic(true)
	{

	}

	void Receive(int aVal)
	{
		if(aVal <= mLast) mMonotonic = false;
		++mNum;
		mLast = aVal;
	}

	bool IsMonotonic()
	{
		return mMonotonic;
	}

	int Num()
	{
		return mNum;
	}

private:
	asiopal::LogFanoutHandler log;
	asiopal::IOServiceThreadPool pool;
	asio::strand strand;

public:
	asiopal::ASIOExecutor exe;

private:

	int mLast;
	int mNum;
	bool mMonotonic;
};

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


TEST_CASE(SUITE("TestOrderedDispatch"))
{
	const int NUM = 10000;

	TimerTestObject test;
	auto pTest = &test;

	for(int i = 0; i < NUM; ++i)
	{		
		auto lambda = [pTest, i]() { pTest->Receive(i); };
		test.exe.PostLambda(lambda);
	}

	{
		ExecutorPause p(&test.exe);
	}

	REQUIRE(NUM ==  test.Num());
	REQUIRE(test.IsMonotonic());
}


TEST_CASE(SUITE("ExpirationAndReuse"))
{
	MockTimerHandler mth;
	auto pTimerHandler = &mth;
	asio::io_service srv;
	asio::strand strand(srv);
	ASIOExecutor exe(&strand);

	auto lambda = [pTimerHandler]() { pTimerHandler->OnExpiration(); };
	auto runnable = Runnable::Bind(lambda);
	ITimer* pT1 = exe.Start(TimeDuration::Milliseconds(1), runnable);
	REQUIRE(1 ==  srv.run_one());
	REQUIRE(1 ==  mth.GetCount());
	ITimer* pT2 = exe.Start(TimeDuration::Milliseconds(1), runnable);
	srv.reset();
	REQUIRE(1 ==  srv.run_one());
	REQUIRE(pT1 ==  pT2); //The ASIO implementation should reuse timers
}

TEST_CASE(SUITE("Cancelation"))
{
	MockTimerHandler mth;
	auto pTimerHandler = &mth;
	asio::io_service srv;
	asio::strand strand(srv);
	ASIOExecutor exe(&strand);
	auto lambda = [pTimerHandler](){ pTimerHandler->OnExpiration(); };
	ITimer* pT1 = exe.Start(TimeDuration::Milliseconds(1), Runnable::Bind(lambda));
	pT1->Cancel();
	REQUIRE(1 ==  srv.run_one());
	REQUIRE(0 ==  mth.GetCount());
	ITimer* pT2 = exe.Start(TimeDuration::Milliseconds(1), Runnable::Bind(lambda));
	srv.reset();
	REQUIRE(1 ==  srv.run_one());
	REQUIRE(pT1 ==  pT2);
}


TEST_CASE(SUITE("MultipleOutstanding"))
{
	MockTimerHandler mth1;
	MockTimerHandler mth2;
	auto pTimerHandler1 = &mth1;
	auto pTimerHandler2 = &mth2;
	
	asio::io_service srv;
	asio::strand strand(srv);
	ASIOExecutor ts(&strand);


	auto lambda1 = [pTimerHandler1](){ pTimerHandler1->OnExpiration(); };
	auto lambda2 = [pTimerHandler2](){ pTimerHandler2->OnExpiration(); };


	ITimer* pT1 = ts.Start(TimeDuration::Milliseconds(0), Runnable::Bind(lambda1));
	ITimer* pT2 = ts.Start(TimeDuration::Milliseconds(100), Runnable::Bind(lambda2));

	REQUIRE(pT1 != pT2);

	REQUIRE(1 ==  srv.run_one());
	REQUIRE(1 ==  mth1.GetCount());
	REQUIRE(0 ==  mth2.GetCount());

	REQUIRE(1 ==  srv.run_one());
	REQUIRE(1 ==  mth1.GetCount());
	REQUIRE(1 ==  mth2.GetCount());
}


