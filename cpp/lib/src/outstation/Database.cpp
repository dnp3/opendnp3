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

#include "Database.h"

namespace opendnp3
{

Database::Database(const DatabaseConfig& config,
                         IEventReceiver& event_receiver,
                         StaticTypeBitField allowed_class_zero_types)
    : allowed_class_zero_types(allowed_class_zero_types),
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
        return assign_class_to_all(this->binary_input, clazz);
    case (AssignClassType::DoubleBinaryInput):
        return assign_class_to_all(this->double_binary, clazz);
    case (AssignClassType::Counter):
        return assign_class_to_all(this->counter, clazz);
    case (AssignClassType::FrozenCounter):
        return assign_class_to_all(this->frozen_counter, clazz);
    case (AssignClassType::AnalogInput):
        return assign_class_to_all(this->analog_input, clazz);
    case (AssignClassType::BinaryOutputStatus):
        return assign_class_to_all(this->binary_output_status, clazz);
    case (AssignClassType::AnalogOutputStatus):
        return assign_class_to_all(this->analog_output_status, clazz);
    default:
        return Range::Invalid();
    }
}

Range Database::AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range)
{
    switch (type)
    {
    case (AssignClassType::BinaryInput):
        return assign_class_to_range(this->binary_input, clazz, range);
    case (AssignClassType::DoubleBinaryInput):
        return assign_class_to_range(this->double_binary, clazz, range);
    case (AssignClassType::Counter):
        return assign_class_to_range(this->counter, clazz, range);
    case (AssignClassType::FrozenCounter):
        return assign_class_to_range(this->frozen_counter, clazz, range);
    case (AssignClassType::AnalogInput):
        return assign_class_to_range(this->analog_input, clazz, range);
    case (AssignClassType::BinaryOutputStatus):
        return assign_class_to_range(this->binary_output_status, clazz, range);
    case (AssignClassType::AnalogOutputStatus):
        return assign_class_to_range(this->analog_output_status, clazz, range);
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
    // TODO
    return false;
}

bool Database::Update(const Binary& meas, uint16_t index, EventMode mode)
{
    return this->binary_input.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode)
{
    return this->double_binary.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const Analog& meas, uint16_t index, EventMode mode)
{
    return this->analog_input.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const Counter& meas, uint16_t index, EventMode mode)
{
    return this->counter.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const FrozenCounter& meas, uint16_t index, EventMode mode)
{
    return this->frozen_counter.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode)
{
    return this->binary_output_status.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode)
{
    return this->analog_output_status.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const OctetString& meas, uint16_t index, EventMode mode)
{
    return this->octet_string.update(meas, index) == UpdateResult::event;
}

bool Database::Update(const TimeAndInterval& meas, uint16_t index)
{
    return this->time_and_interval.update(meas, index) == UpdateResult::event;
}

bool Database::Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags)
{
    switch (type)
    {
    case (FlagsType::BinaryInput):
        return this->binary_input.modify(start, stop, flags);
    case (FlagsType::DoubleBinaryInput):
        return this->double_binary.modify(start, stop, flags);
    case (FlagsType::AnalogInput):
        return this->analog_input.modify(start, stop, flags);
    case (FlagsType::Counter):
        return this->counter.modify(start, stop, flags);
    case (FlagsType::FrozenCounter):
        return this->frozen_counter.modify(start, stop, flags);
    case (FlagsType::BinaryOutputStatus):
        return this->binary_output_status.modify(start, stop, flags);
    case (FlagsType::AnalogOutputStatus):
        return this->analog_output_status.modify(start, stop, flags);
    }

    return false;
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

template<class Spec> Range Database::assign_class_to_all(StaticDataMap<Spec>& map, PointClass clazz)
{
    // TODO
    return Range::Invalid();
}

template<class Spec>
Range Database::assign_class_to_range(StaticDataMap<Spec>& map, PointClass clazz, const Range& range)
{
    // TODO
    return Range::Invalid();
}

} // namespace opendnp3
