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
#ifndef __MEASUREMENT_TYPES_H_
#define __MEASUREMENT_TYPES_H_

#include "QualityMasks.h"
#include "EventTriggers.h"
#include "opendnp3/gen/DoubleBit.h"

namespace opendnp3
{

/**
	The Binary data type is for describing on-off (boolean) type values. Good examples of
	binaries are alarms, mode settings, enabled/disabled flags etc. Think of it as a status
	LED on a piece of equipment.
*/
class Binary : public TypedMeasurement<bool>
{
public:

	Binary() : TypedMeasurement(false, ToUnderlying(BinaryQuality::RESTART))
	{}

	Binary(bool value) : TypedMeasurement(value, GetQual(ToUnderlying(BinaryQuality::ONLINE), value))
	{}

	Binary(uint8_t quality) : TypedMeasurement((quality & ToUnderlying(BinaryQuality::STATE)) != 0, quality)
	{}

	Binary(uint8_t quality, uint64_t aTime) : TypedMeasurement((quality & ToUnderlying(BinaryQuality::STATE)) != 0, quality, aTime)
	{}

	Binary(bool value, uint8_t quality) : TypedMeasurement(value, GetQual(quality, value))
	{}

	Binary(bool value, uint8_t quality, uint64_t aTime) : TypedMeasurement(value, GetQual(quality, value), aTime)
	{}

	bool IsEvent(const Binary& newValue) const
	{
		return quality != newValue.quality;
	}

private:
	static uint8_t GetQual(uint8_t q, bool value)
	{
		return (value) ? (q | ToUnderlying(BinaryQuality::STATE)) : (q & (~ToUnderlying(BinaryQuality::STATE)));
	}
};

/**
The Double-bit Binary data type has two stable states, on and off, and an in transit state. Motor operated switches
or binary valves are good examples.
*/
class DoubleBitBinary : public TypedMeasurement<DoubleBit>
{
public:


	DoubleBitBinary() : TypedMeasurement(DoubleBit::INDETERMINATE, ToUnderlying(DoubleBitBinaryQuality::RESTART))
	{}

	DoubleBitBinary(DoubleBit value) : TypedMeasurement(value, GetQual(ToUnderlying(DoubleBitBinaryQuality::ONLINE), value))
	{}

	DoubleBitBinary(uint8_t quality) : TypedMeasurement(GetValueFromQuality(quality), quality)
	{}

	DoubleBitBinary(uint8_t quality, uint64_t aTime) : TypedMeasurement(GetValueFromQuality(quality), quality, aTime)
	{}

	DoubleBitBinary(DoubleBit value, uint8_t quality) : TypedMeasurement(value, GetQual(quality, value))
	{}

	DoubleBitBinary(DoubleBit value, uint8_t quality, uint64_t aTime) : TypedMeasurement(value, GetQual(quality, value), aTime)
	{}

	bool IsEvent(const DoubleBitBinary& newValue) const
	{
		return quality != newValue.quality;
	}

private:

	static const uint8_t ValueMask = 0xC0;
	static const uint8_t QualityMask = 0x3F;

	static DoubleBit GetValueFromQuality(uint8_t quality)
	{
		// the value is the top 2 bits, so downshift 6
		uint8_t value = quality >> 6;
		return DoubleBitFromType(value);
	}

	static uint8_t GetQual(uint8_t quality, DoubleBit state)
	{
		uint8_t value = DoubleBitToType(state) << 6;
		return (QualityMask & quality) | value;
	}
};


/**
	BinaryOutputStatus is used for describing the current state of a control. It is very infrequently
	used and many masters don't provide any mechanisms for reading these values so their use is
	strongly discouraged, a Binary should be used instead.
*/
class BinaryOutputStatus : public TypedMeasurement<bool>
{
public:
	BinaryOutputStatus() : TypedMeasurement(false, ToUnderlying(BinaryOutputStatusQuality::RESTART))
	{}

	BinaryOutputStatus(bool value) : TypedMeasurement(value, GetQual(ToUnderlying(BinaryOutputStatusQuality::ONLINE), value))
	{}

	BinaryOutputStatus(uint8_t quality) : TypedMeasurement((quality & ToUnderlying(BinaryOutputStatusQuality::STATE)) != 0, quality)
	{}

	BinaryOutputStatus(uint8_t quality, uint64_t aTime) : TypedMeasurement((quality& ToUnderlying(BinaryOutputStatusQuality::STATE)) != 0, quality, aTime)
	{}

