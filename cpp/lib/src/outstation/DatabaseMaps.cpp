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
#include "DatabaseMaps.h"

namespace opendnp3
{

DatabaseMaps::DatabaseMaps(const DatabaseConfigNew& config,
                                 StaticTypeBitField allowed_class_zero_types)
    : maps(config), allowed_class_zero_types(allowed_class_zero_types)
{
}

void DatabaseMaps::Unselect()
{
    this->Deselect<BinarySpec>();
    this->Deselect<DoubleBitBinarySpec>();
    this->Deselect<CounterSpec>();
    this->Deselect<FrozenCounterSpec>();
    this->Deselect<AnalogSpec>();
    this->Deselect<BinaryOutputStatusSpec>();
    this->Deselect<AnalogOutputStatusSpec>();
    this->Deselect<TimeAndIntervalSpec>();
}

IINField DatabaseMaps::SelectAll(GroupVariation gv)
{
    if (gv == GroupVariation::Group60Var1)
    {
        this->SelectAllClass0<BinarySpec>();
        this->SelectAllClass0<DoubleBitBinarySpec>();
        this->SelectAllClass0<CounterSpec>();
        this->SelectAllClass0<FrozenCounterSpec>();
        this->SelectAllClass0<AnalogSpec>();
        this->SelectAllClass0<BinaryOutputStatusSpec>();
        this->SelectAllClass0<AnalogOutputStatusSpec>();
        this->SelectAllClass0<OctetStringSpec>();
        this->SelectAllClass0<TimeAndIntervalSpec>();

        return IINField::Empty();
    }

    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return this->SelectAll<BinarySpec>();
    case (GroupVariation::Group1Var1):
        return this->SelectAllUsing<BinarySpec>(StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return this->SelectAllUsing<BinarySpec>(StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return this->SelectAll<DoubleBitBinarySpec>();
    case (GroupVariation::Group3Var2):
        return this->SelectAllUsing<DoubleBitBinarySpec>(StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return this->SelectAll<BinaryOutputStatusSpec>();
    case (GroupVariation::Group10Var2):
        return this->SelectAllUsing<BinaryOutputStatusSpec>(StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return this->SelectAll<CounterSpec>();
    case (GroupVariation::Group20Var1):
        return this->SelectAllUsing<CounterSpec>(StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return this->SelectAllUsing<CounterSpec>(StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return this->SelectAllUsing<CounterSpec>(StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return this->SelectAllUsing<CounterSpec>(StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return this->SelectAll<FrozenCounterSpec>();
    case (GroupVariation::Group21Var1):
        return this->SelectAllUsing<FrozenCounterSpec>(StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return this->SelectAllUsing<FrozenCounterSpec>(StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return this->SelectAllUsing<FrozenCounterSpec>(StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return this->SelectAllUsing<FrozenCounterSpec>(StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return this->SelectAllUsing<FrozenCounterSpec>(StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return this->SelectAllUsing<FrozenCounterSpec>(StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return this->SelectAll<AnalogSpec>();
    case (GroupVariation::Group30Var1):
        return this->SelectAllUsing<AnalogSpec>(StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return this->SelectAllUsing<AnalogSpec>(StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return this->SelectAllUsing<AnalogSpec>(StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return this->SelectAllUsing<AnalogSpec>(StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return this->SelectAllUsing<AnalogSpec>(StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return this->SelectAllUsing<AnalogSpec>(StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return this->SelectAll<AnalogOutputStatusSpec>();
    case (GroupVariation::Group40Var1):
        return this->SelectAllUsing<AnalogOutputStatusSpec>(StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return this->SelectAllUsing<AnalogOutputStatusSpec>(StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return this->SelectAllUsing<AnalogOutputStatusSpec>(StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return this->SelectAllUsing<AnalogOutputStatusSpec>(StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return this->SelectAllUsing<TimeAndIntervalSpec>(StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return this->SelectAll<OctetStringSpec>();

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

IINField DatabaseMaps::SelectRange(GroupVariation gv, const Range& range)
{
    switch (gv)
    {
    case (GroupVariation::Group1Var0):
        return this->SelectRange<BinarySpec>(range);
    case (GroupVariation::Group1Var1):
        return this->SelectRangeUsing<BinarySpec>(range, StaticBinaryVariation::Group1Var1);
    case (GroupVariation::Group1Var2):
        return this->SelectRangeUsing<BinarySpec>(range, StaticBinaryVariation::Group1Var2);

    case (GroupVariation::Group3Var0):
        return this->SelectRange<DoubleBitBinarySpec>(range);
    case (GroupVariation::Group3Var2):
        return this->SelectRangeUsing<DoubleBitBinarySpec>(range, StaticDoubleBinaryVariation::Group3Var2);

    case (GroupVariation::Group10Var0):
        return this->SelectRange<BinaryOutputStatusSpec>(range);
    case (GroupVariation::Group10Var2):
        return this->SelectRangeUsing<BinaryOutputStatusSpec>(range, StaticBinaryOutputStatusVariation::Group10Var2);

    case (GroupVariation::Group20Var0):
        return this->SelectRange<CounterSpec>(range);
    case (GroupVariation::Group20Var1):
        return this->SelectRangeUsing<CounterSpec>(range, StaticCounterVariation::Group20Var1);
    case (GroupVariation::Group20Var2):
        return this->SelectRangeUsing<CounterSpec>(range, StaticCounterVariation::Group20Var2);
    case (GroupVariation::Group20Var5):
        return this->SelectRangeUsing<CounterSpec>(range, StaticCounterVariation::Group20Var5);
    case (GroupVariation::Group20Var6):
        return this->SelectRangeUsing<CounterSpec>(range, StaticCounterVariation::Group20Var6);

    case (GroupVariation::Group21Var0):
        return this->SelectRange<FrozenCounterSpec>(range);
    case (GroupVariation::Group21Var1):
        return this->SelectRangeUsing<FrozenCounterSpec>(range, StaticFrozenCounterVariation::Group21Var1);
    case (GroupVariation::Group21Var2):
        return this->SelectRangeUsing<FrozenCounterSpec>(range, StaticFrozenCounterVariation::Group21Var2);
    case (GroupVariation::Group21Var5):
        return this->SelectRangeUsing<FrozenCounterSpec>(range, StaticFrozenCounterVariation::Group21Var5);
    case (GroupVariation::Group21Var6):
        return this->SelectRangeUsing<FrozenCounterSpec>(range, StaticFrozenCounterVariation::Group21Var6);
    case (GroupVariation::Group21Var9):
        return this->SelectRangeUsing<FrozenCounterSpec>(range, StaticFrozenCounterVariation::Group21Var9);
    case (GroupVariation::Group21Var10):
        return this->SelectRangeUsing<FrozenCounterSpec>(range, StaticFrozenCounterVariation::Group21Var10);

    case (GroupVariation::Group30Var0):
        return this->SelectRange<AnalogSpec>(range);
    case (GroupVariation::Group30Var1):
        return this->SelectRangeUsing<AnalogSpec>(range, StaticAnalogVariation::Group30Var1);
    case (GroupVariation::Group30Var2):
        return this->SelectRangeUsing<AnalogSpec>(range, StaticAnalogVariation::Group30Var2);
    case (GroupVariation::Group30Var3):
        return this->SelectRangeUsing<AnalogSpec>(range, StaticAnalogVariation::Group30Var3);
    case (GroupVariation::Group30Var4):
        return this->SelectRangeUsing<AnalogSpec>(range, StaticAnalogVariation::Group30Var4);
    case (GroupVariation::Group30Var5):
        return this->SelectRangeUsing<AnalogSpec>(range, StaticAnalogVariation::Group30Var5);
    case (GroupVariation::Group30Var6):
        return this->SelectRangeUsing<AnalogSpec>(range, StaticAnalogVariation::Group30Var6);

    case (GroupVariation::Group40Var0):
        return this->SelectRange<AnalogOutputStatusSpec>(range);
    case (GroupVariation::Group40Var1):
        return this->SelectRangeUsing<AnalogOutputStatusSpec>(range, StaticAnalogOutputStatusVariation::Group40Var1);
    case (GroupVariation::Group40Var2):
        return this->SelectRangeUsing<AnalogOutputStatusSpec>(range, StaticAnalogOutputStatusVariation::Group40Var2);
    case (GroupVariation::Group40Var3):
        return this->SelectRangeUsing<AnalogOutputStatusSpec>(range, StaticAnalogOutputStatusVariation::Group40Var3);
    case (GroupVariation::Group40Var4):
        return this->SelectRangeUsing<AnalogOutputStatusSpec>(range, StaticAnalogOutputStatusVariation::Group40Var4);

    case (GroupVariation::Group50Var4):
        return this->SelectRangeUsing<TimeAndIntervalSpec>(range, StaticTimeAndIntervalVariation::Group50Var4);

    case (GroupVariation::Group110Var0):
        return this->SelectRangeUsing<OctetStringSpec>(range, StaticOctetStringVariation::Group110Var0);

    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

bool DatabaseMaps::Load(HeaderWriter& writer)
{
    typedef bool (DatabaseMaps::*LoadFun)(HeaderWriter & writer);

    const int NUM_TYPE = 9;

    LoadFun functions[NUM_TYPE] = {&DatabaseMaps::LoadType<BinarySpec>,
                                   &DatabaseMaps::LoadType<DoubleBitBinarySpec>,
                                   &DatabaseMaps::LoadType<CounterSpec>,
                                   &DatabaseMaps::LoadType<FrozenCounterSpec>,
                                   &DatabaseMaps::LoadType<AnalogSpec>,
                                   &DatabaseMaps::LoadType<BinaryOutputStatusSpec>,
                                   &DatabaseMaps::LoadType<AnalogOutputStatusSpec>,
                                   &DatabaseMaps::LoadType<TimeAndIntervalSpec>,
                                   &DatabaseMaps::LoadType<OctetStringSpec>};

    for (auto& function : functions)
    {
        if (!(this->*function)(writer))
        {
            // return early because the APDU is full
            return false;
        }
    }

    return true;
}

Range DatabaseMaps::AssignClassToAll(AssignClassType type, PointClass clazz)
{
    switch (type)
    {
    /* TODO
    case (AssignClassType::BinaryInput):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<BinarySpec>().length()));
    case (AssignClassType::DoubleBinaryInput):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<DoubleBitBinarySpec>().length()));
    case (AssignClassType::Counter):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<CounterSpec>().length()));
    case (AssignClassType::FrozenCounter):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<FrozenCounterSpec>().length()));
    case (AssignClassType::AnalogInput):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<AnalogSpec>().length()));
    case (AssignClassType::BinaryOutputStatus):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<BinaryOutputStatusSpec>().length()));
    case (AssignClassType::AnalogOutputStatus):
        return AssignClassToRange(type, clazz, RangeOf(buffers.GetArrayView<AnalogOutputStatusSpec>().length()));
        */
    default:
        return Range::Invalid();
    }
}

Range DatabaseMaps::AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range)
{
    switch (type)
    {
    case (AssignClassType::BinaryInput):
        return AssignClassTo<BinarySpec>(clazz, range);
    case (AssignClassType::DoubleBinaryInput):
        return AssignClassTo<DoubleBitBinarySpec>(clazz, range);
    case (AssignClassType::Counter):
        return AssignClassTo<CounterSpec>(clazz, range);
    case (AssignClassType::FrozenCounter):
        return AssignClassTo<FrozenCounterSpec>(clazz, range);
    case (AssignClassType::AnalogInput):
        return AssignClassTo<AnalogSpec>(clazz, range);
    case (AssignClassType::BinaryOutputStatus):
        return AssignClassTo<BinaryOutputStatusSpec>(clazz, range);
    case (AssignClassType::AnalogOutputStatus):
        return AssignClassTo<AnalogOutputStatusSpec>(clazz, range);
    default:
        return Range::Invalid();
    }
}

template<>
StaticBinaryVariation DatabaseMaps::CheckForPromotion<BinarySpec>(const Binary& value,
                                                                     StaticBinaryVariation variation)
{
    if (variation == StaticBinaryVariation::Group1Var1)
    {
        return BinarySpec::IsQualityOnlineOnly(value) ? variation : StaticBinaryVariation::Group1Var2;
    }

    return variation;
}

Range DatabaseMaps::RangeOf(uint16_t size)
{
    return size > 0 ? Range::From(0, size - 1) : Range::Invalid();
}

} // namespace opendnp3
