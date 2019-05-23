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
#include "SelectedRanges.h"

#include "opendnp3/app/MeasurementTypeSpecs.h"
#include "opendnp3/app/SecurityStat.h"

namespace opendnp3
{

bool SelectedRanges::HasAnySelection() const
{
    return binaries.IsValid() || doubleBinaries.IsValid() || analogs.IsValid() || counters.IsValid()
        || frozenCounters.IsValid() || binaryOutputStatii.IsValid() || analogOutputStatii.IsValid()
        || timeAndIntervals.IsValid() || octetString.IsValid() || securityStats.IsValid();
}

template<> Range& SelectedRanges::GetRangeRef<BinarySpec>()
{
    return binaries;
}

template<> Range& SelectedRanges::GetRangeRef<DoubleBitBinarySpec>()
{
    return doubleBinaries;
}

template<> Range& SelectedRanges::GetRangeRef<AnalogSpec>()
{
    return analogs;
}

template<> Range& SelectedRanges::GetRangeRef<CounterSpec>()
{
    return counters;
}

template<> Range& SelectedRanges::GetRangeRef<FrozenCounterSpec>()
{
    return frozenCounters;
}

template<> Range& SelectedRanges::GetRangeRef<BinaryOutputStatusSpec>()
{
    return binaryOutputStatii;
}

template<> Range& SelectedRanges::GetRangeRef<AnalogOutputStatusSpec>()
{
    return analogOutputStatii;
}

template<> Range& SelectedRanges::GetRangeRef<TimeAndIntervalSpec>()
{
    return timeAndIntervals;
}

template<> Range& SelectedRanges::GetRangeRef<SecurityStatSpec>()
{
    return securityStats;
}

template<> Range& SelectedRanges::GetRangeRef<OctetStringSpec>()
{
    return octetString;
}

} // namespace opendnp3
