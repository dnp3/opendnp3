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

#include <asiopal/ThreadPool.h>
#include <asiopal/StrandExecutor.h>

#include <opendnp3/LogLevels.h>

#include <testlib/MockLogHandler.h>

using namespace std;
using namespace std::chrono;
using namespace openpal;
using namespace opendnp3;
using namespace asiopal;

#define SUITE(name) "StrandExecutorTestSuite - " name

TEST_CASE(SUITE("Test automatic resource reclaimation"))
{
	const int NUM_THREAD = 10;
	const int NUM_STRAND = 100;
	const int NUM_OPS = 1000;

	uint32_t counter[NUM_STRAND] = { 0 };
	testlib::MockLogHandler log;


	auto pool = ThreadPool::Create(&log, levels::NORMAL, NUM_THREAD);

	auto setup = [&](uint32_t & counter)
	{
		auto exe = StrandExecutor::Create(pool);
		auto increment = [&]()
		{
			++counter;
		};
		for (int i = 0; i < NUM_OPS; ++i)
		{
			exe->PostLambda(increment);
			exe->StartLambda(TimeDuration::Milliseconds(0), increment);
		}
	};

	for (int i = 0; i < NUM_STRAND; ++i)
	{
		setup(counter[i]);
	}

	pool->Shutdown();

	for (int i = 0; i < NUM_STRAND; ++i)
	{
		REQUIRE(counter[i] == 2 * NUM_OPS);
	}
}

TEST_CASE(SUITE("Test ReturnFrom<T>()"))
{
	const int NUM_THREAD = 10;
	testlib::MockLogHandler log;
	int counter = 0;

	auto pool = ThreadPool::Create(&log, levels::NORMAL, NUM_THREAD);
	auto exe = StrandExecutor::Create(pool);


	for (int i = 0; i < 100; ++i)
	{
		auto getvalue = []() -> int { return 1; };
		counter += exe->ReturnFrom<int>(getvalue);
	}

	pool->Shutdown();


	REQUIRE(counter == 100);

}




