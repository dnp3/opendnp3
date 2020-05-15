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
#include "utils/OutstationTestObject.h"

#include "utils/BufferHelpers.h"

using namespace opendnp3;

OutstationTestObject::OutstationTestObject(const OutstationConfig& config, const opendnp3::DatabaseConfig& db_config)
    : exe(std::make_shared<exe4cpp::MockExecutor>()),
      lower(std::make_shared<MockLowerLayer>()),
      cmdHandler(std::make_shared<MockCommandHandler>(CommandStatus::SUCCESS)),
      application(std::make_shared<MockOutstationApplication>()),
      context(Addresses(), config, db_config, log.logger, exe, lower, cmdHandler, application)
{
    lower->SetUpperLayer(context);
}

size_t OutstationTestObject::LowerLayerUp()
{
    context.OnLowerLayerUp();
    return exe->run_many();
}

size_t OutstationTestObject::LowerLayerDown()
{
    context.OnLowerLayerDown();
    return exe->run_many();
}

size_t OutstationTestObject::OnTxReady()
{
    context.OnTxReady();
    return exe->run_many();
}

size_t OutstationTestObject::SendToOutstation(const std::string& hex)
{
    HexSequence hs(hex);
    context.OnReceive(Message(Addresses(), hs.ToRSeq()));
    return exe->run_many();
}

size_t OutstationTestObject::BroadcastToOutstation(LinkBroadcastAddress broadcast_address, const std::string& hex)
{
    HexSequence hs(hex);
    context.OnReceive(Message(Addresses(0, broadcast_address), hs.ToRSeq()));
    return exe->run_many();
}

size_t OutstationTestObject::NumPendingTimers() const
{
    return exe->num_pending_timers();
}

bool OutstationTestObject::AdvanceToNextTimer()
{
    if (exe->advance_to_next_timer())
    {
        return exe->run_many() > 0;
    }

    return false;
}

size_t OutstationTestObject::AdvanceTime(const TimeDuration& td)
{
    exe->advance_time(td.value);
    return exe->run_many();
}
