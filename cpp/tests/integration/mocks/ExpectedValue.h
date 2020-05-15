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

#ifndef OPENDNP3_INTEGRATIONTESTS_EXPECTEDVALUE_H
#define OPENDNP3_INTEGRATIONTESTS_EXPECTEDVALUE_H

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/gen/StaticTypeBitmask.h>

#include <cstdint>
#include <sstream>

enum class ValueType
{
    Binary,
    DoubleBitBinary,
    Analog,
    Counter,
    FrozenCounter,
    AnalogOutputStatus,
    BinaryOutputStatus
};

class ExpectedValue final
{
    ExpectedValue(int64_t value, uint16_t index, ValueType type) : value(value), index(index), type(type) {}

public:
    static const char* ToString(ValueType vt)
    {
        switch (vt)
        {
        case (ValueType::Binary):
            return "Binary";
        case (ValueType::DoubleBitBinary):
            return "DoubleBitBinary";
        case (ValueType::Analog):
            return "Analog";
        case (ValueType::Counter):
            return "Counter";
        case (ValueType::FrozenCounter):
            return "FrozenCounter";
        case (ValueType::AnalogOutputStatus):
        default:
            return "BinaryOutputStatus";
        }
    }

    bool Equals(const ExpectedValue& other) const
    {
        return (type == other.type) && (index == other.index) && (value == other.value);
    }

    int64_t value;
    uint16_t index;
    ValueType type;

    ExpectedValue(const opendnp3::Binary& value, uint16_t index) : ExpectedValue(value.value, index, ValueType::Binary)
    {
    }
    ExpectedValue(const opendnp3::DoubleBitBinary& value, uint16_t index)
        : ExpectedValue(static_cast<int64_t>(value.value), index, ValueType::DoubleBitBinary)
    {
    }
    ExpectedValue(const opendnp3::Analog& value, uint16_t index)
        : ExpectedValue(static_cast<int64_t>(value.value), index, ValueType::Analog)
    {
    }
    ExpectedValue(const opendnp3::Counter& value, uint16_t index)
        : ExpectedValue(value.value, index, ValueType::Counter)
    {
    }
    ExpectedValue(const opendnp3::FrozenCounter& value, uint16_t index)
        : ExpectedValue(value.value, index, ValueType::FrozenCounter)
    {
    }
    ExpectedValue(const opendnp3::AnalogOutputStatus& value, uint16_t index)
        : ExpectedValue(static_cast<int64_t>(value.value), index, ValueType::AnalogOutputStatus)
    {
    }
    ExpectedValue(const opendnp3::BinaryOutputStatus& value, uint16_t index)
        : ExpectedValue(value.value, index, ValueType::BinaryOutputStatus)
    {
    }
};

inline std::ostream& operator<<(std::ostream& os, const ExpectedValue& v)
{
    os << v.index << " - " << ExpectedValue::ToString(v.type) << "(" << v.value << ")";
    return os;
}

#endif
