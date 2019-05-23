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
#include "OutstationTestObject.h"

#include <testlib/BufferHelpers.h>

using namespace openpal;
using namespace testlib;

namespace opendnp3
{

OutstationTestObject::OutstationTestObject(const OutstationConfig& config, const DatabaseSizes& dbSizes)
    : exe(std::make_shared<MockExecutor>()),
      lower(std::make_shared<MockLowerLayer>()),
      cmdHandler(std::make_shared<MockCommandHandler>(CommandStatus::SUCCESS)),
      application(std::make_shared<MockOutstationApplication>()),
      context(Addresses(), config, dbSizes, log.logger, exe, lower, cmdHandler, application)
{
    lower->SetUpperLayer(context);
}

size_t OutstationTestObject::LowerLayerUp()
{
    context.OnLowerLayerUp();
    return exe->RunMany();
}

size_t OutstationTestObject::LowerLayerDown()
{
    context.OnLowerLayerDown();
    return exe->RunMany();
}

size_t OutstationTestObject::OnTxReady()
{
    context.OnTxReady();
    return exe->RunMany();
}

size_t OutstationTestObject::SendToOutstation(const std::string& hex)
{
    HexSequence hs(hex);
    context.OnReceive(Message(Addresses(), hs.ToRSlice()));
    return exe->RunMany();
}

size_t OutstationTestObject::NumPendingTimers() const
{
    return exe->NumPendingTimers();
}

bool OutstationTestObject::AdvanceToNextTimer()
{
    if (exe->AdvanceToNextTimer())
    {
        return exe->RunMany() > 0;
    }

    return false;
}

size_t OutstationTestObject::AdvanceTime(const openpal::TimeDuration& td)
{
    exe->AdvanceTime(td);
    return exe->RunMany();
}

} // namespace opendnp3
