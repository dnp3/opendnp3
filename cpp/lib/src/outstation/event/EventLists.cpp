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

#include "EventLists.h"

namespace opendnp3
{

EventLists::EventLists(const EventBufferConfig& config)
    : events(config.TotalEvents()),
      binary(config.maxBinaryEvents),
      doubleBinary(config.maxDoubleBinaryEvents),
      analog(config.maxAnalogEvents),
      counter(config.maxCounterEvents),
      frozenCounter(config.maxFrozenCounterEvents),
      binaryOutputStatus(config.maxBinaryOutputStatusEvents),
      analogOutputStatus(config.maxAnalogOutputStatusEvents),
      octetString(config.maxOctetStringEvents)
{
}

bool EventLists::IsAnyTypeFull() const
{
    return this->binary.IsFullAndCapacityNotZero() || this->doubleBinary.IsFullAndCapacityNotZero()
        || this->counter.IsFullAndCapacityNotZero() || this->frozenCounter.IsFullAndCapacityNotZero()
        || this->analog.IsFullAndCapacityNotZero() || this->binaryOutputStatus.IsFullAndCapacityNotZero()
        || this->analogOutputStatus.IsFullAndCapacityNotZero() || this->octetString.IsFullAndCapacityNotZero();
}

template<> List<TypedEventRecord<BinarySpec>>& EventLists::GetList()
{
    return this->binary;
}

template<> List<TypedEventRecord<DoubleBitBinarySpec>>& EventLists::GetList()
{
    return this->doubleBinary;
}

template<> List<TypedEventRecord<CounterSpec>>& EventLists::GetList()
{
    return this->counter;
}

template<> List<TypedEventRecord<FrozenCounterSpec>>& EventLists::GetList()
{
    return this->frozenCounter;
}

template<> List<TypedEventRecord<AnalogSpec>>& EventLists::GetList()
{
    return this->analog;
}

template<> List<TypedEventRecord<BinaryOutputStatusSpec>>& EventLists::GetList()
{
    return this->binaryOutputStatus;
}

template<> List<TypedEventRecord<AnalogOutputStatusSpec>>& EventLists::GetList()
{
    return this->analogOutputStatus;
}

template<> List<TypedEventRecord<OctetStringSpec>>& EventLists::GetList()
{
    return this->octetString;
}

} // namespace opendnp3