	BinaryOutputStatus(bool value, uint8_t quality) : TypedMeasurement(value, GetQual(quality, value))
	{}

	BinaryOutputStatus(bool value, uint8_t quality, uint64_t aTime) : TypedMeasurement(value, GetQual(quality, value), aTime)
	{}

	bool IsEvent(const BinaryOutputStatus& newValue) const
	{
		return quality != newValue.quality;
	}

private:

	static uint8_t GetQual(uint8_t q, bool value)
	{
		return (value) ? (q | ToUnderlying(BinaryOutputStatusQuality::STATE)) : (q & (~ToUnderlying(BinaryOutputStatusQuality::STATE)));
	}
};

/**
	Analogs are used for variable data points that usuually reflect a real world value.
	Good examples are current, voltage, sensor readouts, etc. Think of a speedometer gauge.
*/

class Analog : public TypedMeasurement<double>
{
public:

	Analog() : TypedMeasurement(ToUnderlying(AnalogQuality::RESTART))
	{}

	Analog(double value) : TypedMeasurement(value, ToUnderlying(AnalogQuality::ONLINE))
	{}

	Analog(double value, uint8_t quality) : TypedMeasurement(value, quality)
	{}

	Analog(double value, uint8_t quality, uint64_t aTime) : TypedMeasurement<double>(value, quality, aTime)
	{}

	bool IsEvent(const Analog& newValue, double deadband) const
	{
		return measurements::IsEvent(newValue, *this, deadband);
	}
};

/**
	Counters are used for describing generally increasing values (non-negative!). Good examples are
	total power consumed, max voltage. Think odometer on a car.
*/
class Counter : public TypedMeasurement<uint32_t>
{
public:

	Counter() : TypedMeasurement(0, ToUnderlying(CounterQuality::RESTART)) {}

	Counter(uint32_t value) : TypedMeasurement<uint32_t>(value, ToUnderlying(CounterQuality::ONLINE))
	{}

	Counter(uint32_t value, uint8_t quality) : TypedMeasurement<uint32_t>(value, quality)
	{}

	Counter(uint32_t value, uint8_t quality, uint64_t aTime) : TypedMeasurement<uint32_t>(value, quality, aTime)
	{}

	bool IsEvent(const Counter& newValue, uint32_t aDeadband) const
	{
		if(quality != newValue.quality) return true;
		else
		{
			return measurements::IsEvent<uint32_t, uint64_t>(this->value, newValue.value, aDeadband);
		}
	}
};

/**
	Frozen counters are used to report the value of a counter point captured at the instant when the count is frozen.
*/
class FrozenCounter : public TypedMeasurement<uint32_t>
{
public:

	FrozenCounter() : TypedMeasurement(0, ToUnderlying(CounterQuality::RESTART)) {}

	FrozenCounter(uint32_t value) : TypedMeasurement<uint32_t>(value, ToUnderlying(CounterQuality::ONLINE))
	{}

	FrozenCounter(uint32_t value, uint8_t quality) : TypedMeasurement<uint32_t>(value, quality)
	{}

	FrozenCounter(uint32_t value, uint8_t quality, uint64_t aTime) : TypedMeasurement<uint32_t>(value, quality, aTime)
	{}

	bool IsEvent(const FrozenCounter& newValue, uint32_t aDeadband) const
	{
		if(quality != newValue.quality) return true;
		else
		{
			return measurements::IsEvent<uint32_t, uint64_t>(this->value, newValue.value, aDeadband);
		}
	}
};

/**
	Describes the last set value of the setpoint. Like the BinaryOutputStatus data type it is not
	well supportted and its generally better practice to use an explict analog.
*/
class AnalogOutputStatus : public TypedMeasurement<double>
{
public:

	AnalogOutputStatus() : TypedMeasurement<double>(ToUnderlying(AnalogQuality::RESTART)) {}

	AnalogOutputStatus(double value) : TypedMeasurement<double>(value, ToUnderlying(AnalogQuality::ONLINE))
	{}

	AnalogOutputStatus(double value, uint8_t quality) : TypedMeasurement<double>(value, quality)
	{}

	AnalogOutputStatus(double value, uint8_t quality, uint64_t aTime) : TypedMeasurement<double>(value, quality, aTime)
	{}

	bool IsEvent(const AnalogOutputStatus& newValue, double deadband) const
	{
		return measurements::IsEvent(newValue, *this, deadband);
	}
};

}

#endif
