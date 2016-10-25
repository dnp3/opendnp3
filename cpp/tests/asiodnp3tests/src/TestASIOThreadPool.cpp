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
#include <asiopal/ASIOExecutor.h>
#include <asiodnp3/ConsoleLogger.h>

#include <openpal/logging/LogRoot.h>

#include <opendnp3/LogLevels.h>

#include <thread>

using namespace std;
using namespace boost;
using namespace asio;

using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;


#define SUITE(name) "ASIOThreadPoolTestSuite - " name

TEST_CASE(SUITE("CleanConstructionDestruction"))
{
	ThreadPool pool(nullptr, levels::NORMAL, 4);
}

TEST_CASE(SUITE("ThreadPoolShutsdownCleanlyEvenIfALotOfWorkIsSubmitted"))
{
	ThreadPool pool(nullptr, levels::NORMAL, 4);
	for(size_t i = 0; i < 100000; ++i) pool.GetIOService().post([]() {});
}


TEST_CASE(SUITE("StrandsSequenceCallbacksViaStrandPost"))
{

	const size_t ITERATIONS = 100000;
	int count1 = 0;

	{
		ThreadPool pool(nullptr, levels::NORMAL, 8);
		strand s1(pool.GetIOService());

		auto increment = [&count1]()
		{
			++count1;
		};

		for (size_t i = 0; i < ITERATIONS; ++i) s1.post(increment);
	}

	REQUIRE(count1 == ITERATIONS);
}

TEST_CASE(SUITE("StrandsSequenceCallbacksViaStrandWrap"))
{

	const size_t ITERATIONS = 100000;

	int count1 = 0;

	{
		ThreadPool pool(nullptr, levels::NORMAL, 8);

		strand s1(pool.GetIOService());

		auto increment = [&count1]()
		{
			++count1;
		};

		for (size_t i = 0; i < ITERATIONS; ++i) pool.GetIOService().post(s1.wrap(increment));
	}

	REQUIRE(count1 == ITERATIONS);
}





