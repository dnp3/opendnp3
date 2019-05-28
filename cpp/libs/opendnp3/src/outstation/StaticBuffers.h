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
#ifndef OPENDNP3_STATICBUFFERS_H
#define OPENDNP3_STATICBUFFERS_H

#include <ser4cpp/container/Array.h>
#include <ser4cpp/util/Uncopyable.h>

#include "opendnp3/outstation/Cell.h"
#include "outstation/DatabaseConfigView.h"
#include "opendnp3/outstation/DatabaseSizes.h"

namespace opendnp3
{

/**
 * The static database provides storage for current values and all of the associated metadata
 */
class StaticBuffers : private ser4cpp::Uncopyable
{

public:
    explicit StaticBuffers(const DatabaseSizes& dbSizes);

    DatabaseConfigView GetView() const;

    // specializations in cpp file
    template<class Spec> ser4cpp::ArrayView<Cell<Spec>, uint16_t> GetArrayView();

private:
    template<class Spec> void SetDefaultIndices()
    {
        auto view = GetArrayView<Spec>();
        for (uint16_t i = 0; i < view.length(); ++i)
        {
            view[i].config.vIndex = i;
        }
    }

    ser4cpp::Array<Cell<BinarySpec>, uint16_t> binaries;
    ser4cpp::Array<Cell<DoubleBitBinarySpec>, uint16_t> doubleBinaries;
    ser4cpp::Array<Cell<AnalogSpec>, uint16_t> analogs;
    ser4cpp::Array<Cell<CounterSpec>, uint16_t> counters;
    ser4cpp::Array<Cell<FrozenCounterSpec>, uint16_t> frozenCounters;
    ser4cpp::Array<Cell<BinaryOutputStatusSpec>, uint16_t> binaryOutputStatii;
    ser4cpp::Array<Cell<AnalogOutputStatusSpec>, uint16_t> analogOutputStatii;
    ser4cpp::Array<Cell<TimeAndIntervalSpec>, uint16_t> timeAndIntervals;
    ser4cpp::Array<Cell<OctetStringSpec>, uint16_t> octetStrings;
};

} // namespace opendnp3

#endif
