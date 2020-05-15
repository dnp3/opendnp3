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
#ifndef OPENDNP3_MEASUREMENTINFO_H
#define OPENDNP3_MEASUREMENTINFO_H

#include "opendnp3/app/EventType.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/gen/BinaryQuality.h"
#include "opendnp3/gen/EventAnalogOutputStatusVariation.h"
#include "opendnp3/gen/EventAnalogVariation.h"
#include "opendnp3/gen/EventBinaryOutputStatusVariation.h"
#include "opendnp3/gen/EventBinaryVariation.h"
#include "opendnp3/gen/EventCounterVariation.h"
#include "opendnp3/gen/EventDoubleBinaryVariation.h"
#include "opendnp3/gen/EventFrozenCounterVariation.h"
#include "opendnp3/gen/EventOctetStringVariation.h"
#include "opendnp3/gen/EventSecurityStatVariation.h"
#include "opendnp3/gen/StaticAnalogOutputStatusVariation.h"
#include "opendnp3/gen/StaticAnalogVariation.h"
#include "opendnp3/gen/StaticBinaryOutputStatusVariation.h"
#include "opendnp3/gen/StaticBinaryVariation.h"
#include "opendnp3/gen/StaticCounterVariation.h"
#include "opendnp3/gen/StaticDoubleBinaryVariation.h"
#include "opendnp3/gen/StaticFrozenCounterVariation.h"
#include "opendnp3/gen/StaticOctetStringVariation.h"
#include "opendnp3/gen/StaticSecurityStatVariation.h"
#include "opendnp3/gen/StaticTimeAndIntervalVariation.h"
#include "opendnp3/gen/StaticTypeBitmask.h"
#include "opendnp3/util/StaticOnly.h"

namespace opendnp3
{

struct BinaryInfo : private StaticOnly
{
    typedef Binary meas_t;
    typedef bool value_t;
    typedef EventBinaryVariation event_variation_t;
    typedef StaticBinaryVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct DoubleBitBinaryInfo : private StaticOnly
{
    typedef DoubleBitBinary meas_t;
    typedef DoubleBit value_t;
    typedef EventDoubleBinaryVariation event_variation_t;
    typedef StaticDoubleBinaryVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct BinaryOutputStatusInfo : private StaticOnly
{
    typedef BinaryOutputStatus meas_t;
    typedef bool value_t;
    typedef EventBinaryOutputStatusVariation event_variation_t;
    typedef StaticBinaryOutputStatusVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct AnalogInfo : private StaticOnly
{
    typedef Analog meas_t;
    typedef double value_t;
    typedef EventAnalogVariation event_variation_t;
    typedef StaticAnalogVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct CounterInfo : private StaticOnly
{
    typedef Counter meas_t;
    typedef uint32_t value_t;
    typedef EventCounterVariation event_variation_t;
    typedef StaticCounterVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct FrozenCounterInfo : private StaticOnly
{
    typedef FrozenCounter meas_t;
    typedef uint32_t value_t;
    typedef EventFrozenCounterVariation event_variation_t;
    typedef StaticFrozenCounterVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct AnalogOutputStatusInfo : private StaticOnly
{
    typedef AnalogOutputStatus meas_t;
    typedef double value_t;
    typedef EventAnalogOutputStatusVariation event_variation_t;
    typedef StaticAnalogOutputStatusVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct OctetStringInfo : private StaticOnly
{
    typedef OctetString meas_t;
    typedef EventOctetStringVariation event_variation_t;
    typedef StaticOctetStringVariation static_variation_t;

    static const EventType EventTypeEnum;
    static const StaticTypeBitmask StaticTypeEnum;
    static const event_variation_t DefaultEventVariation;
    static const static_variation_t DefaultStaticVariation;
};

struct TimeAndIntervalInfo : private StaticOnly
{
    typedef TimeAndInterval meas_t;
    typedef StaticTimeAndIntervalVariation static_variation_t;

    static const StaticTypeBitmask StaticTypeEnum;
    static const StaticTimeAndIntervalVariation DefaultStaticVariation;
};

} // namespace opendnp3

#endif
