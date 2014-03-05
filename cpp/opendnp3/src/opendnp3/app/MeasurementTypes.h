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

#include <cmath>
#include <limits>

#include "BaseMeasurementTypes.h"
#include "QualityMasks.h"

namespace opendnp3
{

template <class T, class U>
bool ExceedsDeadband(const T& val1, const T& val2, T aDeadband)
{
	// T can be unsigned data type so std::abs won't work since it only directly supports signed data types
	// If one uses std::abs and T is unsigned one will get an ambiguous override error.

	U diff = (val2 > val1) ? (static_cast<U>(val2) - static_cast<U>(val1)) : (static_cast<U>(val1) - static_cast<U>(val2));

	return diff > aDeadband;	
}


/**
	The Binary data type is for describing on-off (boolean) type values. Good examples of
	binaries are alarms, mode settings, enabled/disabled flags etc. Think of it as a status
	LED on a piece of equipment.
*/
class Binary : public TypedMeasurement<bool>
{
public:

	Binary() : TypedMeasurement(false, BQ_RESTART)
	{}

	Binary(bool aValue) : TypedMeasurement(aValue, GetQual(BQ_ONLINE, aValue))
	{}

	Binary(uint8_t aQuality) : TypedMeasurement((aQuality & BQ_STATE) != 0, aQuality)
	{}

	Binary(uint8_t aQuality, int64_t aTime) : TypedMeasurement((aQuality & BQ_STATE) != 0, aQuality, aTime)
	{}
	
	Binary(bool aValue, uint8_t aQuality) : TypedMeasurement(aValue, GetQual(aQuality, aValue))
	{}

	Binary(bool aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement(aValue, GetQual(aQuality, aValue), aTime) 
	{}

	bool IsEvent(const Binary& newValue) const
	{
		return quality != newValue.quality;
	}	

private:
	static uint8_t GetQual(uint8_t q, bool aValue)
	{
		return (aValue) ? (q | BQ_STATE) : (q & (~BQ_STATE));
	}
};

enum class TwoBitState : uint8_t
{
	Intermediate = 0x00,
	DeterminedOff = 0x40,
	DeterminedOn = 0x80,
	Indeterminate = 0xC0
};

/**
The Double-bit Binary data type has two stable states, on and off, and an in transit state. Motor operated switches
or binary valves are good examples.
*/
class DoubleBitBinary : public TypedMeasurement<TwoBitState>
{
public:

	
	DoubleBitBinary() : TypedMeasurement(TwoBitState::Indeterminate, DBQ_RESTART)
	{}

	DoubleBitBinary(TwoBitState value) : TypedMeasurement(value, GetQual(DBQ_ONLINE, value))
	{}

	DoubleBitBinary(uint8_t aQuality) : TypedMeasurement(GetValueFromQuality(aQuality), aQuality)
	{}

	DoubleBitBinary(uint8_t aQuality, int64_t aTime) : TypedMeasurement(GetValueFromQuality(aQuality), aQuality, aTime)
	{}

	DoubleBitBinary(TwoBitState aValue, uint8_t aQuality) : TypedMeasurement(aValue, GetQual(aQuality, aValue))
	{}

