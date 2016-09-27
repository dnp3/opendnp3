/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the Infoific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#ifndef OPENDNP3_MEASUREMENTCONFIG_H
#define OPENDNP3_MEASUREMENTCONFIG_H

#include "opendnp3/app/MeasurementInfo.h"
#include "opendnp3/gen/PointClass.h"

namespace opendnp3
{

// All entries have this information
template <class Info>
struct StaticConfig
{
	// virtual index for discontiguous data, as opposed to the raw array index
	uint16_t vIndex = 0; 
	typename Info::StaticVariation svariation = Info::DefaultStaticVariation;
};

template <class Info>
struct DeadbandConfig
{
	typename Info::ValueType deadband = 0;
};

template <class Info>
struct EventConfig
{
	PointClass clazz;
	typename Info::EventVariation evariation = Info::DefaultEventVariation;
};

class BinaryConfig : public StaticConfig<BinaryInfo>, public EventConfig<BinaryInfo> {};
class DoubleBitBinaryConfig : public StaticConfig<DoubleBitBinaryInfo>, public EventConfig<DoubleBitBinaryInfo> {};
class AnalogConfig : public StaticConfig<AnalogInfo>, public EventConfig<AnalogInfo>, public DeadbandConfig<AnalogInfo> {};
class CounterConfig : public StaticConfig<CounterInfo>, public EventConfig<CounterInfo>, public DeadbandConfig<CounterInfo> {};
class FrozenCounterConfig : public StaticConfig<FrozenCounterInfo>, public EventConfig<FrozenCounterInfo>, public DeadbandConfig<FrozenCounterInfo> {};
class BOStatusConfig : public StaticConfig<BinaryOutputStatusInfo>, public EventConfig<BinaryOutputStatusInfo> {};
class AOStatusConfig : public StaticConfig<AnalogOutputStatusInfo>, public EventConfig<AnalogOutputStatusInfo> {};
class TimeAndIntervalConfig : public StaticConfig<TimeAndIntervalInfo> {};
class SecurityStatConfig {};

}

#endif
