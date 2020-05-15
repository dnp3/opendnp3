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

#include "Database.h"

#include "outstation/StaticWriters.h"

namespace opendnp3
{

template<class Spec>
IINField select_indices(StaticDataMap<Spec>& map,
                        const ICollection<uint16_t>& indices,
                        typename Spec::static_variation_t variation)
{
    auto missing_index = false;
    auto select = [&](uint16_t index) {
        if (!map.select(index, variation))
        {
            missing_index = true;
        }
    };
    indices.ForeachItem(select);

    return missing_index ? IINField(IINBit::PARAM_ERROR) : IINField::Empty();
}

template<class Spec> IINField select_indices(StaticDataMap<Spec>& map, const ICollection<uint16_t>& indices)
{
    auto missing_index = false;
    auto select = [&](uint16_t index) {
        if (!map.select(index))
        {
            missing_index = true;
        }
    };
    indices.ForeachItem(select);

    return missing_index ? IINField(IINBit::PARAM_ERROR) : IINField::Empty();
}

template<class Spec> bool load_type(StaticDataMap<Spec>& map, HeaderWriter& writer)
{
    while (true)
    {
        auto iter = map.begin();

        if (iter == map.end())
        {
            // there is no data left to write
            return true;
        }

        if (!StaticWriters::get((*iter).second.variation)(map, writer))
        {
            // the APDU is full
            return false;
        }
    }
}

Database::Database(const DatabaseConfig& config,
                   IEventReceiver& event_receiver,
                   IDnpTimeSource& time_source,
                   StaticTypeBitField allowed_class_zero_types)
    : event_receiver(event_receiver),
      time_source(time_source),
      allowed_class_zero_types(allowed_class_zero_types),
      binary_input(config.binary_input),
      double_binary(config.double_binary),
      analog_input(config.analog_input),
      counter(config.counter),
      frozen_counter(config.frozen_counter),
      binary_output_status(config.binary_output_status),
      analog_output_status(config.analog_output_status),
      time_and_interval(config.time_and_interval),
      octet_string(config.octet_string)
{
}

IINField Database::SelectAll(GroupVariation gv)
{
    if (gv == GroupVariation::Group60Var1)
    {
        this->select_all_class_zero<BinarySpec>(this->binary_input);
        this->select_all_class_zero<DoubleBitBinarySpec>(this->double_binary);
        this->select_all_class_zero<BinaryOutputStatusSpec>(this->binary_output_status);
        this->select_all_class_zero<CounterSpec>(this->counter);
        this->select_all_class_zero<FrozenCounterSpec>(this->frozen_counter);
        this->select_all_class_zero<AnalogSpec>(this->analog_input);
        this->select_all_class_zero<AnalogOutputStatusSpec>(this->analog_output_status);
        this->select_all_class_zero<TimeAndIntervalSpec>(this->time_and_interval);
        this->select_all_class_zero<OctetStringSpec>(this->octet_string);

        return IINField::Empty();
    }

    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return select_all<BinarySpec>(this->binary_input);
    case (GroupVariation::Group1Var1):
        return select_all<BinarySpec>(this->binary_input, StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return select_all<BinarySpec>(this->binary_input, StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return select_all<DoubleBitBinarySpec>(this->double_binary);
    case (GroupVariation::Group3Var2):
        return select_all<DoubleBitBinarySpec>(this->double_binary, StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return select_all<BinaryOutputStatusSpec>(this->binary_output_status);
    case (GroupVariation::Group10Var2):
        return select_all<BinaryOutputStatusSpec>(this->binary_output_status,
                                                  StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return select_all<CounterSpec>(this->counter);
    case (GroupVariation::Group20Var1):
        return select_all<CounterSpec>(this->counter, StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return select_all<CounterSpec>(this->counter, StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return select_all<CounterSpec>(this->counter, StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return select_all<CounterSpec>(this->counter, StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return select_all<FrozenCounterSpec>(this->frozen_counter);
    case (GroupVariation::Group21Var1):
        return select_all<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return select_all<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return select_all<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return select_all<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return select_all<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return select_all<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return select_all<AnalogSpec>(this->analog_input);
    case (GroupVariation::Group30Var1):
        return select_all<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return select_all<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return select_all<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return select_all<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return select_all<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return select_all<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return select_all<AnalogOutputStatusSpec>(this->analog_output_status);
    case (GroupVariation::Group40Var1):
        return select_all<AnalogOutputStatusSpec>(this->analog_output_status,
                                                  StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return select_all<AnalogOutputStatusSpec>(this->analog_output_status,
                                                  StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return select_all<AnalogOutputStatusSpec>(this->analog_output_status,
                                                  StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return select_all<AnalogOutputStatusSpec>(this->analog_output_status,
                                                  StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return select_all<TimeAndIntervalSpec>(this->time_and_interval, StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return select_all<OctetStringSpec>(this->octet_string);

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

IINField Database::SelectRange(GroupVariation gv, const Range& range)
{
    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return select_range<BinarySpec>(this->binary_input, range);
    case (GroupVariation::Group1Var1):
        return select_range<BinarySpec>(this->binary_input, range, StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return select_range<BinarySpec>(this->binary_input, range, StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return select_range<DoubleBitBinarySpec>(this->double_binary, range);
    case (GroupVariation::Group3Var2):
        return select_range<DoubleBitBinarySpec>(this->double_binary, range, StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return select_range<BinaryOutputStatusSpec>(this->binary_output_status, range);
    case (GroupVariation::Group10Var2):
        return select_range<BinaryOutputStatusSpec>(this->binary_output_status, range,
                                                    StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return select_range<CounterSpec>(this->counter, range);
    case (GroupVariation::Group20Var1):
        return select_range<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return select_range<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return select_range<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return select_range<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range);
    case (GroupVariation::Group21Var1):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range, StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range, StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range, StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range, StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range, StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return select_range<FrozenCounterSpec>(this->frozen_counter, range, StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return select_range<AnalogSpec>(this->analog_input, range);
    case (GroupVariation::Group30Var1):
        return select_range<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return select_range<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return select_range<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return select_range<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return select_range<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return select_range<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return select_range<AnalogOutputStatusSpec>(this->analog_output_status, range);
    case (GroupVariation::Group40Var1):
        return select_range<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                    StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return select_range<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                    StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return select_range<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                    StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return select_range<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                    StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return select_range<TimeAndIntervalSpec>(this->time_and_interval, range,
                                                 StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return select_range<OctetStringSpec>(this->octet_string, range, StaticOctetStringVariation::Group110Var0);

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

IINField Database::SelectIndices(GroupVariation gv, const ICollection<uint16_t>& indices)
{
    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return select_indices(this->binary_input, indices);
    case (GroupVariation::Group1Var1):
        return select_indices(this->binary_input, indices, StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return select_indices(this->binary_input, indices, StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return select_indices(this->double_binary, indices);
    case (GroupVariation::Group3Var2):
        return select_indices(this->double_binary, indices, StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return select_indices(this->binary_output_status, indices);
    case (GroupVariation::Group10Var2):
        return select_indices(this->binary_output_status, indices, StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return select_indices(this->counter, indices);
    case (GroupVariation::Group20Var1):
        return select_indices(this->counter, indices, StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return select_indices(this->counter, indices, StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return select_indices(this->counter, indices, StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return select_indices(this->counter, indices, StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return select_indices(this->frozen_counter, indices);
    case (GroupVariation::Group21Var1):
        return select_indices(this->frozen_counter, indices, StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return select_indices(this->frozen_counter, indices, StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return select_indices(this->frozen_counter, indices, StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return select_indices(this->frozen_counter, indices, StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return select_indices(this->frozen_counter, indices, StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return select_indices(this->frozen_counter, indices, StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return select_indices<AnalogSpec>(this->analog_input, indices);
    case (GroupVariation::Group30Var1):
        return select_indices(this->analog_input, indices, StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return select_indices(this->analog_input, indices, StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return select_indices(this->analog_input, indices, StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return select_indices(this->analog_input, indices, StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return select_indices(this->analog_input, indices, StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return select_indices(this->analog_input, indices, StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return select_indices(this->analog_output_status, indices);
    case (GroupVariation::Group40Var1):
        return select_indices(this->analog_output_status, indices, StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return select_indices(this->analog_output_status, indices, StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return select_indices(this->analog_output_status, indices, StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return select_indices(this->analog_output_status, indices, StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return select_indices(this->time_and_interval, indices, StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return select_indices(this->octet_string, indices, StaticOctetStringVariation::Group110Var0);

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

void Database::Unselect()
{
    this->binary_input.clear_selection();
    this->double_binary.clear_selection();
    this->binary_output_status.clear_selection();
    this->counter.clear_selection();
    this->frozen_counter.clear_selection();
    this->analog_input.clear_selection();
    this->analog_output_status.clear_selection();
    this->time_and_interval.clear_selection();
    this->octet_string.clear_selection();
}

Range Database::AssignClassToAll(AssignClassType type, PointClass clazz)
{
    switch (type)
    {
    case (AssignClassType::BinaryInput):
        return this->binary_input.assign_class(clazz);
    case (AssignClassType::DoubleBinaryInput):
        return this->double_binary.assign_class(clazz);
    case (AssignClassType::Counter):
        return this->counter.assign_class(clazz);
    case (AssignClassType::FrozenCounter):
        return this->frozen_counter.assign_class(clazz);
    case (AssignClassType::AnalogInput):
        return this->analog_input.assign_class(clazz);
    case (AssignClassType::BinaryOutputStatus):
        return this->binary_output_status.assign_class(clazz);
    case (AssignClassType::AnalogOutputStatus):
        return this->analog_output_status.assign_class(clazz);
    default:
        return Range::Invalid();
    }
}

Range Database::AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range)
{
    switch (type)
    {
    case (AssignClassType::BinaryInput):
        return this->binary_input.assign_class(clazz, range);
    case (AssignClassType::DoubleBinaryInput):
        return this->double_binary.assign_class(clazz, range);
    case (AssignClassType::Counter):
        return this->counter.assign_class(clazz, range);
    case (AssignClassType::FrozenCounter):
        return this->frozen_counter.assign_class(clazz, range);
    case (AssignClassType::AnalogInput):
        return this->analog_input.assign_class(clazz, range);
    case (AssignClassType::BinaryOutputStatus):
        return this->binary_output_status.assign_class(clazz, range);
    case (AssignClassType::AnalogOutputStatus):
        return this->analog_output_status.assign_class(clazz, range);
    default:
        return Range::Invalid();
    }
}

bool Database::HasAnySelection() const
{
    return binary_input.has_any_selection() || double_binary.has_any_selection() || analog_input.has_any_selection()
        || counter.has_any_selection() || frozen_counter.has_any_selection() || binary_output_status.has_any_selection()
        || analog_output_status.has_any_selection() || time_and_interval.has_any_selection()
        || octet_string.has_any_selection();
}

bool Database::Load(HeaderWriter& writer)
{
    return load_type(this->binary_input, writer) && load_type(this->double_binary, writer)
        && load_type(this->analog_input, writer) && load_type(this->counter, writer)
        && load_type(this->frozen_counter, writer) && load_type(this->binary_output_status, writer)
        && load_type(this->analog_output_status, writer) && load_type(this->time_and_interval, writer)
        && load_type(this->octet_string, writer);
}

bool Database::Update(const Binary& meas, uint16_t index, EventMode mode)
{
    return this->binary_input.update(meas, index, mode, event_receiver);
}

bool Database::Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode)
{
    return this->double_binary.update(meas, index, mode, event_receiver);
}

bool Database::Update(const Analog& meas, uint16_t index, EventMode mode)
{
    return this->analog_input.update(meas, index, mode, event_receiver);
}

bool Database::Update(const Counter& meas, uint16_t index, EventMode mode)
{
    return this->counter.update(meas, index, mode, event_receiver);
}

bool Database::FreezeCounter(uint16_t index, bool clear, EventMode mode)
{
    auto num_selected = this->counter.select(Range::From(index, index));
    this->FreezeSelectedCounters(clear, mode);

    return num_selected > 0;
}

bool Database::Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode)
{
    return this->binary_output_status.update(meas, index, mode, event_receiver);
}

bool Database::Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode)
{
    return this->analog_output_status.update(meas, index, mode, event_receiver);
}

bool Database::Update(const OctetString& meas, uint16_t index, EventMode mode)
{
    return this->octet_string.update(meas, index, mode, event_receiver);
}

bool Database::Update(const TimeAndInterval& meas, uint16_t index)
{
    return this->time_and_interval.update(meas, index, EventMode::Suppress, event_receiver);
}

bool Database::Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags)
{
    switch (type)
    {
    case (FlagsType::BinaryInput):
        return this->binary_input.modify(start, stop, flags, this->event_receiver);
    case (FlagsType::DoubleBinaryInput):
        return this->double_binary.modify(start, stop, flags, this->event_receiver);
    case (FlagsType::AnalogInput):
        return this->analog_input.modify(start, stop, flags, this->event_receiver);
    case (FlagsType::Counter):
        return this->counter.modify(start, stop, flags, this->event_receiver);
    case (FlagsType::FrozenCounter):
        return this->frozen_counter.modify(start, stop, flags, this->event_receiver);
    case (FlagsType::BinaryOutputStatus):
        return this->binary_output_status.modify(start, stop, flags, this->event_receiver);
    case (FlagsType::AnalogOutputStatus):
        return this->analog_output_status.modify(start, stop, flags, this->event_receiver);
    }

    return false;
}

bool Database::FreezeSelectedCounters(bool clear, EventMode mode)
{
    for (auto c : this->counter)
    {
        FrozenCounter new_value(c.second.value.value, c.second.value.flags, time_source.Now());
        this->frozen_counter.update(new_value, c.first, mode, this->event_receiver);

        if (clear)
        {
            c.second.value.value = 0;
            c.second.value.time = time_source.Now();
            this->counter.update(c.second.value, c.first, mode, this->event_receiver);
        }
    }

    this->counter.clear_selection();

    return true;
}

template<class Spec> void Database::select_all_class_zero(StaticDataMap<Spec>& map)
{
    if (this->allowed_class_zero_types.IsSet(Spec::StaticTypeEnum))
    {
        select_all<Spec>(map);
    }
}

template<class Spec> IINField Database::select_all(StaticDataMap<Spec>& map)
{
    map.select_all();
    return IINField::Empty();
}

template<class Spec>
IINField Database::select_all(StaticDataMap<Spec>& map, typename Spec::static_variation_t variation)
{
    map.select_all(variation);
    return IINField::Empty();
}

template<class Spec> IINField Database::select_range(StaticDataMap<Spec>& map, const Range& range)
{
    const auto count = map.select(range);
    return (count != range.Count()) ? IINField(IINBit::PARAM_ERROR) : IINField::Empty();
}

template<class Spec>
IINField Database::select_range(StaticDataMap<Spec>& map,
                                const Range& range,
                                typename Spec::static_variation_t variation)
{
    const auto count = map.select(range, variation);
    return (count != range.Count()) ? IINField(IINBit::PARAM_ERROR) : IINField::Empty();
}

} // namespace opendnp3
