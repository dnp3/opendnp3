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
#include "outstation/StaticBuffers.h"

namespace opendnp3
{

StaticBuffers::StaticBuffers(const DatabaseSizes& dbSizes)
    : binaries(dbSizes.numBinary),
      doubleBinaries(dbSizes.numDoubleBinary),
      analogs(dbSizes.numAnalog),
      counters(dbSizes.numCounter),
      frozenCounters(dbSizes.numFrozenCounter),
      binaryOutputStatii(dbSizes.numBinaryOutputStatus),
      analogOutputStatii(dbSizes.numAnalogOutputStatus),
      timeAndIntervals(dbSizes.numTimeAndInterval),
      octetStrings(dbSizes.numOctetString)
{
    this->SetDefaultIndices<BinarySpec>();
    this->SetDefaultIndices<DoubleBitBinarySpec>();
    this->SetDefaultIndices<CounterSpec>();
    this->SetDefaultIndices<FrozenCounterSpec>();
    this->SetDefaultIndices<AnalogSpec>();
    this->SetDefaultIndices<BinaryOutputStatusSpec>();
    this->SetDefaultIndices<AnalogOutputStatusSpec>();
    this->SetDefaultIndices<TimeAndIntervalSpec>();
    this->SetDefaultIndices<OctetStringSpec>();
}

DatabaseConfigView StaticBuffers::GetView() const
{
    return DatabaseConfigView(binaries.to_view(), doubleBinaries.to_view(), analogs.to_view(), counters.to_view(),
                              frozenCounters.to_view(), binaryOutputStatii.to_view(), analogOutputStatii.to_view(),
                              timeAndIntervals.to_view(), octetStrings.to_view());
}

template<> ser4cpp::ArrayView<Cell<BinarySpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return binaries.to_view();
}

template<> ser4cpp::ArrayView<Cell<DoubleBitBinarySpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return doubleBinaries.to_view();
}

template<> ser4cpp::ArrayView<Cell<CounterSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return counters.to_view();
}

template<> ser4cpp::ArrayView<Cell<FrozenCounterSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return frozenCounters.to_view();
}

template<> ser4cpp::ArrayView<Cell<AnalogSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return analogs.to_view();
}

template<> ser4cpp::ArrayView<Cell<BinaryOutputStatusSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return binaryOutputStatii.to_view();
}

template<> ser4cpp::ArrayView<Cell<AnalogOutputStatusSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return analogOutputStatii.to_view();
}

template<> ser4cpp::ArrayView<Cell<TimeAndIntervalSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return timeAndIntervals.to_view();
}

template<> ser4cpp::ArrayView<Cell<OctetStringSpec>, uint16_t> StaticBuffers::GetArrayView()
{
    return octetStrings.to_view();
}

} // namespace opendnp3