	DoubleBitBinary(TwoBitState aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement(aValue, GetQual(aQuality, aValue), aTime)
	{}

	bool IsEvent(const DoubleBitBinary& newValue) const
	{
		return quality != newValue.quality;
	}

private:

	static const uint8_t ValueMask = 0xC0;
	static const uint8_t QualityMask = 0x3F;

	static TwoBitState GetValueFromQuality(uint8_t quality)
	{
		switch (quality & ValueMask)
		{
			case(0x00) :
				return TwoBitState::Intermediate;
			case(0x40) :
				return TwoBitState::DeterminedOff;
			case(0x80) :
				return TwoBitState::DeterminedOn;
			default:
				return TwoBitState::Indeterminate;
		}
	}

	static uint8_t GetQual(uint8_t quality, TwoBitState state)
	{
		return (QualityMask & quality) | (static_cast<uint8_t>(state)& ValueMask);		
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
	BinaryOutputStatus() : TypedMeasurement(false, TQ_RESTART) 
	{}

	BinaryOutputStatus(bool aValue) : TypedMeasurement(aValue, GetQual(TQ_ONLINE, aValue))
	{}

	BinaryOutputStatus(uint8_t aQuality) : TypedMeasurement((aQuality & TQ_STATE) != 0, aQuality)
	{}

	BinaryOutputStatus(bool aValue, uint8_t aQuality) : TypedMeasurement(aValue, GetQual(aQuality, aValue))
	{}

	BinaryOutputStatus(bool aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement(aValue, GetQual(aQuality, aValue), aTime) 
	{}	

private:

	static uint8_t GetQual(uint8_t q, bool aValue)
	{
		return (aValue) ? (q | TQ_STATE) : (q & (~TQ_STATE));
	}
};

/**
	Analogs are used for variable data points that usuually reflect a real world value.
	Good examples are current, voltage, sensor readouts, etc. Think of a speedometer gauge.
*/

class Analog : public TypedMeasurement<double>
{
public:

	Analog() : TypedMeasurement(AQ_RESTART) 
	{}

	Analog(double aValue) : TypedMeasurement(aValue, AQ_ONLINE) 
	{}

	Analog(double aValue, uint8_t aQuality) : TypedMeasurement(aValue, aQuality) 
	{}

	Analog(double aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<double>(aValue, aQuality, aTime) 
	{}

	bool IsEvent(const Analog& newValue, double aDeadband) const
	{
		if(quality != newValue.quality) return true;
		else 
		{
			double diff = fabs(GetValue() - newValue.GetValue());
			if(diff == std::numeric_limits<double>::infinity()) return true;
			else return diff > aDeadband;
		}		
	}		
};

/**
	Counters are used for describing generally increasing values (non-negative!). Good examples are
	total power consumed, max voltage. Think odometer on a car.
*/
class Counter : public TypedMeasurement<uint32_t>
{
public:

	Counter() : TypedMeasurement(0, CQ_RESTART) {}

	Counter(uint32_t aValue) : TypedMeasurement<uint32_t>(aValue, CQ_ONLINE) 
	{}

	Counter(uint32_t aValue, uint8_t aQuality) : TypedMeasurement<uint32_t>(aValue, aQuality) 
	{}

	Counter(uint32_t aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<uint32_t>(aValue, aQuality, aTime) 
	{}

	bool IsEvent(const Counter& newValue, uint32_t aDeadband) const
	{
		if(quality != newValue.quality) return true;
		else 
		{
			return ExceedsDeadband<uint32_t, uint64_t>(this->GetValue(), newValue.GetValue(), aDeadband);
		}
	}	
};

/**
	Frozen counters are used to report the value of a counter point captured at the instant when the count is frozen. 
*/
class FrozenCounter : public TypedMeasurement<uint32_t>
{
public:

	FrozenCounter() : TypedMeasurement(0, CQ_RESTART) {}

	FrozenCounter(uint32_t aValue) : TypedMeasurement<uint32_t>(aValue, CQ_ONLINE) 
	{}

	FrozenCounter(uint32_t aValue, uint8_t aQuality) : TypedMeasurement<uint32_t>(aValue, aQuality) 
	{}

	FrozenCounter(uint32_t aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<uint32_t>(aValue, aQuality, aTime) 
	{}

	bool IsEvent(const FrozenCounter& newValue, uint32_t aDeadband) const
	{
		if(quality != newValue.quality) return true;
		else 
		{
			return ExceedsDeadband<uint32_t, uint64_t>(this->GetValue(), newValue.GetValue(), aDeadband);
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
	
	AnalogOutputStatus() : TypedMeasurement<double>(PQ_RESTART) {}

	AnalogOutputStatus(double aValue) : TypedMeasurement<double>(aValue, PQ_ONLINE) 
	{}
	
	AnalogOutputStatus(double aValue, uint8_t aQuality) : TypedMeasurement<double>(aValue, aQuality) 
	{}

	AnalogOutputStatus(double aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<double>(aValue, aQuality, aTime) 
	{}	
};

}

#endif
