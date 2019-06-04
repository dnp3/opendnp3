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
#ifndef OPENDNP3_STATICDATAMAPS_H
#define OPENDNP3_STATICDATAMAPS_H

#include "outstation/StaticDataMap.h"
#include "outstation/IStaticSelector.h"

#include <opendnp3/outstation/DatabaseConfigNew.h>
#include <opendnp3/app/MeasurementTypeSpecs.h>

namespace opendnp3
{

class StaticDataMaps final : public IStaticSelector
{
public:

	StaticDataMaps(const DatabaseConfigNew& config);

	bool has_any_selection() const;

    // ------- IStaticSelector -------------

    IINField SelectAll(GroupVariation gv) override;
    IINField SelectRange(GroupVariation gv, const Range& range) override;    
    void Unselect() override;

private:

	template<class Spec> 
	static IINField SelectAll(StaticDataMap<Spec>& map);

	template<class Spec> IINField 
	static SelectAll(StaticDataMap<Spec>& map, typename Spec::static_variation_t variation);

	template<class Spec>
    static IINField SelectRange(StaticDataMap<Spec>& map, const Range& range);

	template<class Spec>
    static IINField SelectRange(StaticDataMap<Spec>& map,
                                const Range& range,
                                typename Spec::static_variation_t variation);

    StaticDataMap<BinarySpec> binary_input;
    StaticDataMap<DoubleBitBinarySpec> double_binary;
    StaticDataMap<AnalogSpec> analog_input;
    StaticDataMap<CounterSpec> counter;
    StaticDataMap<FrozenCounterSpec> frozen_counter;
    StaticDataMap<BinaryOutputStatusSpec> binary_output_status;
    StaticDataMap<AnalogOutputStatusSpec> analog_output_status;
    StaticDataMap<TimeAndIntervalSpec> time_and_interval;
    StaticDataMap<OctetStringSpec> octet_string;
};

} // namespace opendnp3

#endif
