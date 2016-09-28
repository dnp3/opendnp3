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
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef OPENDNP3_MEASUREMENTINFO_H
#define OPENDNP3_MEASUREMENTINFO_H

#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/SecurityStat.h"

#include "opendnp3/app/EventType.h"
#include "opendnp3/app/EventTriggers.h"

#include "opendnp3/gen/StaticBinaryVariation.h"
#include "opendnp3/gen/StaticDoubleBinaryVariation.h"
#include "opendnp3/gen/StaticBinaryOutputStatusVariation.h"
#include "opendnp3/gen/StaticCounterVariation.h"
#include "opendnp3/gen/StaticFrozenCounterVariation.h"
#include "opendnp3/gen/StaticAnalogVariation.h"
#include "opendnp3/gen/StaticAnalogOutputStatusVariation.h"
#include "opendnp3/gen/StaticTimeAndIntervalVariation.h"
#include "opendnp3/gen/StaticSecurityStatVariation.h"

#include "opendnp3/gen/EventBinaryVariation.h"
#include "opendnp3/gen/EventDoubleBinaryVariation.h"
#include "opendnp3/gen/EventBinaryOutputStatusVariation.h"
#include "opendnp3/gen/EventCounterVariation.h"
#include "opendnp3/gen/EventFrozenCounterVariation.h"
#include "opendnp3/gen/EventAnalogVariation.h"
#include "opendnp3/gen/EventAnalogOutputStatusVariation.h"
#include "opendnp3/gen/EventSecurityStatVariation.h"

#include "opendnp3/gen/StaticTypeBitmask.h"
#include "opendnp3/gen/BinaryQuality.h"
#include "openpal/util/Uncopyable.h"

namespace opendnp3
{


struct BinaryInfo : private openpal::StaticOnly
{
	typedef Binary type_t;
	typedef bool ValueType;
	typedef EventBinaryVariation EventVariation;
	typedef StaticBinaryVariation StaticVariation;

	static const EventType EventTypeEnum = EventType::Binary;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::BinaryInput;
	static const EventBinaryVariation DefaultEventVariation = EventBinaryVariation::Group2Var1;
	static const StaticBinaryVariation DefaultStaticVariation = StaticBinaryVariation::Group1Var2;	
};

struct DoubleBitBinaryInfo : private openpal::StaticOnly
{
	typedef DoubleBitBinary type_t;
	typedef DoubleBit ValueType;
	typedef EventDoubleBinaryVariation EventVariation;
	typedef StaticDoubleBinaryVariation StaticVariation;

	static const EventType EventTypeEnum = EventType::DoubleBitBinary;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::DoubleBinaryInput;
	static const EventDoubleBinaryVariation DefaultEventVariation = EventDoubleBinaryVariation::Group4Var1;
	static const StaticDoubleBinaryVariation DefaultStaticVariation = StaticDoubleBinaryVariation::Group3Var2;		
};

class BinaryOutputStatusInfo : private openpal::StaticOnly
{
public:

	typedef BinaryOutputStatus type_t;

	static const EventType EventTypeEnum = EventType::BinaryOutputStatus;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::BinaryOutputStatus;
	static const EventBinaryOutputStatusVariation DefaultEventVariation = EventBinaryOutputStatusVariation::Group11Var1;
	static const StaticBinaryOutputStatusVariation DefaultStaticVariation = StaticBinaryOutputStatusVariation::Group10Var2;

	typedef bool ValueType;
	typedef EventBinaryOutputStatusVariation EventVariation;
	typedef StaticBinaryOutputStatusVariation StaticVariation;		
};


struct AnalogInfo : private openpal::StaticOnly
{
	typedef Analog type_t;
	typedef double ValueType;
	typedef EventAnalogVariation EventVariation;
	typedef StaticAnalogVariation StaticVariation;

	static const EventType EventTypeEnum = EventType::Analog;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::AnalogInput;
	static const EventAnalogVariation DefaultEventVariation = EventAnalogVariation::Group32Var1;
	static const StaticAnalogVariation DefaultStaticVariation = StaticAnalogVariation::Group30Var1;	
};

struct CounterInfo : private openpal::StaticOnly
{
	typedef Counter type_t;
	typedef uint32_t ValueType;
	typedef EventCounterVariation EventVariation;
	typedef StaticCounterVariation StaticVariation;

	static const EventType EventTypeEnum = EventType::Counter;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::Counter;
	static const EventCounterVariation DefaultEventVariation = EventCounterVariation::Group22Var1;
	static const StaticCounterVariation DefaultStaticVariation = StaticCounterVariation::Group20Var1;		
};

struct FrozenCounterInfo : private openpal::StaticOnly
{
	typedef FrozenCounter type_t;
	typedef uint32_t ValueType;
	typedef EventFrozenCounterVariation EventVariation;
	typedef StaticFrozenCounterVariation StaticVariation;

	static const EventType EventTypeEnum = EventType::FrozenCounter;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::FrozenCounter;
	static const EventVariation DefaultEventVariation = EventFrozenCounterVariation::Group23Var1;
	static const StaticVariation DefaultStaticVariation = StaticFrozenCounterVariation::Group21Var1;
};

struct AnalogOutputStatusInfo : private openpal::StaticOnly
{
	typedef AnalogOutputStatus type_t;
	typedef double ValueType;
	typedef EventAnalogOutputStatusVariation EventVariation;
	typedef StaticAnalogOutputStatusVariation StaticVariation;

	static const EventType EventTypeEnum = EventType::AnalogOutputStatus;
	static const StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::AnalogOutputStatus;
	static const EventAnalogOutputStatusVariation DefaultEventVariation = EventAnalogOutputStatusVariation::Group42Var1;
	static const StaticAnalogOutputStatusVariation DefaultStaticVariation = StaticAnalogOutputStatusVariation::Group40Var1;
};

struct TimeAndIntervalInfo : private openpal::StaticOnly
{
	typedef TimeAndInterval type_t;
	typedef StaticTimeAndIntervalVariation StaticVariation;

	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::TimeAndInterval;
	const static StaticTimeAndIntervalVariation DefaultStaticVariation = StaticTimeAndIntervalVariation::Group50Var4;		
};

struct SecurityStatInfo : private openpal::StaticOnly
{
	typedef SecurityStat type_t;
	typedef SecurityStat::Value ValueType;
	typedef EventSecurityStatVariation EventVariation;
	typedef StaticSecurityStatVariation StaticVariation;

	const static EventType EventTypeEnum = EventType::SecurityStat;
	const static EventSecurityStatVariation DefaultEventVariation = EventSecurityStatVariation::Group122Var1;
	const static StaticSecurityStatVariation DefaultStaticVariation = StaticSecurityStatVariation::Group121Var1;		
};

}

#endif
