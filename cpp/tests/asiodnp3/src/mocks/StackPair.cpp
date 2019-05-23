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

#include "StackPair.h"

#include "opendnp3/outstation/SimpleCommandHandler.h"

#include "asiodnp3/DefaultMasterApplication.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <utility>

using namespace opendnp3;

namespace asiodnp3
{

StackPair::StackPair(uint32_t levels,
                     openpal::TimeDuration timeout,
                     DNP3Manager& manager,
                     uint16_t port,
                     uint16_t numPointsPerType,
                     uint32_t eventsPerIteration)
    : PORT(port),
      EVENTS_PER_ITERATION(eventsPerIteration),
      soeHandler(std::make_shared<opendnp3::QueuingSOEHandler>()),
      clientListener(std::make_shared<QueuedChannelListener>()),
      serverListener(std::make_shared<QueuedChannelListener>()),
      master(CreateMaster(levels, timeout, manager, port, this->soeHandler, this->clientListener)),
      outstation(CreateOutstation(
          levels, timeout, manager, port, numPointsPerType, 3 * eventsPerIteration, this->serverListener)),
      index_distribution(0, numPointsPerType - 1),
      type_distribution(0, 6),
      bool_distribution(0, 1),
      int_distribution(0, 32767)
{
    this->outstation->Enable();
    this->master->Enable();
}

void StackPair::WaitForChannelsOnline(std::chrono::steady_clock::duration timeout)
{
    if (!this->clientListener->WaitForState(opendnp3::ChannelState::OPEN, timeout))
    {
        throw std::runtime_error("client timed out before opening");
    }

    if (!this->serverListener->WaitForState(opendnp3::ChannelState::OPEN, timeout))
    {
        throw std::runtime_error("server timed out before opening");
    }
}

void StackPair::SendRandomValues()
{
    UpdateBuilder builder;
    for (uint32_t i = 0; i < EVENTS_PER_ITERATION; ++i)
    {
        this->tx_values.push_back(this->AddRandomValue(builder));
    }
    this->outstation->Apply(builder.Build());
}

void StackPair::WaitToRxValues(std::chrono::steady_clock::duration timeout)
{
    while (!this->tx_values.empty())
    {
        std::deque<ExpectedValue> rx_values;
        this->soeHandler->values.DrainTo(rx_values, timeout);

        if (rx_values.empty())
        {
            std::ostringstream oss;
            oss << "No values received within timeout: " << PORT;
            throw std::runtime_error(oss.str());
        }

        // compare values

        while (!rx_values.empty())
        {
            if (tx_values.empty())
            {
                std::ostringstream oss;
                oss << "more values received than transmited: " << PORT;
                throw std::runtime_error(oss.str());
            }

            const auto& rx = rx_values.front();
            const auto& tx = tx_values.front();

            if (!rx.Equals(tx))
            {
                std::ostringstream oss;
                oss << rx << " != " << tx;
                throw std::runtime_error(oss.str());
            }

            rx_values.pop_front();
            tx_values.pop_front();
        }
    }
}

ExpectedValue StackPair::AddRandomValue(UpdateBuilder& builder)
{
    const auto index = index_distribution(generator);

    switch (type_distribution(generator))
    {
    case (0):
    {
        opendnp3::Binary value(bool_distribution(generator) == 0);
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    case (1):
    {
        opendnp3::DoubleBitBinary value(static_cast<DoubleBit>(bool_distribution(generator)));
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    case (2):
    {
        opendnp3::Analog value(static_cast<double>(int_distribution(generator)));
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    case (3):
    {
        opendnp3::Counter value(int_distribution(generator));
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    case (4):
    {
        opendnp3::FrozenCounter value(int_distribution(generator));
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    case (5):
    {
        opendnp3::BinaryOutputStatus value(bool_distribution(generator) == 0);
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    default:
    {
        opendnp3::AnalogOutputStatus value(static_cast<double>(int_distribution(generator)));
        builder.Update(value, index, EventMode::Force);
        return ExpectedValue(value, index);
    }
    }
}

OutstationStackConfig StackPair::GetOutstationStackConfig(uint16_t numPointsPerType,
                                                          uint16_t eventBufferSize,
                                                          openpal::TimeDuration timeout)
{
    OutstationStackConfig config(opendnp3::DatabaseSizes::AllTypes(numPointsPerType));

    config.outstation.params.unsolConfirmTimeout = timeout;
    config.outstation.eventBufferConfig = opendnp3::EventBufferConfig::AllTypes(eventBufferSize);
    config.outstation.params.allowUnsolicited = true;

    return config;
}

MasterStackConfig StackPair::GetMasterStackConfig(openpal::TimeDuration timeout)
{
    MasterStackConfig config;

    config.master.responseTimeout = timeout;
    config.master.taskRetryPeriod = timeout;
    config.master.disableUnsolOnStartup = false;
    config.master.startupIntegrityClassMask = opendnp3::ClassField::None();
    config.master.unsolClassMask = opendnp3::ClassField::AllEventClasses();

    return config;
}

std::shared_ptr<IMaster> StackPair::CreateMaster(uint32_t levels,
                                                 openpal::TimeDuration timeout,
                                                 DNP3Manager& manager,
                                                 uint16_t port,
                                                 std::shared_ptr<opendnp3::ISOEHandler> soehandler,
                                                 std::shared_ptr<IChannelListener> listener)
{
    auto channel = manager.AddTCPClient(GetId("client", port), levels, asiopal::ChannelRetry::Default(), "127.0.0.1",
                                        "127.0.0.1", port, std::move(listener));

    return channel->AddMaster(GetId("master", port), std::move(soehandler), DefaultMasterApplication::Create(),
                              GetMasterStackConfig(timeout));
}

std::shared_ptr<IOutstation> StackPair::CreateOutstation(uint32_t levels,
                                                         openpal::TimeDuration timeout,
                                                         DNP3Manager& manager,
                                                         uint16_t port,
                                                         uint16_t numPointsPerType,
                                                         uint16_t eventBufferSize,
                                                         std::shared_ptr<IChannelListener> listener)
{
    auto channel = manager.AddTCPServer(GetId("server", port), levels, ServerAcceptMode::CloseExisting, "127.0.0.1",
                                        port, std::move(listener));

    return channel->AddOutstation(GetId("outstation", port), opendnp3::SuccessCommandHandler::Create(),
                                  opendnp3::DefaultOutstationApplication::Create(),
                                  GetOutstationStackConfig(numPointsPerType, eventBufferSize, timeout));
}

std::string StackPair::GetId(const char* name, uint16_t port)
{
    std::ostringstream oss;
    oss << name << ":" << port;
    return oss.str();
}
} // namespace asiodnp3
