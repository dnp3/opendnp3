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

#ifndef OPENDNP3_INTEGRATIONTESTS_PERFORMANCESTACKPAIR_H
#define OPENDNP3_INTEGRATIONTESTS_PERFORMANCESTACKPAIR_H

#include "mocks/CountingSOEHandler.h"
#include "mocks/QueuedChannelListener.h"

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/logging/LogLevels.h>
#include <opendnp3/outstation/UpdateBuilder.h>

#include <deque>
#include <memory>
#include <random>

class PerformanceStackPair final : opendnp3::Uncopyable
{
    const uint16_t NUM_POINTS_PER_TYPE;
    const uint32_t EVENTS_PER_ITERATION;

    const std::shared_ptr<CountingSOEHandler> soeHandler;

    std::shared_ptr<QueuedChannelListener> clientListener;
    std::shared_ptr<QueuedChannelListener> serverListener;

    const std::shared_ptr<opendnp3::IMaster> master;
    const std::shared_ptr<opendnp3::IOutstation> outstation;

    static opendnp3::OutstationStackConfig GetOutstationStackConfig(uint16_t numPointsPerType,
                                                                    uint16_t eventBufferSize,
                                                                    opendnp3::TimeDuration timeout);
    static opendnp3::MasterStackConfig GetMasterStackConfig(opendnp3::TimeDuration timeout);

    static std::shared_ptr<opendnp3::IMaster> CreateMaster(opendnp3::LogLevels levels,
                                                           opendnp3::TimeDuration timeout,
                                                           opendnp3::DNP3Manager& manager,
                                                           uint16_t port,
                                                           std::shared_ptr<opendnp3::ISOEHandler>,
                                                           std::shared_ptr<opendnp3::IChannelListener> listener);
    static std::shared_ptr<opendnp3::IOutstation> CreateOutstation(
        opendnp3::LogLevels levels,
        opendnp3::TimeDuration timeout,
        opendnp3::DNP3Manager& manager,
        uint16_t port,
        uint16_t numPointsPerType,
        uint16_t eventBufferSize,
        std::shared_ptr<opendnp3::IChannelListener> listener);

    static std::string GetId(const char* name, uint16_t port);
    void AddValue(uint32_t i, opendnp3::UpdateBuilder& builder);

public:
    PerformanceStackPair(opendnp3::LogLevels levels,
                         opendnp3::TimeDuration timeout,
                         opendnp3::DNP3Manager& manager,
                         uint16_t port,
                         uint16_t numPointsPerType,
                         uint32_t eventsPerIteration);

    void WaitForChannelsOnline(std::chrono::steady_clock::duration timeout);

    void SendValues();

    void WaitForValues(std::chrono::steady_clock::duration timeout);
};

#endif
