/**
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
#ifndef OPENDNP3_MEASUREMENTTYPES_H
#define OPENDNP3_MEASUREMENTTYPES_H

#include "QualityMasks.h"
#include "EventTriggers.h"

#include "opendnp3/app/EventType.h"
#include "opendnp3/app/EventMetadata.h"

#include "opendnp3/gen/DoubleBit.h"

#include "opendnp3/gen/StaticBinaryVariation.h"
#include "opendnp3/gen/StaticDoubleBinaryVariation.h"
#include "opendnp3/gen/StaticBinaryOutputStatusVariation.h"
#include "opendnp3/gen/StaticCounterVariation.h"
#include "opendnp3/gen/StaticFrozenCounterVariation.h"
#include "opendnp3/gen/StaticAnalogVariation.h"
#include "opendnp3/gen/StaticAnalogOutputStatusVariation.h"

#include "opendnp3/gen/StaticTypeBitmask.h"

#include "opendnp3/gen/EventBinaryVariation.h"
#include "opendnp3/gen/EventDoubleBinaryVariation.h"
#include "opendnp3/gen/EventBinaryOutputStatusVariation.h"
#include "opendnp3/gen/EventCounterVariation.h"
#include "opendnp3/gen/EventFrozenCounterVariation.h"
#include "opendnp3/gen/EventAnalogVariation.h"
#include "opendnp3/gen/EventAnalogOutputStatusVariation.h"



namespace opendnp3
{

namespace quality {

	enum Quality : uint8_t
	{
		ONLINE = 0x01,
		RESTART = 0x02
	};

	bool GetBinaryValue(uint8_t quality);
	
	uint8_t GetBinaryQuality(uint8_t q, bool value);
}

/**
	The Binary data type is for describing on-off (boolean) type values. Good examples of
	binaries are alarms, mode settings, enabled/disabled flags etc. Think of it as a status
	LED on a piece of equipment.
*/
class Binary : public TypedMeasurement<bool>
{
public:

	Binary();

	Binary(bool value);

	Binary(uint8_t quality);

	Binary(uint8_t quality, uint64_t aTime);

	Binary(bool value, uint8_t quality);

	Binary(bool value, uint8_t quality, uint64_t aTime);

	bool IsQualityOnlineOnly() const;

	bool IsEvent(const Binary& newValue) const;

	const static EventType EventTypeEnum = EventType::Binary;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::BinaryInput;
	
	typedef EventBinaryVariation EventVariation;
	typedef StaticBinaryVariation StaticVariation;
	typedef SimpleEventMetadata<Binary> MetadataType;
};

/**
The Double-bit Binary data type has two stable states, on and off, and an in transit state. Motor operated switches
or binary valves are good examples.
*/
class DoubleBitBinary : public TypedMeasurement<DoubleBit>
{
public:


	DoubleBitBinary();

	DoubleBitBinary(DoubleBit value);

	DoubleBitBinary(uint8_t quality);

	DoubleBitBinary(uint8_t quality, uint64_t time);

	DoubleBitBinary(DoubleBit value, uint8_t quality);

	DoubleBitBinary(DoubleBit value, uint8_t quality, uint64_t time);

	bool IsEvent(const DoubleBitBinary& newValue) const;

	const static EventType EventTypeEnum = EventType::DoubleBitBinary;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::DoubleBinaryInput;

	typedef EventDoubleBinaryVariation EventVariation;
	typedef StaticDoubleBinaryVariation StaticVariation;
	typedef SimpleEventMetadata<DoubleBitBinary> MetadataType;

private:

	static const uint8_t ValueMask = 0xC0;
	static const uint8_t QualityMask = 0x3F;

	static DoubleBit GetValue(uint8_t quality);

	static uint8_t GetQual(uint8_t quality, DoubleBit state);
};


/**
	BinaryOutputStatus is used for describing the current state of a control. It is very infrequently
	used and many masters don't provide any mechanisms for reading these values so their use is
	strongly discouraged, a Binary should be used instead.
*/
class BinaryOutputStatus : public TypedMeasurement<bool>
{
public:

