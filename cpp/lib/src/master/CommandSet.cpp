/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "opendnp3/master/CommandSet.h"

#include "gen/objects/Group12.h"
#include "gen/objects/Group41.h"
#include "master/TypedCommandHeader.h"

namespace opendnp3
{

CommandSet::CommandSet(CommandSet&& other) : m_headers(std::move(other.m_headers)) {}

CommandSet::~CommandSet() = default;

CommandSet::CommandSet(std::initializer_list<Indexed<ControlRelayOutputBlock>> items)
{
    this->Add(items);
}

CommandSet::CommandSet(std::initializer_list<Indexed<AnalogOutputInt16>> items)
{
    this->Add(items);
}

CommandSet::CommandSet(std::initializer_list<Indexed<AnalogOutputInt32>> items)
{
    this->Add(items);
}

CommandSet::CommandSet(std::initializer_list<Indexed<AnalogOutputFloat32>> items)
{
    this->Add(items);
}

CommandSet::CommandSet(std::initializer_list<Indexed<AnalogOutputDouble64>> items)
{
    this->Add(items);
}

ICommandCollection<ControlRelayOutputBlock>& CommandSet::StartHeaderCROB()
{
    const auto header = std::make_shared<TypedCommandHeader<ControlRelayOutputBlock>>(Group12Var1::Inst());
    this->m_headers.push_back(header);
    return *header;
}

ICommandCollection<AnalogOutputInt32>& CommandSet::StartHeaderAOInt32()
{
    const auto header = std::make_shared<TypedCommandHeader<AnalogOutputInt32>>(Group41Var1::Inst());
    this->m_headers.push_back(header);
    return *header;
}

ICommandCollection<AnalogOutputInt16>& CommandSet::StartHeaderAOInt16()
{
    const auto header = std::make_shared<TypedCommandHeader<AnalogOutputInt16>>(Group41Var2::Inst());
    this->m_headers.push_back(header);
    return *header;
}

ICommandCollection<AnalogOutputFloat32>& CommandSet::StartHeaderAOFloat32()
{
    const auto header = std::make_shared<TypedCommandHeader<AnalogOutputFloat32>>(Group41Var3::Inst());
    this->m_headers.push_back(header);
    return *header;
}

ICommandCollection<AnalogOutputDouble64>& CommandSet::StartHeaderAODouble64()
{
    const auto header = std::make_shared<TypedCommandHeader<AnalogOutputDouble64>>(Group41Var4::Inst());
    this->m_headers.push_back(header);
    return *header;
}

} // namespace opendnp3
