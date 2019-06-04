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

#include "StaticDataMaps.h"

namespace opendnp3
{

StaticDataMaps::StaticDataMaps(const DatabaseConfigNew& config, StaticTypeBitField allowed_class_zero_types)
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

bool StaticDataMaps::has_any_selection() const
{
    return binary_input.has_any_selection() || double_binary.has_any_selection() || analog_input.has_any_selection()
        || counter.has_any_selection() || frozen_counter.has_any_selection() || binary_output_status.has_any_selection()
        || analog_output_status.has_any_selection() || time_and_interval.has_any_selection()
        || octet_string.has_any_selection();
}

IINField StaticDataMaps::SelectAll(GroupVariation gv)
{
    if (gv == GroupVariation::Group60Var1)
    {
        this->SelectAllClassZero<BinarySpec>(this->binary_input);
        this->SelectAllClassZero<DoubleBitBinarySpec>(this->double_binary);
        this->SelectAllClassZero<BinaryOutputStatusSpec>(this->binary_output_status);
        this->SelectAllClassZero<CounterSpec>(this->counter);
        this->SelectAllClassZero<FrozenCounterSpec>(this->frozen_counter);
        this->SelectAllClassZero<AnalogSpec>(this->analog_input);
        this->SelectAllClassZero<AnalogOutputStatusSpec>(this->analog_output_status);
        this->SelectAllClassZero<TimeAndIntervalSpec>(this->time_and_interval);
        this->SelectAllClassZero<OctetStringSpec>(this->octet_string);

        return IINField::Empty();
    }

    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return SelectAll<BinarySpec>(this->binary_input);
    case (GroupVariation::Group1Var1):
        return SelectAll<BinarySpec>(this->binary_input, StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return SelectAll<BinarySpec>(this->binary_input, StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return SelectAll<DoubleBitBinarySpec>(this->double_binary);
    case (GroupVariation::Group3Var2):
        return SelectAll<DoubleBitBinarySpec>(this->double_binary, StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return SelectAll<BinaryOutputStatusSpec>(this->binary_output_status);
    case (GroupVariation::Group10Var2):
        return SelectAll<BinaryOutputStatusSpec>(this->binary_output_status,
                                                       StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return SelectAll<CounterSpec>(this->counter);
    case (GroupVariation::Group20Var1):
        return SelectAll<CounterSpec>(this->counter, StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return SelectAll<CounterSpec>(this->counter, StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return SelectAll<CounterSpec>(this->counter, StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return SelectAll<CounterSpec>(this->counter, StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter);
    case (GroupVariation::Group21Var1):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return SelectAll<FrozenCounterSpec>(this->frozen_counter, StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return SelectAll<AnalogSpec>(this->analog_input);
    case (GroupVariation::Group30Var1):
        return SelectAll<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return SelectAll<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return SelectAll<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return SelectAll<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return SelectAll<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return SelectAll<AnalogSpec>(this->analog_input, StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return SelectAll<AnalogOutputStatusSpec>(this->analog_output_status);
    case (GroupVariation::Group40Var1):
        return SelectAll<AnalogOutputStatusSpec>(this->analog_output_status,
                                                       StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return SelectAll<AnalogOutputStatusSpec>(this->analog_output_status,
                                                       StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return SelectAll<AnalogOutputStatusSpec>(this->analog_output_status,
                                                       StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return SelectAll<AnalogOutputStatusSpec>(this->analog_output_status,
                                                       StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return SelectAll<TimeAndIntervalSpec>(this->time_and_interval,
                                                    StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return SelectAll<OctetStringSpec>(this->octet_string);

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

IINField StaticDataMaps::SelectRange(GroupVariation gv, const Range& range)
{
    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return SelectRange<BinarySpec>(this->binary_input, range);
    case (GroupVariation::Group1Var1):
        return SelectRange<BinarySpec>(this->binary_input, range, StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return SelectRange<BinarySpec>(this->binary_input, range, StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return SelectRange<DoubleBitBinarySpec>(this->double_binary, range);
    case (GroupVariation::Group3Var2):
        return SelectRange<DoubleBitBinarySpec>(this->double_binary, range,
                                                      StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return SelectRange<BinaryOutputStatusSpec>(this->binary_output_status, range);
    case (GroupVariation::Group10Var2):
        return SelectRange<BinaryOutputStatusSpec>(this->binary_output_status, range,
                                                         StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return SelectRange<CounterSpec>(this->counter, range);
    case (GroupVariation::Group20Var1):
        return SelectRange<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return SelectRange<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return SelectRange<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return SelectRange<CounterSpec>(this->counter, range, StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range);
    case (GroupVariation::Group21Var1):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range,
                                                    StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range,
                                                    StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range,
                                                    StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range,
                                                    StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range,
                                                    StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return SelectRange<FrozenCounterSpec>(this->frozen_counter, range,
                                                    StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return SelectRange<AnalogSpec>(this->analog_input, range);
    case (GroupVariation::Group30Var1):
        return SelectRange<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return SelectRange<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return SelectRange<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return SelectRange<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return SelectRange<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return SelectRange<AnalogSpec>(this->analog_input, range, StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return SelectRange<AnalogOutputStatusSpec>(this->analog_output_status, range);
    case (GroupVariation::Group40Var1):
        return SelectRange<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                         StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return SelectRange<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                         StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return SelectRange<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                         StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return SelectRange<AnalogOutputStatusSpec>(this->analog_output_status, range,
                                                         StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return SelectRange<TimeAndIntervalSpec>(this->time_and_interval, range,
                                                      StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return SelectRange<OctetStringSpec>(this->octet_string, range, StaticOctetStringVariation::Group110Var0);

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

void StaticDataMaps::Unselect()
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

template<class Spec> void StaticDataMaps::SelectAllClassZero(StaticDataMap<Spec>& map)
{
    if (this->allowed_class_zero_types.IsSet(Spec::StaticTypeEnum))
    {
        SelectAll<Spec>(map);
    }
}

template<class Spec> IINField StaticDataMaps::SelectAll(StaticDataMap<Spec>& map)
{
    map.select_all();
    return IINField::Empty();
}

template<class Spec>
IINField StaticDataMaps::SelectAll(StaticDataMap<Spec>& map, typename Spec::static_variation_t variation)
{
    map.select_all(variation);
    return IINField::Empty();
}

template<class Spec> IINField StaticDataMaps::SelectRange(StaticDataMap<Spec>& map, const Range& range)
{
    const auto count = map.select(range);
    return (count != range.Count()) ? IINField(IINBit::PARAM_ERROR) : IINField::Empty();
}

template<class Spec>
IINField StaticDataMaps::SelectRange(StaticDataMap<Spec>& map,
                                     const Range& range,
                                     typename Spec::static_variation_t variation)
{
    const auto count = map.select(range, variation);
    return (count != range.Count()) ? IINField(IINBit::PARAM_ERROR) : IINField::Empty();
}

} // namespace opendnp3
