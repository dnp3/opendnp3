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
#ifndef OPENDNP3_MEASUREMENTFACTORY_H
#define OPENDNP3_MEASUREMENTFACTORY_H

#include "opendnp3/app/AnalogCommandEvent.h"
#include "opendnp3/app/AnalogOutput.h"
#include "opendnp3/app/BinaryCommandEvent.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/util/StaticOnly.h"

namespace opendnp3
{

struct BinaryFactory : private StaticOnly
{
    inline static Binary From(uint8_t flags)
    {
        return Binary(Flags(flags));
    }

    inline static Binary From(uint8_t flags, uint16_t time)
    {
        return From(flags, DNPTime(time));
    }

    inline static Binary From(uint8_t flags, DNPTime time)
    {
        return Binary(Flags(flags), time);
    }
};

struct DoubleBitBinaryFactory : private StaticOnly
{
    inline static DoubleBitBinary From(uint8_t flags)
    {
        return DoubleBitBinary(Flags(flags));
    }

    inline static DoubleBitBinary From(uint8_t flags, uint16_t time)
    {
        return From(flags, DNPTime(time));
    }

    inline static DoubleBitBinary From(uint8_t flags, DNPTime time)
    {
        return DoubleBitBinary(Flags(flags), time);
    }
};

struct AnalogFactory : private StaticOnly
{
    static Analog From(int32_t count)
    {
        return Analog(count);
    }
    static Analog From(uint8_t flags, double value)
    {
        return Analog(value, Flags(flags));
    }
    static Analog From(uint8_t flags, double value, DNPTime time)
    {
        return Analog(value, Flags(flags), time);
    }
};

struct CounterFactory : private StaticOnly
{
    inline static Counter From(uint32_t count)
    {
        return Counter(count);
    }
    inline static Counter From(uint8_t flags, uint32_t count)
    {
        return Counter(count, Flags(flags));
    }
    inline static Counter From(uint8_t flags, uint32_t count, DNPTime time)
    {
        return Counter(count, Flags(flags), time);
    }
};

struct FrozenCounterFactory : private StaticOnly
{
    inline static FrozenCounter From(uint32_t count)
    {
        return FrozenCounter(count);
    }
    inline static FrozenCounter From(uint8_t flags, uint32_t count)
    {
        return FrozenCounter(count, Flags(flags));
    }
    inline static FrozenCounter From(uint8_t flags, uint32_t count, DNPTime time)
    {
        return FrozenCounter(count, Flags(flags), time);
    }
};

struct TimeAndIntervalFactory : private StaticOnly
{
    inline static TimeAndInterval From(DNPTime time, uint32_t interval, uint8_t units)
    {
        return TimeAndInterval(time, interval, units);
    }
};

struct ControlRelayOutputBlockFactory : private StaticOnly
{
    inline static ControlRelayOutputBlock From(
        uint8_t code, uint8_t count, uint32_t onTime, uint32_t offTime, uint8_t status)
    {
        return ControlRelayOutputBlock(code, count, onTime, offTime, CommandStatusSpec::from_type(status));
    }
};

struct BinaryOutputStatusFactory : private StaticOnly
{
    inline static BinaryOutputStatus From(uint8_t flags)
    {
        return BinaryOutputStatus(Flags(flags));
    }

    inline static BinaryOutputStatus From(uint8_t flags, DNPTime time)
    {
        return BinaryOutputStatus(Flags(flags), time);
    }
};

struct BinaryCommandEventFactory : private StaticOnly
{
    inline static BinaryCommandEvent From(uint8_t flags)
    {
        return BinaryCommandEvent(Flags(flags));
    }

    inline static BinaryCommandEvent From(uint8_t flags, DNPTime time)
    {
        return BinaryCommandEvent(Flags(flags), time);
    }
};

struct AnalogOutputStatusFactory : private StaticOnly
{
    inline static AnalogOutputStatus From(uint8_t flags, double value)
    {
        return AnalogOutputStatus(value, Flags(flags));
    }

    inline static AnalogOutputStatus From(uint8_t flags, double value, DNPTime time)
    {
        return AnalogOutputStatus(value, Flags(flags), time);
    }
};

struct AnalogCommandEventFactory : private StaticOnly
{
    inline static AnalogCommandEvent From(uint8_t status, double value)
    {
        return AnalogCommandEvent(value, CommandStatusSpec::from_type(status));
    }

    inline static AnalogCommandEvent From(uint8_t status, double value, DNPTime time)
    {
        return AnalogCommandEvent(value, CommandStatusSpec::from_type(status), time);
    }
};

template<class Target, class ValueType> struct AnalogOutputFactory : private StaticOnly
{
    inline static Target From(ValueType value, uint8_t status)
    {
        return Target(value, CommandStatusSpec::from_type(status));
    }
};

using AnalogOutputInt32Factory = AnalogOutputFactory<AnalogOutputInt32, int32_t>;
using AnalogOutputInt16Factory = AnalogOutputFactory<AnalogOutputInt16, int16_t>;
using AnalogOutputFloat32Factory = AnalogOutputFactory<AnalogOutputFloat32, float>;
using AnalogOutputDouble64Factory = AnalogOutputFactory<AnalogOutputDouble64, double>;

} // namespace opendnp3

#endif
