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

#include "mocks/PerformanceStackPair.h"

#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/DNP3Manager.h>

#include <catch.hpp>

#include <iostream>
#include <memory>
#include <thread>

using namespace opendnp3;

#define SUITE(name) "PerformanceTestSuite - " name

TEST_CASE(SUITE("PointsPerSecond"))
{
    const uint16_t START_PORT = 20000;
    const uint16_t NUM_STACK_PAIRS = 10;

    const uint16_t NUM_POINTS_PER_TYPE = 50;
    const uint16_t EVENTS_PER_ITERATION = 50;
    const int NUM_ITERATIONS = 100;

    const auto LEVELS = levels::NOTHING | flags::ERR | flags::WARN;

    const auto TEST_TIMEOUT = std::chrono::seconds(5);
    const auto STACK_TIMEOUT = TimeDuration::Seconds(1);

    // run with at least a concurrency of 2, but more if there are more cores
    const auto concurrency = std::max<unsigned int>(std::thread::hardware_concurrency(), 2);

    INFO("Concurrency: " << concurrency);

    DNP3Manager manager(concurrency);

    std::vector<std::unique_ptr<PerformanceStackPair>> pairs;

    for (uint16_t i = 0; i < NUM_STACK_PAIRS; ++i)
    {
        auto pair = std::make_unique<PerformanceStackPair>(LEVELS, STACK_TIMEOUT, manager, START_PORT + i,
                                                           NUM_POINTS_PER_TYPE, EVENTS_PER_ITERATION);
        pairs.push_back(std::move(pair));
    }

    for (auto& pair : pairs)
    {
        pair->WaitForChannelsOnline(TEST_TIMEOUT);
    }

    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < NUM_ITERATIONS; ++i)
    {

        for (auto& pair : pairs)
        {
            pair->SendValues();
        }

        for (auto& pair : pairs)
        {
            pair->WaitForValues(TEST_TIMEOUT);
        }

        // std::cout << "iteration: " << i << std::endl;
    }

    const auto milliseconds
        = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

    const auto total_events_transferred = static_cast<uint64_t>(NUM_STACK_PAIRS)
        * static_cast<uint64_t>(EVENTS_PER_ITERATION) * static_cast<uint64_t>(NUM_ITERATIONS);

    const auto rate = (total_events_transferred * 1000) / milliseconds.count();

    std::cout << total_events_transferred << " in " << milliseconds.count() << " ms == " << rate << " events per/sec"
              << std::endl;
}
