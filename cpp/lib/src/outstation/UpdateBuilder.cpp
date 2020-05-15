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

#include "opendnp3/outstation/UpdateBuilder.h"

namespace opendnp3
{

Updates UpdateBuilder::Build()
{
    return Updates(std::move(this->updates));
}

bool UpdateBuilder::Update(const Binary& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::Update(const Analog& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::Update(const Counter& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::FreezeCounter(uint16_t index, bool clear, EventMode mode)
{
    this->Add([=](IUpdateHandler& handler) { handler.FreezeCounter(index, clear, mode); });
    return true;
}

bool UpdateBuilder::Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::Update(const OctetString& meas, uint16_t index, EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

bool UpdateBuilder::Update(const TimeAndInterval& meas, uint16_t index)
{
    return this->Add([=](IUpdateHandler& handler) { handler.Update(meas, index); });
}

bool UpdateBuilder::Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags)
{
    return this->Add([=](IUpdateHandler& handler) { handler.Modify(type, start, stop, flags); });
}

template<class T> bool UpdateBuilder::AddMeas(const T& meas, uint16_t index, EventMode mode)
{
    return this->Add([=](IUpdateHandler& handler) { handler.Update(meas, index, mode); });
}

bool UpdateBuilder::Add(const update_func_t& fun)
{
    if (!this->updates)
    {
        this->updates = std::make_shared<shared_updates_t>();
    }

    updates->push_back(fun);

    return true;
}

} // namespace opendnp3
