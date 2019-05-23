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

#include "asiodnp3/UpdateBuilder.h"

using namespace opendnp3;

namespace asiodnp3
{

Updates UpdateBuilder::Build()
{
    return Updates(std::move(this->updates));
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::Binary& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::DoubleBitBinary& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::Analog& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::Counter& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::FrozenCounter& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::BinaryOutputStatus& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::AnalogOutputStatus& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::OctetString& meas, uint16_t index, opendnp3::EventMode mode)
{
    return this->AddMeas(meas, index, mode);
}

UpdateBuilder& UpdateBuilder::Update(const opendnp3::TimeAndInterval& meas, uint16_t index)
{
    this->Add([=](IUpdateHandler& handler) { handler.Update(meas, index); });
    return *this;
}

UpdateBuilder& UpdateBuilder::Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags)
{
    this->Add([=](IUpdateHandler& handler) { handler.Modify(type, start, stop, flags); });
    return *this;
}

template<class T> UpdateBuilder& UpdateBuilder::AddMeas(const T& meas, uint16_t index, opendnp3::EventMode mode)
{
    this->Add([=](IUpdateHandler& handler) { handler.Update(meas, index, mode); });
    return *this;
}

void UpdateBuilder::Add(const update_func_t& fun)
{
    if (!this->updates)
    {
        this->updates = std::make_shared<shared_updates_t>();
    }

    updates->push_back(fun);
}

} // namespace asiodnp3
