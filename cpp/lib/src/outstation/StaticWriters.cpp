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
#include "StaticWriters.h"

#include "gen/objects/Group1.h"
#include "gen/objects/Group10.h"
#include "gen/objects/Group20.h"
#include "gen/objects/Group21.h"
#include "gen/objects/Group3.h"
#include "gen/objects/Group30.h"
#include "gen/objects/Group40.h"
#include "gen/objects/Group50.h"
#include "outstation/OctetStringSerializer.h"

namespace opendnp3
{

template<class Spec, class IndexType>
bool LoadWithRangeIterator(StaticDataMap<Spec>& map,
                           RangeWriteIterator<IndexType, typename Spec::meas_t>& writer,
                           typename Spec::static_variation_t variation)
{
    auto next_index = map.get_selected_range().start;

    for (const auto& elem : map)
    {
        if (elem.second.variation != variation)
        {
            // the variation has changed
            return true;
        }

        if (elem.first != next_index)
        {
            // we've loaded all we can with a contiguous range
            return true;
        }

        if (!writer.Write(elem.second.value))
        {
            return false;
        }

        ++next_index;
    }

    return true;
}

template<class Spec, class IndexType>
bool LoadWithBitfieldIterator(StaticDataMap<Spec>& map,
                              BitfieldRangeWriteIterator<IndexType>& iter,
                              typename Spec::static_variation_t variation)
{
    auto next_index = map.get_selected_range().start;

    for (const auto& elem : map)
    {
        if (elem.second.variation != variation)
        {
            // the variation has changed
            return true;
        }

        if (elem.first != next_index)
        {
            // we've loaded all we can with a contiguous range
            return true;
        }

        if (!iter.Write(elem.second.value.value))
        {
            return false;
        }

        ++next_index;
    }

    return true;
}

template<class Spec, class GV> bool WriteSingleBitfield(StaticDataMap<Spec>& map, HeaderWriter& writer)
{
    const auto range = map.get_selected_range();

    if (range.IsOneByte())
    {
        auto write_iter = writer.IterateOverSingleBitfield<ser4cpp::UInt8>(GV::ID(), QualifierCode::UINT8_START_STOP,
                                                                           static_cast<uint8_t>(range.start));
        return LoadWithBitfieldIterator<Spec, ser4cpp::UInt8>(map, write_iter, GV::svariation);
    }

    auto write_iter
        = writer.IterateOverSingleBitfield<ser4cpp::UInt16>(GV::ID(), QualifierCode::UINT16_START_STOP, range.start);
    return LoadWithBitfieldIterator<Spec, ser4cpp::UInt16>(map, write_iter, GV::svariation);
}

template<class Spec, class Serializer> bool WriteWithSerializer(StaticDataMap<Spec>& map, HeaderWriter& writer)
{
    const auto range = map.get_selected_range();

    if (range.IsOneByte())
    {
        auto iter = writer.IterateOverRange<ser4cpp::UInt8, typename Serializer::Target>(
            QualifierCode::UINT8_START_STOP, Serializer::Inst(), static_cast<uint8_t>(range.start));
        return LoadWithRangeIterator<Spec, ser4cpp::UInt8>(map, iter, Serializer::svariation);
    }

    auto iter = writer.IterateOverRange<ser4cpp::UInt16, typename Serializer::Target>(QualifierCode::UINT16_START_STOP,
                                                                                      Serializer::Inst(), range.start);
    return LoadWithRangeIterator<Spec, ser4cpp::UInt16>(map, iter, Serializer::svariation);
}

static_write_func_t<BinarySpec> StaticWriters::get(StaticBinaryVariation variation)
{
    switch (variation)
    {
    case (StaticBinaryVariation::Group1Var1):
        return &WriteSingleBitfield<BinarySpec, Group1Var1>;
    default:
        return &WriteWithSerializer<BinarySpec, Group1Var2>;
    }
}

static_write_func_t<DoubleBitBinarySpec> StaticWriters::get(StaticDoubleBinaryVariation variation)
{
    switch (variation)
    {
    case (StaticDoubleBinaryVariation::Group3Var2):
        return &WriteWithSerializer<DoubleBitBinarySpec, Group3Var2>;
    default:
        return &WriteWithSerializer<DoubleBitBinarySpec, Group3Var2>;
    }
}

static_write_func_t<AnalogSpec> StaticWriters::get(StaticAnalogVariation variation)
{
    switch (variation)
    {
    case (StaticAnalogVariation::Group30Var1):
        return &WriteWithSerializer<AnalogSpec, Group30Var1>;
    case (StaticAnalogVariation::Group30Var2):
        return &WriteWithSerializer<AnalogSpec, Group30Var2>;
    case (StaticAnalogVariation::Group30Var3):
        return &WriteWithSerializer<AnalogSpec, Group30Var3>;
    case (StaticAnalogVariation::Group30Var4):
        return &WriteWithSerializer<AnalogSpec, Group30Var4>;
    case (StaticAnalogVariation::Group30Var5):
        return &WriteWithSerializer<AnalogSpec, Group30Var5>;
    case (StaticAnalogVariation::Group30Var6):
        return &WriteWithSerializer<AnalogSpec, Group30Var6>;
    default:
        return &WriteWithSerializer<AnalogSpec, Group30Var1>;
    }
}

static_write_func_t<CounterSpec> StaticWriters::get(StaticCounterVariation variation)
{
    switch (variation)
    {
    case (StaticCounterVariation::Group20Var1):
        return &WriteWithSerializer<CounterSpec, Group20Var1>;
    case (StaticCounterVariation::Group20Var2):
        return &WriteWithSerializer<CounterSpec, Group20Var2>;
    case (StaticCounterVariation::Group20Var5):
        return &WriteWithSerializer<CounterSpec, Group20Var5>;
    case (StaticCounterVariation::Group20Var6):
        return &WriteWithSerializer<CounterSpec, Group20Var6>;
    default:
        return &WriteWithSerializer<CounterSpec, Group20Var1>;
    }
}

static_write_func_t<FrozenCounterSpec> StaticWriters::get(StaticFrozenCounterVariation variation)
{
    switch (variation)
    {
    case (StaticFrozenCounterVariation::Group21Var1):
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var1>;
    case (StaticFrozenCounterVariation::Group21Var2):
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var2>;
    case (StaticFrozenCounterVariation::Group21Var5):
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var5>;
    case (StaticFrozenCounterVariation::Group21Var6):
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var6>;
    case (StaticFrozenCounterVariation::Group21Var9):
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var9>;
    case (StaticFrozenCounterVariation::Group21Var10):
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var10>;
    default:
        return &WriteWithSerializer<FrozenCounterSpec, Group21Var1>;
    }
}

static_write_func_t<BinaryOutputStatusSpec> StaticWriters::get(StaticBinaryOutputStatusVariation variation)
{
    switch (variation)
    {
    case (StaticBinaryOutputStatusVariation::Group10Var2):
        return &WriteWithSerializer<BinaryOutputStatusSpec, Group10Var2>;
    default:
        return &WriteWithSerializer<BinaryOutputStatusSpec, Group10Var2>;
    }
}

static_write_func_t<AnalogOutputStatusSpec> StaticWriters::get(StaticAnalogOutputStatusVariation variation)
{
    switch (variation)
    {
    case (StaticAnalogOutputStatusVariation::Group40Var1):
        return &WriteWithSerializer<AnalogOutputStatusSpec, Group40Var1>;
    case (StaticAnalogOutputStatusVariation::Group40Var2):
        return &WriteWithSerializer<AnalogOutputStatusSpec, Group40Var2>;
    case (StaticAnalogOutputStatusVariation::Group40Var3):
        return &WriteWithSerializer<AnalogOutputStatusSpec, Group40Var3>;
    case (StaticAnalogOutputStatusVariation::Group40Var4):
        return &WriteWithSerializer<AnalogOutputStatusSpec, Group40Var4>;
    default:
        return &WriteWithSerializer<AnalogOutputStatusSpec, Group40Var1>;
    }
}

bool write_octet_strings(StaticDataMap<OctetStringSpec>& map, HeaderWriter& writer);

static_write_func_t<OctetStringSpec> StaticWriters::get(StaticOctetStringVariation /*variation*/)
{
    return write_octet_strings;
}

static_write_func_t<TimeAndIntervalSpec> StaticWriters::get(StaticTimeAndIntervalVariation /*variation*/)
{
    return &WriteWithSerializer<TimeAndIntervalSpec, Group50Var4>;
}

template<class Writer> bool write_some_octet_strings(StaticDataMap<OctetStringSpec>& map, Writer& writer)
{
    bool first = true;
    uint8_t last_length = 0;
    uint16_t next_index = 0;

    for (const auto& elem : map)
    {

        if(!first) {

            if(next_index != elem.first) {
                // discontiguous indices
                return true;
            }

            if(last_length != elem.second.value.Size()) {
                // different lengths
                return true;
            }
        }

        first = false;
        next_index = elem.first + 1;
        last_length = elem.second.value.Size();

        if (!writer.Write(elem.second.value))
        {
            return false;
        }
    }

    return true;
}

bool write_octet_strings(StaticDataMap<OctetStringSpec>& map, HeaderWriter& writer)
{
    const auto range = map.get_selected_range();

    const uint8_t starting_size = (*map.begin()).second.value.Size();
    const OctetStringSerializer serializer(false, starting_size);

    if (range.IsOneByte())
    {
        auto iter = writer.IterateOverRange<ser4cpp::UInt8>(QualifierCode::UINT8_START_STOP, serializer,
                                                            static_cast<uint8_t>(range.start));
        return write_some_octet_strings(map, iter);
    }

    auto iter = writer.IterateOverRange<ser4cpp::UInt16>(QualifierCode::UINT16_START_STOP, serializer, range.start);
    return write_some_octet_strings(map, iter);
}

} // namespace opendnp3
