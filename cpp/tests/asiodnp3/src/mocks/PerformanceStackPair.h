/*
 * Copyright 2013-2019 Automatak, LLC
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

#ifndef OPENDNP3_PERFORMANCESTACKPAIR_H
#define OPENDNP3_PERFORMANCESTACKPAIR_H

#include "CountingSOEHandler.h"
#include "QueuedChannelListener.h"

#include "opendnp3/LogLevels.h"

#include "asiodnp3/DNP3Manager.h"
#include "asiodnp3/UpdateBuilder.h"

#include <deque>
#include <memory>
#include <random>

namespace asiodnp3
{

class PerformanceStackPair final : openpal::Uncopyable
{
    const uint16_t NUM_POINTS_PER_TYPE;
    const uint32_t EVENTS_PER_ITERATION;

    const std::shared_ptr<opendnp3::CountingSOEHandler> soeHandler;

    std::shared_ptr<QueuedChannelListener> clientListener;
    std::shared_ptr<QueuedChannelListener> serverListener;

    const std::shared_ptr<IMaster> master;
    const std::shared_ptr<IOutstation> outstation;

    static OutstationStackConfig GetOutstationStackConfig(uint16_t numPointsPerType,
                                                          uint16_t eventBufferSize,
                                                          openpal::TimeDuration timeout);
    static MasterStackConfig GetMasterStackConfig(openpal::TimeDuration timeout);

    static std::shared_ptr<IMaster> CreateMaster(uint32_t levels,
                                                 openpal::TimeDuration timeout,
                                                 DNP3Manager&,
                                                 uint16_t port,
                                                 std::shared_ptr<opendnp3::ISOEHandler>,
                                                 std::shared_ptr<IChannelListener> listener);
    static std::shared_ptr<IOutstation> CreateOutstation(uint32_t levels,
                                                         openpal::TimeDuration timeout,
                                                         DNP3Manager&,
                                                         uint16_t port,
                                                         uint16_t numPointsPerType,
                                                         uint16_t eventBufferSize,
                                                         std::shared_ptr<IChannelListener> listener);

    static std::string GetId(const char* name, uint16_t port);
    void AddValue(uint32_t i, UpdateBuilder& builder);

public:
    PerformanceStackPair(uint32_t levels,
                         openpal::TimeDuration timeout,
                         DNP3Manager&,
                         uint16_t port,
                         uint16_t numPointsPerType,
                         uint32_t eventsPerIteration);

    void WaitForChannelsOnline(std::chrono::steady_clock::duration timeout);

    void SendValues();

    void WaitForValues(std::chrono::steady_clock::duration timeout);
};

} // namespace asiodnp3

#endif