	BinaryOutputStatus();

	BinaryOutputStatus(bool value);

	BinaryOutputStatus(uint8_t quality);

	BinaryOutputStatus(uint8_t quality, uint64_t time);

	BinaryOutputStatus(bool value, uint8_t quality);

	BinaryOutputStatus(bool value, uint8_t quality, uint64_t aTime);

	bool IsEvent(const BinaryOutputStatus& newValue) const;

	const static EventType EventTypeEnum = EventType::BinaryOutputStatus;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::BinaryOutputStatus;

	typedef EventBinaryOutputStatusVariation EventVariation;
	typedef StaticBinaryOutputStatusVariation StaticVariation;
	typedef SimpleEventMetadata<BinaryOutputStatus> MetadataType;
};

/**
	Analogs are used for variable data points that usuually reflect a real world value.
	Good examples are current, voltage, sensor readouts, etc. Think of a speedometer gauge.
*/

class Analog : public TypedMeasurement<double>
{
public:

	Analog();

	Analog(double value);

	Analog(double value, uint8_t quality);

	Analog(double value, uint8_t quality, uint64_t time);

	bool IsEvent(const Analog& newValue, double deadband) const;

	const static EventType EventTypeEnum = EventType::Analog;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::AnalogInput;

	typedef EventAnalogVariation EventVariation;
	typedef StaticAnalogVariation StaticVariation;
	typedef DeadbandMetadata<Analog, double> MetadataType;	
};

/**
	Counters are used for describing generally increasing values (non-negative!). Good examples are
	total power consumed, max voltage. Think odometer on a car.
*/
class Counter : public TypedMeasurement<uint32_t>
{
public:

	Counter();

	Counter(uint32_t value);

	Counter(uint32_t value, uint8_t quality);

	Counter(uint32_t value, uint8_t quality, uint64_t aTime);

	bool IsEvent(const Counter& newValue, uint32_t aDeadband) const;

	const static EventType EventTypeEnum = EventType::Counter;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::Counter;

	typedef EventCounterVariation EventVariation;
	typedef StaticCounterVariation StaticVariation;
	typedef DeadbandMetadata<Counter, uint32_t> MetadataType;
};

/**
	Frozen counters are used to report the value of a counter point captured at the instant when the count is frozen.
*/
class FrozenCounter : public TypedMeasurement<uint32_t>
{
public:

	FrozenCounter();

	FrozenCounter(uint32_t value);	

	FrozenCounter(uint32_t value, uint8_t quality);

	FrozenCounter(uint32_t value, uint8_t quality, uint64_t aTime);

	bool IsEvent(const FrozenCounter& newValue, uint32_t aDeadband) const;

	const static EventType EventTypeEnum = EventType::FrozenCounter;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::FrozenCounter;

	typedef EventFrozenCounterVariation EventVariation;
	typedef StaticFrozenCounterVariation StaticVariation;
	typedef DeadbandMetadata<FrozenCounter, uint32_t> MetadataType;
};

/**
	Describes the last set value of the setpoint. Like the BinaryOutputStatus data type it is not
	well supportted and its generally better practice to use an explict analog.
*/
class AnalogOutputStatus : public TypedMeasurement<double>
{
public:

	AnalogOutputStatus();

	AnalogOutputStatus(double value);	

	AnalogOutputStatus(double value, uint8_t quality);

	AnalogOutputStatus(double value, uint8_t quality, uint64_t aTime);

	bool IsEvent(const AnalogOutputStatus& newValue, double deadband) const;

	const static EventType EventTypeEnum = EventType::AnalogOutputStatus;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::AnalogOutputStatus;

	typedef EventAnalogOutputStatusVariation EventVariation;
	typedef StaticAnalogOutputStatusVariation StaticVariation;
	typedef DeadbandMetadata<AnalogOutputStatus, double> MetadataType;
};

}

#endif
