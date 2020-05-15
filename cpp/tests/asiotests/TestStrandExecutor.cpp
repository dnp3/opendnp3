/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <opendnp3/logging/LogLevels.h>

#include <exe4cpp/asio/StrandExecutor.h>
#include <exe4cpp/asio/ThreadPool.h>

#include <catch.hpp>

using namespace exe4cpp;

#define SUITE(name) "ExecutorTestSuite - " name

TEST_CASE(SUITE("Test automatic resource reclaimation"))
{
    const int NUM_THREAD = 10;
    const int NUM_STRAND = 100;
    const int NUM_OPS = 1000;

    uint32_t counter[NUM_STRAND] = {0};

    auto io = std::make_shared<asio::io_context>();

    ThreadPool pool(io, NUM_THREAD);

    auto setup = [&](uint32_t& counter) {
        auto exe = std::make_shared<exe4cpp::StrandExecutor>(io);
        auto increment = [&]() { ++counter; };
        for (int i = 0; i < NUM_OPS; ++i)
        {
            exe->post(increment);
            exe->start(std::chrono::milliseconds(0), increment);
        }
    };

    for (unsigned int& i : counter)
    {
        setup(i);
    }

    pool.shutdown();

    for (unsigned int i : counter)
    {
        REQUIRE(i == 2 * NUM_OPS);
    }
}

TEST_CASE(SUITE("Executor dispatch is from only one thread at a time"))
{
    const int NUM_THREAD = 10;
    const int NUM_OPS = 1000;

    auto io = std::make_shared<asio::io_context>();

    int sum = 0;

    {
        ThreadPool pool(io, NUM_THREAD);
        auto exe = std::make_shared<exe4cpp::StrandExecutor>(io);

        for (int i = 0; i < NUM_OPS; ++i)
        {
            auto increment = [&sum]() { ++sum; };
            exe->post(increment);
        }
    }

    REQUIRE(sum == NUM_OPS);
}

TEST_CASE(SUITE("Executor dispatch is in same order as post order"))
{
    const int NUM_THREAD = 10;
    const int NUM_OPS = 1000;

    auto io = std::make_shared<asio::io_context>();

    int order = 0;
    bool is_ordered = true;

    {
        ThreadPool pool(io, NUM_THREAD);
        auto exe = std::make_shared<exe4cpp::StrandExecutor>(io);

        for (int i = 0; i < NUM_OPS; ++i)
        {
            auto test_order = [i, &order, &is_ordered]() {
                if (is_ordered)
                {
                    if (i == order)
                    {
                        ++order;
                    }
                    else
                    {
                        is_ordered = false;
                    }
                }
            };
            exe->post(test_order);
        }
    }

    REQUIRE(is_ordered);
}

TEST_CASE(SUITE("Test ReturnFrom<T>()"))
{
    const int NUM_THREAD = 10;
    const int NUM_ACTIONS = 100;
    int counter = 0;

    auto io = std::make_shared<asio::io_context>();

    {
        ThreadPool pool(io, NUM_THREAD);
        auto exe = std::make_shared<exe4cpp::StrandExecutor>(io);

        for (int i = 0; i < NUM_ACTIONS; ++i)
        {
            auto getvalue = []() -> int { return 1; };
            counter += exe->return_from<int>(getvalue);
        }
    }

    REQUIRE(counter == NUM_ACTIONS);
}
