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
#ifndef OPENDNP3_STATICWRITERS_H
#define OPENDNP3_STATICWRITERS_H

#include "app/HeaderWriter.h"
#include "app/MeasurementTypeSpecs.h"
#include "outstation/StaticDataMap.h"

#include "opendnp3/gen/StaticAnalogOutputStatusVariation.h"
#include "opendnp3/gen/StaticAnalogVariation.h"
#include "opendnp3/gen/StaticBinaryOutputStatusVariation.h"
#include "opendnp3/gen/StaticBinaryVariation.h"
#include "opendnp3/gen/StaticCounterVariation.h"
#include "opendnp3/gen/StaticDoubleBinaryVariation.h"
#include "opendnp3/gen/StaticFrozenCounterVariation.h"
#include "opendnp3/util/StaticOnly.h"

namespace opendnp3
{

template<class Spec> using static_write_func_t = bool (*)(StaticDataMap<Spec>& map, HeaderWriter& writer);

struct StaticWriters : private StaticOnly
{
    static static_write_func_t<BinarySpec> get(StaticBinaryVariation variation);
    static static_write_func_t<DoubleBitBinarySpec> get(StaticDoubleBinaryVariation variation);
    static static_write_func_t<CounterSpec> get(StaticCounterVariation variation);
    static static_write_func_t<FrozenCounterSpec> get(StaticFrozenCounterVariation variation);
    static static_write_func_t<AnalogSpec> get(StaticAnalogVariation variation);
    static static_write_func_t<AnalogOutputStatusSpec> get(StaticAnalogOutputStatusVariation variation);
    static static_write_func_t<BinaryOutputStatusSpec> get(StaticBinaryOutputStatusVariation variation);
    static static_write_func_t<OctetStringSpec> get(StaticOctetStringVariation variation);
    static static_write_func_t<TimeAndIntervalSpec> get(StaticTimeAndIntervalVariation variation);
};

} // namespace opendnp3

#endif
