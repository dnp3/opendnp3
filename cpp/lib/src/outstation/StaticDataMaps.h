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

#include <opendnp3/outstation/DatabaseConfigNew.h>
#include <opendnp3/app/MeasurementTypeSpecs.h>
#include <opendnp3/outstation/StaticTypeBitfield.h>

#include "outstation/StaticDataMap.h"
#include "outstation/IStaticSelector.h"
#include "outstation/IClassAssigner.h"
#include "outstation/IResponseLoader.h"

namespace opendnp3
{

class StaticDataMaps final : public IStaticSelector, public IClassAssigner, public IResponseLoader
{
public:

	StaticDataMaps(const DatabaseConfigNew& config, StaticTypeBitField allowed_class_zero_types);

    // ------- IStaticSelector -------------
    IINField SelectAll(GroupVariation gv) override;
    IINField SelectRange(GroupVariation gv, const Range& range) override;
    void Unselect() override;

	// ------- IClassAssigner -------------
    Range AssignClassToAll(AssignClassType type, PointClass clazz) override;
    Range AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range) override;

	// ------- IResponseLoader -------------
    bool HasAnySelection() const override;
    bool Load(HeaderWriter& writer) override;

	// ------- update methods ---------------
    bool Update(const Binary& meas, uint16_t index);
    bool Update(const DoubleBitBinary& meas, uint16_t index);
    bool Update(const Analog& meas, uint16_t index);
    bool Update(const Counter& meas, uint16_t index);
    bool Update(const FrozenCounter& meas, uint16_t index);
    bool Update(const BinaryOutputStatus& meas, uint16_t index);
    bool Update(const AnalogOutputStatus& meas, uint16_t index);
    bool Update(const OctetString& meas, uint16_t index);
    bool Update(const TimeAndInterval& meas, uint16_t index);

private:

	StaticTypeBitField allowed_class_zero_types;
    
	StaticDataMap<BinarySpec> binary_input;
    StaticDataMap<DoubleBitBinarySpec> double_binary;
    StaticDataMap<AnalogSpec> analog_input;
    StaticDataMap<CounterSpec> counter;
    StaticDataMap<FrozenCounterSpec> frozen_counter;
    StaticDataMap<BinaryOutputStatusSpec> binary_output_status;
    StaticDataMap<AnalogOutputStatusSpec> analog_output_status;
    StaticDataMap<TimeAndIntervalSpec> time_and_interval;
    StaticDataMap<OctetStringSpec> octet_string;

	// ----- helper methods ------ 

	template<class Spec>
	void select_all_class_zero(StaticDataMap<Spec>& map);

	template<class Spec> 
	static IINField select_all(StaticDataMap<Spec>& map);

	template<class Spec> IINField 
	static select_all(StaticDataMap<Spec>& map, typename Spec::static_variation_t variation);

	template<class Spec>
    static IINField select_range(StaticDataMap<Spec>& map, const Range& range);

	template<class Spec>
    static IINField select_range(StaticDataMap<Spec>& map,
                                const Range& range,
                                typename Spec::static_variation_t variation);

	template<class Spec>
	static Range assign_class_to_all(StaticDataMap<Spec>& map, PointClass clazz);

	template<class Spec>
    static Range assign_class_to_range(StaticDataMap<Spec>& map, PointClass clazz, const Range& range);
	
};
    

} // namespace opendnp3

#endif
