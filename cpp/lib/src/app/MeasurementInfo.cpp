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

#include <opendnp3/app/MeasurementInfo.h>

namespace opendnp3
{

const EventType BinaryInfo::EventTypeEnum = EventType::Binary;
const StaticTypeBitmask BinaryInfo::StaticTypeEnum = StaticTypeBitmask::BinaryInput;
const BinaryInfo::event_variation_t BinaryInfo::DefaultEventVariation = EventBinaryVariation::Group2Var1;
const BinaryInfo::static_variation_t BinaryInfo::DefaultStaticVariation = StaticBinaryVariation::Group1Var2;

const EventType DoubleBitBinaryInfo::EventTypeEnum = EventType::DoubleBitBinary;
const StaticTypeBitmask DoubleBitBinaryInfo::StaticTypeEnum = StaticTypeBitmask::DoubleBinaryInput;
const DoubleBitBinaryInfo::event_variation_t DoubleBitBinaryInfo::DefaultEventVariation
    = EventDoubleBinaryVariation::Group4Var1;
const DoubleBitBinaryInfo::static_variation_t DoubleBitBinaryInfo::DefaultStaticVariation
    = StaticDoubleBinaryVariation::Group3Var2;

const EventType BinaryOutputStatusInfo::EventTypeEnum = EventType::BinaryOutputStatus;
const StaticTypeBitmask BinaryOutputStatusInfo::StaticTypeEnum = StaticTypeBitmask::BinaryOutputStatus;
const BinaryOutputStatusInfo::event_variation_t BinaryOutputStatusInfo::DefaultEventVariation
    = EventBinaryOutputStatusVariation::Group11Var1;
const BinaryOutputStatusInfo::static_variation_t BinaryOutputStatusInfo::DefaultStaticVariation
    = StaticBinaryOutputStatusVariation::Group10Var2;

const EventType AnalogInfo::EventTypeEnum = EventType::Analog;
const StaticTypeBitmask AnalogInfo::StaticTypeEnum = StaticTypeBitmask::AnalogInput;
const AnalogInfo::event_variation_t AnalogInfo::DefaultEventVariation = EventAnalogVariation::Group32Var1;
const AnalogInfo::static_variation_t AnalogInfo::DefaultStaticVariation = StaticAnalogVariation::Group30Var1;

const EventType CounterInfo::EventTypeEnum = EventType::Counter;
const StaticTypeBitmask CounterInfo::StaticTypeEnum = StaticTypeBitmask::Counter;
const CounterInfo::event_variation_t CounterInfo::DefaultEventVariation = EventCounterVariation::Group22Var1;
const CounterInfo::static_variation_t CounterInfo::DefaultStaticVariation = StaticCounterVariation::Group20Var1;

const EventType FrozenCounterInfo::EventTypeEnum = EventType::FrozenCounter;
const StaticTypeBitmask FrozenCounterInfo::StaticTypeEnum = StaticTypeBitmask::FrozenCounter;
const FrozenCounterInfo::event_variation_t FrozenCounterInfo::DefaultEventVariation
    = EventFrozenCounterVariation::Group23Var1;
const FrozenCounterInfo::static_variation_t FrozenCounterInfo::DefaultStaticVariation
    = StaticFrozenCounterVariation::Group21Var1;

const EventType AnalogOutputStatusInfo::EventTypeEnum = EventType::AnalogOutputStatus;
const StaticTypeBitmask AnalogOutputStatusInfo::StaticTypeEnum = StaticTypeBitmask::AnalogOutputStatus;
const AnalogOutputStatusInfo::event_variation_t AnalogOutputStatusInfo::DefaultEventVariation
    = EventAnalogOutputStatusVariation::Group42Var1;
const AnalogOutputStatusInfo::static_variation_t AnalogOutputStatusInfo::DefaultStaticVariation
    = StaticAnalogOutputStatusVariation::Group40Var1;

const EventType OctetStringInfo::EventTypeEnum = EventType::OctetString;
const StaticTypeBitmask OctetStringInfo::StaticTypeEnum = StaticTypeBitmask::OctetString;
const OctetStringInfo::event_variation_t OctetStringInfo::DefaultEventVariation
    = EventOctetStringVariation::Group111Var0;
const OctetStringInfo::static_variation_t OctetStringInfo::DefaultStaticVariation
    = StaticOctetStringVariation::Group110Var0;

const StaticTypeBitmask TimeAndIntervalInfo::StaticTypeEnum = StaticTypeBitmask::TimeAndInterval;
const StaticTimeAndIntervalVariation TimeAndIntervalInfo::DefaultStaticVariation
    = StaticTimeAndIntervalVariation::Group50Var4;

} // namespace opendnp3
