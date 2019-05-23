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
#include "StaticWriters.h"

#include "opendnp3/objects/Group1.h"
#include "opendnp3/objects/Group10.h"
#include "opendnp3/objects/Group20.h"
#include "opendnp3/objects/Group21.h"
#include "opendnp3/objects/Group3.h"
#include "opendnp3/objects/Group30.h"
#include "opendnp3/objects/Group40.h"
#include "opendnp3/objects/Group50.h"
#include "opendnp3/outstation/OctetStringSerializer.h"

using namespace openpal;

namespace opendnp3
{

template<class Spec, class IndexType>
bool LoadWithRangeIterator(openpal::ArrayView<Cell<Spec>, uint16_t>& view,
                           RangeWriteIterator<IndexType, typename Spec::meas_t>& iterator,
                           Range& range)
{
    const Cell<Spec>& start = view[range.start];
    uint16_t nextIndex = start.config.vIndex;

    while (range.IsValid() && view[range.start].selection.selected
           && (view[range.start].selection.variation == start.selection.variation)
           && (view[range.start].config.vIndex == nextIndex))
    {
        if (iterator.Write(view[range.start].selection.value))
        {
            // deselect the value and advance the range
            view[range.start].selection.selected = false;
            range.Advance();
            ++nextIndex;
        }
        else
        {
            return false;
        }
    }

    return true;
}

template<class Spec, class IndexType>
bool LoadWithBitfieldIterator(openpal::ArrayView<Cell<Spec>, uint16_t>& view,
                              BitfieldRangeWriteIterator<IndexType>& iterator,
                              Range& range)
{
    const Cell<Spec>& start = view[range.start];

    uint16_t nextIndex = start.config.vIndex;

    while (range.IsValid() && view[range.start].selection.selected
           && (view[range.start].selection.variation == start.selection.variation)
           && (view[range.start].config.vIndex == nextIndex))
    {
        if (iterator.Write(view[range.start].selection.value.value))
        {
            // deselect the value and advance the range
            view[range.start].selection.selected = false;
            range.Advance();
            ++nextIndex;
        }
        else
        {
            return false;
        }
    }

    return true;
}

template<class Spec, class GV>
bool WriteSingleBitfield(openpal::ArrayView<Cell<Spec>, uint16_t>& view, HeaderWriter& writer, Range& range)
{
    auto start = view[range.start].config.vIndex;
    auto stop = view[range.stop].config.vIndex;
    auto mapped = Range::From(start, stop);

    if (mapped.IsOneByte())
    {
        auto iter = writer.IterateOverSingleBitfield<openpal::UInt8>(GV::ID(), QualifierCode::UINT8_START_STOP,
                                                                     static_cast<uint8_t>(mapped.start));
        return LoadWithBitfieldIterator<Spec, openpal::UInt8>(view, iter, range);
    }

    auto iter
        = writer.IterateOverSingleBitfield<openpal::UInt16>(GV::ID(), QualifierCode::UINT16_START_STOP, mapped.start);
    return LoadWithBitfieldIterator<Spec, openpal::UInt16>(view, iter, range);
}

template<class Spec, class Serializer>
bool WriteWithSerializer(openpal::ArrayView<Cell<Spec>, uint16_t>& view, HeaderWriter& writer, Range& range)
{
    auto start = view[range.start].config.vIndex;
    auto stop = view[range.stop].config.vIndex;
    auto mapped = Range::From(start, stop);

    if (mapped.IsOneByte())
    {
        auto iter = writer.IterateOverRange<openpal::UInt8, typename Serializer::Target>(
            QualifierCode::UINT8_START_STOP, Serializer::Inst(), static_cast<uint8_t>(mapped.start));
        return LoadWithRangeIterator<Spec, openpal::UInt8>(view, iter, range);
    }

    auto iter = writer.IterateOverRange<openpal::UInt16, typename Serializer::Target>(QualifierCode::UINT16_START_STOP,
                                                                                      Serializer::Inst(), mapped.start);
    return LoadWithRangeIterator<Spec, openpal::UInt16>(view, iter, range);
}

StaticWrite<BinarySpec>::func_t StaticWriters::Get(StaticBinaryVariation variation)
{
    switch (variation)
    {
    case (StaticBinaryVariation::Group1Var1):
        return &WriteSingleBitfield<BinarySpec, Group1Var1>;
    default:
        return &WriteWithSerializer<BinarySpec, Group1Var2>;
    }
}

StaticWrite<DoubleBitBinarySpec>::func_t StaticWriters::Get(StaticDoubleBinaryVariation variation)
{
    switch (variation)
    {
    case (StaticDoubleBinaryVariation::Group3Var2):
        return &WriteWithSerializer<DoubleBitBinarySpec, Group3Var2>;
    default:
        return &WriteWithSerializer<DoubleBitBinarySpec, Group3Var2>;
    }
}

StaticWrite<AnalogSpec>::func_t StaticWriters::Get(StaticAnalogVariation variation)
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

StaticWrite<CounterSpec>::func_t StaticWriters::Get(StaticCounterVariation variation)
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

StaticWrite<FrozenCounterSpec>::func_t StaticWriters::Get(StaticFrozenCounterVariation variation)
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

StaticWrite<BinaryOutputStatusSpec>::func_t StaticWriters::Get(StaticBinaryOutputStatusVariation variation)
{
    switch (variation)
    {
    case (StaticBinaryOutputStatusVariation::Group10Var2):
        return &WriteWithSerializer<BinaryOutputStatusSpec, Group10Var2>;
    default:
        return &WriteWithSerializer<BinaryOutputStatusSpec, Group10Var2>;
    }
}

StaticWrite<AnalogOutputStatusSpec>::func_t StaticWriters::Get(StaticAnalogOutputStatusVariation variation)
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

StaticWrite<OctetStringSpec>::func_t StaticWriters::Get(StaticOctetStringVariation /*variation*/)
{
    // variation is always the same
    return &Write;
}

StaticWrite<TimeAndIntervalSpec>::func_t StaticWriters::Get(StaticTimeAndIntervalVariation /*variation*/)
{
    return &WriteWithSerializer<TimeAndIntervalSpec, Group50Var4>;
}

template<class Iterator>
uint16_t WriteSomeOctetString(openpal::ArrayView<Cell<OctetStringSpec>, uint16_t>& view,
                              Iterator& iterator,
                              Range& range,
                              uint8_t size)
{
    const Cell<OctetStringSpec>& start = view[range.start];
    uint16_t nextIndex = start.config.vIndex;

    uint16_t num_written = 0;

    while (range.IsValid() && view[range.start].selection.value.Size() == size && view[range.start].selection.selected
           && (view[range.start].selection.variation == start.selection.variation)
           && (view[range.start].config.vIndex == nextIndex))
    {
        if (iterator.Write(view[range.start].selection.value))
        {
            // deselect the value and advance the range
            view[range.start].selection.selected = false;
            ++num_written;
            ++nextIndex;
            range.Advance();
        }
        else
        {
            return false;
        }
    }

    return num_written;
}

bool StaticWriters::Write(openpal::ArrayView<Cell<OctetStringSpec>, uint16_t>& view, HeaderWriter& writer, Range& range)
{
    auto start = view[range.start].config.vIndex;
    auto stop = view[range.stop].config.vIndex;
    auto mapped = Range::From(start, stop);

    if (mapped.IsValid())
    {
        const uint8_t sizeStartingSize = view[range.start].selection.value.Size();
        const OctetStringSerializer serializer(false, sizeStartingSize);

        if (mapped.IsOneByte())
        {
            auto iter = writer.IterateOverRange<UInt8>(QualifierCode::UINT8_START_STOP, serializer,
                                                       static_cast<uint8_t>(mapped.start));
            const uint16_t num_written = WriteSomeOctetString(view, iter, range, sizeStartingSize);
            if (num_written == 0)
                return false;
        }
        else
        {
            auto iter = writer.IterateOverRange<UInt16>(QualifierCode::UINT16_START_STOP, serializer, mapped.start);
            const uint16_t num_written = WriteSomeOctetString(view, iter, range, sizeStartingSize);
            if (num_written == 0)
                return false;
        }
    }

    return true;
}

} // namespace opendnp3
