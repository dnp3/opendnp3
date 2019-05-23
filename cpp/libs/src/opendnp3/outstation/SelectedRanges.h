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
#ifndef OPENDNP3_SELECTEDRANGES_H
#define OPENDNP3_SELECTEDRANGES_H

#include <openpal/util/Uncopyable.h>

#include "opendnp3/app/Range.h"

namespace opendnp3
{

/**
 * Selected ranges for each static datatype
 */
class SelectedRanges : private openpal::Uncopyable
{

public:
    template<class Spec> Range Get()
    {
        return GetRangeRef<Spec>();
    }

    template<class Spec> void Set(const Range& range)
    {
        GetRangeRef<Spec>() = range;
    }

    template<class Spec> void Merge(const Range& range)
    {
        auto& ref = GetRangeRef<Spec>();
        ref = ref.Union(range);
    }

    template<class Spec> void Clear()
    {
        Set<Spec>(Range::Invalid());
    }

    bool HasAnySelection() const;

private:
    // specializations in cpp file
    template<class Spec> Range& GetRangeRef();

    Range binaries;
    Range doubleBinaries;
    Range analogs;
    Range counters;
    Range frozenCounters;
    Range binaryOutputStatii;
    Range analogOutputStatii;
    Range octetString;
    Range timeAndIntervals;
    Range securityStats;
};

} // namespace opendnp3

#endif
