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
#include <boost/test/unit_test.hpp>

#include <opendnp3/ExecutorPause.h>
#include <asiopal/Log.h>

#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/ASIOExecutor.h>

#include "Exception.h"

#include <thread>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;


BOOST_AUTO_TEST_SUITE(ASIOThreadPoolTestSuite)

BOOST_AUTO_TEST_CASE(CleanConstructionDestruction)
{
	EventLog log;
	IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), 4);
}

BOOST_AUTO_TEST_CASE(ThreadPoolShutsdownCleanlyEvenIfALotOfWorkIsSubmitted)
{
	EventLog log;
	IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), 4);
	for(size_t i = 0; i < 100000; ++i) pool.GetIOService()->post([]() {});
}


BOOST_AUTO_TEST_CASE(StrandsSequenceCallbacksViaStrandPost)
{
	EventLog log;
	IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), 8);

	size_t iterations = 100000;

	strand s1(*pool.GetIOService());

	int count1 = 0;

	for(size_t i = 0; i < iterations; ++i) s1.post([&count1]() {
		++count1;
	});

	pool.Shutdown();
	BOOST_REQUIRE_EQUAL(iterations, count1);
}

BOOST_AUTO_TEST_CASE(StrandsSequenceCallbacksViaStrandWrap)
{
	EventLog log;
	IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), 8);
	size_t iterations = 100000;

	io_service* pService = pool.GetIOService();

	strand s1(*pService);

	int count1 = 0;

	for(size_t i = 0; i < iterations; ++i) pService->post(s1.wrap([&count1]() {
		++count1;
	}));

	pool.Shutdown();
	BOOST_REQUIRE_EQUAL(iterations, count1);
}

BOOST_AUTO_TEST_CASE(ExecutorPauseGuardsRaceConditions)
{
	EventLog log;
	IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), 8);
	size_t iterations = 100000;

	boost::asio::strand strand(*pool.GetIOService());
	ASIOExecutor exe(&strand);

	int count = 0;
	auto increment = [&]() {
		int i = count;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		count = i + 1;
	};

	for(size_t i = 0; i < 100; ++i) { //try to cause a race condition between the Post and the Pause
		exe.Post(increment);
		ExecutorPause p1(&exe);
		increment();
	}

	pool.Shutdown();

	BOOST_REQUIRE_EQUAL(200, count);
}

BOOST_AUTO_TEST_SUITE_END()

