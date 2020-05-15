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
#ifndef OPENDNP3_MEASUREMENTTYPESPECS_H
#define OPENDNP3_MEASUREMENTTYPESPECS_H

#include "opendnp3/app/EventCells.h"
#include "opendnp3/app/EventTriggers.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/outstation/MeasurementConfig.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

struct BinarySpec : public BinaryInfo
{
    typedef BinaryConfig config_t;
    typedef SimpleEventCell<BinarySpec> event_cell_t;

    inline static bool IsQualityOnlineOnly(const Binary& binary)
    {
        return (binary.flags.value & 0b01111111) == static_cast<uint8_t>(BinaryQuality::ONLINE);
    }

    inline static bool IsEvent(const Binary& old_value, const Binary& new_value, const config_t& config)
    {
        return old_value.flags.value != new_value.flags.value;
    }
};

struct DoubleBitBinarySpec : public DoubleBitBinaryInfo
{
    typedef DoubleBitBinaryConfig config_t;
    typedef SimpleEventCell<DoubleBitBinarySpec> event_cell_t;

    inline static bool IsEvent(const DoubleBitBinary& old_value,
                               const DoubleBitBinary& new_value,
                               const config_t& config)
    {
        return old_value.flags.value != new_value.flags.value;
    }
};

struct BinaryOutputStatusSpec : public BinaryOutputStatusInfo
{
    typedef BOStatusConfig config_t;
    typedef SimpleEventCell<BinaryOutputStatusSpec> event_cell_t;

    inline static bool IsEvent(const BinaryOutputStatus& old_value,
                               const BinaryOutputStatus& new_value,
                               const config_t& config)
    {
        return old_value.flags.value != new_value.flags.value;
    }
};

struct AnalogSpec : public AnalogInfo
{
    typedef AnalogConfig config_t;
    typedef DeadbandEventCell<AnalogSpec> event_cell_t;

    inline static bool IsEvent(const Analog& old_value, const Analog& new_value, const config_t& config)
    {
        return measurements::IsEvent(new_value, old_value, config.deadband);
    }
};

struct CounterSpec : public CounterInfo
{
    typedef CounterConfig config_t;
    typedef DeadbandEventCell<CounterSpec> event_cell_t;

    inline static bool IsEvent(const Counter& old_value, const Counter& new_value, const config_t& config)
    {
        if (old_value.flags.value != new_value.flags.value)
        {
            return true;
        }
        else
        {
            return measurements::IsEvent<uint32_t, uint64_t>(old_value.value, new_value.value, config.deadband);
        }
    }
};

struct FrozenCounterSpec : public FrozenCounterInfo
{
    typedef FrozenCounterConfig config_t;
    typedef DeadbandEventCell<FrozenCounterSpec> event_cell_t;

    inline static bool IsEvent(const FrozenCounter& old_value, const FrozenCounter& new_value, const config_t& config)
    {
        if (old_value.flags.value != new_value.flags.value)
        {
            return true;
        }
        else
        {
            return measurements::IsEvent<uint32_t, uint64_t>(old_value.value, new_value.value, config.deadband);
        }
    }
};

struct AnalogOutputStatusSpec : public AnalogOutputStatusInfo
{
    typedef AOStatusConfig config_t;
    typedef DeadbandEventCell<AnalogOutputStatusSpec> event_cell_t;

    inline static bool IsEvent(const AnalogOutputStatus& old_value,
                               const AnalogOutputStatus& new_value,
                               const config_t& config)
    {
        return measurements::IsEvent(new_value, old_value, config.deadband);
    }
};

struct OctetStringSpec : public OctetStringInfo
{
    typedef OctetStringConfig config_t;
    typedef SimpleEventCell<OctetStringSpec> event_cell_t;

    inline static bool IsEvent(const OctetString& old_value, const OctetString& new_value, const config_t& config)
    {
        const auto old_value_buffer = old_value.ToBuffer();
        const ser4cpp::rseq_t old_value_seq(old_value_buffer.data, old_value_buffer.length);
        const auto new_value_buffer = new_value.ToBuffer();
        const ser4cpp::rseq_t new_value_seq(new_value_buffer.data, new_value_buffer.length);
        return !old_value_seq.equals(new_value_seq);
    }
};

struct TimeAndIntervalSpec : public TimeAndIntervalInfo
{
    typedef TimeAndIntervalConfig config_t;
    typedef EmptyEventCell event_cell_t;
};

} // namespace opendnp3

#endif
