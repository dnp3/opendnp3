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
#ifndef OPENDNP3_STATICWRITERS_H
#define OPENDNP3_STATICWRITERS_H

#include <openpal/container/ArrayView.h>
#include <openpal/util/Uncopyable.h>

#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/app/MeasurementTypeSpecs.h"
#include "opendnp3/app/Range.h"
#include "opendnp3/gen/StaticAnalogOutputStatusVariation.h"
#include "opendnp3/gen/StaticAnalogVariation.h"
#include "opendnp3/gen/StaticBinaryOutputStatusVariation.h"
#include "opendnp3/gen/StaticBinaryVariation.h"
#include "opendnp3/gen/StaticCounterVariation.h"
#include "opendnp3/gen/StaticDoubleBinaryVariation.h"
#include "opendnp3/gen/StaticFrozenCounterVariation.h"
#include "opendnp3/outstation/Cell.h"

namespace opendnp3
{

template<class Spec> struct StaticWrite : private openpal::StaticOnly
{
    typedef bool (*func_t)(openpal::ArrayView<Cell<Spec>, uint16_t>& view, HeaderWriter& writer, Range& range);
};

class StaticWriters : private openpal::StaticOnly
{

public:
    static StaticWrite<BinarySpec>::func_t Get(StaticBinaryVariation variation);
    static StaticWrite<DoubleBitBinarySpec>::func_t Get(StaticDoubleBinaryVariation variation);
    static StaticWrite<CounterSpec>::func_t Get(StaticCounterVariation variation);
    static StaticWrite<FrozenCounterSpec>::func_t Get(StaticFrozenCounterVariation variation);
    static StaticWrite<AnalogSpec>::func_t Get(StaticAnalogVariation variation);
    static StaticWrite<AnalogOutputStatusSpec>::func_t Get(StaticAnalogOutputStatusVariation variation);
    static StaticWrite<BinaryOutputStatusSpec>::func_t Get(StaticBinaryOutputStatusVariation variation);
    static StaticWrite<OctetStringSpec>::func_t Get(StaticOctetStringVariation variation);
    static StaticWrite<TimeAndIntervalSpec>::func_t Get(StaticTimeAndIntervalVariation variation);

private:
    static bool Write(openpal::ArrayView<Cell<OctetStringSpec>, uint16_t>& view, HeaderWriter& writer, Range& range);
};

} // namespace opendnp3

#endif
