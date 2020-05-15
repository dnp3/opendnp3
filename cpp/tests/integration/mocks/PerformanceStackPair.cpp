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

#include <opendnp3/master/DefaultMasterApplication.h>
#include <opendnp3/outstation/DefaultOutstationApplication.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

#include <dnp3mocks/DatabaseHelpers.h>

#include <exception>
#include <iostream>
#include <sstream>
#include <utility>

using namespace opendnp3;

PerformanceStackPair::PerformanceStackPair(LogLevels levels,
                                           TimeDuration timeout,
                                           DNP3Manager& manager,
                                           uint16_t port,
                                           uint16_t numPointsPerType,
                                           uint32_t eventsPerIteration)
    : NUM_POINTS_PER_TYPE(numPointsPerType),
      EVENTS_PER_ITERATION(eventsPerIteration),
      soeHandler(std::make_shared<CountingSOEHandler>()),
      clientListener(std::make_shared<QueuedChannelListener>()),
      serverListener(std::make_shared<QueuedChannelListener>()),
      master(CreateMaster(levels, timeout, manager, port, this->soeHandler, this->clientListener)),
      outstation(CreateOutstation(
          levels, timeout, manager, port, numPointsPerType, 3 * eventsPerIteration, this->serverListener))
{
    this->outstation->Enable();
    this->master->Enable();
}

void PerformanceStackPair::WaitForChannelsOnline(std::chrono::steady_clock::duration timeout)
{
    if (!this->clientListener->WaitForState(ChannelState::OPEN, timeout))
    {
        throw std::runtime_error("client timed out before opening");
    }

    if (!this->serverListener->WaitForState(ChannelState::OPEN, timeout))
    {
        throw std::runtime_error("server timed out before opening");
    }
}

void PerformanceStackPair::SendValues()
{
    UpdateBuilder builder;
    for (uint32_t i = 0; i < EVENTS_PER_ITERATION; ++i)
    {
        AddValue(i, builder);
    }
    this->outstation->Apply(builder.Build());
}

void PerformanceStackPair::AddValue(uint32_t i, UpdateBuilder& builder)
{
    const uint16_t index = i % NUM_POINTS_PER_TYPE;

    switch (i % 6)
    {
    case (0):
        builder.Update(Binary(i % 2 == 0), index, EventMode::Force);
        break;
    case (1):
        builder.Update(DoubleBitBinary(i % 2 == 0 ? DoubleBit::DETERMINED_ON : DoubleBit::DETERMINED_OFF), index,
                       EventMode::Force);
        break;
    case (2):
        builder.Update(Analog(i), index, EventMode::Force);
        break;
    case (3):
        builder.Update(Counter(i), index, EventMode::Force);
        break;
    case (4):
        builder.Update(BinaryOutputStatus(i % 2 == 0), index, EventMode::Force);
        break;
    default:
        builder.Update(AnalogOutputStatus(i), index, EventMode::Force);
        break;
    }
}

void PerformanceStackPair::WaitForValues(std::chrono::steady_clock::duration timeout)
{
    this->soeHandler->WaitForCount(this->EVENTS_PER_ITERATION, timeout);
}

OutstationStackConfig PerformanceStackPair::GetOutstationStackConfig(uint16_t numPointsPerType,
                                                                     uint16_t eventBufferSize,
                                                                     TimeDuration timeout)
{
    OutstationStackConfig config(configure::by_count_of::all_types(numPointsPerType));

    config.outstation.params.unsolConfirmTimeout = timeout;
    config.outstation.eventBufferConfig = EventBufferConfig::AllTypes(eventBufferSize);
    config.outstation.params.allowUnsolicited = true;

    return config;
}

MasterStackConfig PerformanceStackPair::GetMasterStackConfig(TimeDuration timeout)
{
    MasterStackConfig config;

    config.master.responseTimeout = timeout;
    config.master.taskRetryPeriod = timeout;
    config.master.disableUnsolOnStartup = false;
    config.master.startupIntegrityClassMask = ClassField::None();
    config.master.unsolClassMask = ClassField::AllEventClasses();

    return config;
}

std::shared_ptr<IMaster> PerformanceStackPair::CreateMaster(LogLevels levels,
                                                            TimeDuration timeout,
                                                            DNP3Manager& manager,
                                                            uint16_t port,
                                                            std::shared_ptr<ISOEHandler> soehandler,
                                                            std::shared_ptr<IChannelListener> listener)
{
    auto channel = manager.AddTCPClient(GetId("client", port), levels, ChannelRetry::Default(),
                                        {IPEndpoint("127.0.0.1", port)}, "127.0.0.1", std::move(listener));

    return channel->AddMaster(GetId("master", port), std::move(soehandler), DefaultMasterApplication::Create(),
                              GetMasterStackConfig(timeout));
}

std::shared_ptr<IOutstation> PerformanceStackPair::CreateOutstation(LogLevels levels,
                                                                    TimeDuration timeout,
                                                                    DNP3Manager& manager,
                                                                    uint16_t port,
                                                                    uint16_t numPointsPerType,
                                                                    uint16_t eventBufferSize,
                                                                    std::shared_ptr<IChannelListener> listener)
{
    auto channel = manager.AddTCPServer(GetId("server", port), levels, ServerAcceptMode::CloseExisting,
                                        IPEndpoint("127.0.0.1", port), std::move(listener));

    return channel->AddOutstation(GetId("outstation", port), SuccessCommandHandler::Create(),
                                  DefaultOutstationApplication::Create(),
                                  GetOutstationStackConfig(numPointsPerType, eventBufferSize, timeout));
}

std::string PerformanceStackPair::GetId(const char* name, uint16_t port)
{
    std::ostringstream oss;
    oss << name << ":" << port;
    return oss.str();
}
