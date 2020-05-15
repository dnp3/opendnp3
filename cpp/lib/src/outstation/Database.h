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
#ifndef OPENDNP3_DATABASE_H
#define OPENDNP3_DATABASE_H

#include "app/MeasurementTypeSpecs.h"
#include "outstation/IClassAssigner.h"
#include "outstation/IEventReceiver.h"
#include "outstation/IResponseLoader.h"
#include "outstation/IStaticSelector.h"
#include "outstation/StaticDataMap.h"

#include "opendnp3/gen/FlagsType.h"
#include "opendnp3/outstation/DatabaseConfig.h"
#include "opendnp3/outstation/IDnpTimeSource.h"
#include "opendnp3/outstation/IUpdateHandler.h"
#include "opendnp3/outstation/StaticTypeBitfield.h"

namespace opendnp3
{

class Database final : public IStaticSelector, public IClassAssigner, public IResponseLoader, public IUpdateHandler
{
public:
    Database(const DatabaseConfig& config,
             IEventReceiver& event_receiver,
             IDnpTimeSource& time_source,
             StaticTypeBitField allowed_class_zero_types);

    // ------- IStaticSelector -------------
    IINField SelectAll(GroupVariation gv) override;
    IINField SelectRange(GroupVariation gv, const Range& range) override;
    IINField SelectIndices(GroupVariation gv, const ICollection<uint16_t>& indices) override;
    void Unselect() override;

    // ------- IClassAssigner -------------
    Range AssignClassToAll(AssignClassType type, PointClass clazz) override;
    Range AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range) override;

    // ------- IResponseLoader -------------
    bool HasAnySelection() const override;
    bool Load(HeaderWriter& writer) override;

    // ------- IUpdateHandler ---------------
    bool Update(const Binary& meas, uint16_t index, EventMode mode) override;
    bool Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode) override;
    bool Update(const Analog& meas, uint16_t index, EventMode mode) override;
    bool Update(const Counter& meas, uint16_t index, EventMode mode) override;
    bool FreezeCounter(uint16_t index, bool clear, EventMode mode) override;
    bool Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode) override;
    bool Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode) override;
    bool Update(const OctetString& meas, uint16_t index, EventMode mode) override;
    bool Update(const TimeAndInterval& meas, uint16_t index) override;
    bool Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags) override;

    bool FreezeSelectedCounters(bool clear, EventMode mode = EventMode::Detect);

private:
    IEventReceiver& event_receiver;
    IDnpTimeSource& time_source;
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

    template<class Spec> void select_all_class_zero(StaticDataMap<Spec>& map);

    template<class Spec> static IINField select_all(StaticDataMap<Spec>& map);

    template<class Spec>
    IINField static select_all(StaticDataMap<Spec>& map, typename Spec::static_variation_t variation);

    template<class Spec> static IINField select_range(StaticDataMap<Spec>& map, const Range& range);

    template<class Spec>
    static IINField select_range(StaticDataMap<Spec>& map,
                                 const Range& range,
                                 typename Spec::static_variation_t variation);
};

} // namespace opendnp3

#endif
