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

#include "MeasurementTypes.h"

namespace opendnp3
{

namespace quality {	

	bool GetBinaryValue(uint8_t quality)
	{
		return (quality & static_cast<uint8_t>(BinaryQuality::STATE)) != 0;
	}

	uint8_t GetBinaryQuality(uint8_t q, bool value)
	{
		return (value) ? (q | static_cast<uint8_t>(BinaryQuality::STATE)) : (q & (~static_cast<uint8_t>(BinaryQuality::STATE)));
	}

}


// ------------ Binary ---------------

Binary::Binary() : TypedMeasurement(false, quality::RESTART)
{}

Binary::Binary(bool value) : TypedMeasurement(value, quality::GetBinaryQuality(quality::ONLINE, value))
{}

Binary::Binary(uint8_t quality) : TypedMeasurement(quality::GetBinaryValue(quality), quality)
{}

Binary::Binary(uint8_t quality, DNPTime time) : TypedMeasurement(quality::GetBinaryValue(quality), quality, time)
{}

Binary::Binary(bool value, uint8_t quality) : TypedMeasurement(value, quality::GetBinaryQuality(quality, value))
{}

Binary::Binary(bool value, uint8_t quality, DNPTime time) : TypedMeasurement(value, quality::GetBinaryQuality(quality, value), time)
{}

bool Binary::IsQualityOnlineOnly() const
{
	bool isOnline = quality & static_cast<uint8_t>(BinaryQuality::ONLINE);
	uint8_t allowedBits = static_cast<uint8_t>(BinaryQuality::ONLINE) | static_cast<uint8_t>(BinaryQuality::STATE);
	bool otherBitsClear = (quality & ~allowedBits) == 0;

	return isOnline && otherBitsClear;	
}

bool Binary::IsEvent(const Binary& newValue) const
{
	return quality != newValue.quality;
}


// ------------ Double Bit Binary ---------------

DoubleBitBinary::DoubleBitBinary() : TypedMeasurement(DoubleBit::INDETERMINATE, quality::RESTART)
{}

DoubleBitBinary::DoubleBitBinary(DoubleBit value) : TypedMeasurement(value, GetQual(quality::ONLINE, value))
{}

DoubleBitBinary::DoubleBitBinary(uint8_t quality) : TypedMeasurement(GetValue(quality), quality)
{}

DoubleBitBinary::DoubleBitBinary(uint8_t quality, DNPTime time) : TypedMeasurement(GetValue(quality), quality, time)
{}

DoubleBitBinary::DoubleBitBinary(DoubleBit value, uint8_t quality) : TypedMeasurement(value, GetQual(quality, value))
{}

DoubleBitBinary::DoubleBitBinary(DoubleBit value, uint8_t quality, DNPTime time) : TypedMeasurement(value, GetQual(quality, value), time)
{}

bool DoubleBitBinary::IsEvent(const DoubleBitBinary& newValue) const
{
	return quality != newValue.quality;
}

DoubleBit DoubleBitBinary::GetValue(uint8_t quality)
{
	// the value is the top 2 bits, so down-shift 6
	uint8_t value = quality >> 6;
	return DoubleBitFromType(value);
}

uint8_t DoubleBitBinary::GetQual(uint8_t quality, DoubleBit state)
{
	uint8_t value = DoubleBitToType(state) << 6;
	return (QualityMask & quality) | value;
}

// ------------ Binary Output Status ---------------


BinaryOutputStatus::BinaryOutputStatus() : TypedMeasurement(false, quality::RESTART)
{}

BinaryOutputStatus::BinaryOutputStatus(bool value) : TypedMeasurement(value, quality::GetBinaryQuality(quality::ONLINE, value))
{}

BinaryOutputStatus::BinaryOutputStatus(uint8_t quality) : TypedMeasurement(quality::GetBinaryValue(quality), quality)
{}

BinaryOutputStatus::BinaryOutputStatus(uint8_t quality, DNPTime time) : TypedMeasurement(quality::GetBinaryValue(quality), quality, time)
{}

BinaryOutputStatus::BinaryOutputStatus(bool value, uint8_t quality) : TypedMeasurement(value, quality::GetBinaryQuality(quality, value))
{}

BinaryOutputStatus::BinaryOutputStatus(bool value, uint8_t quality, DNPTime time) : TypedMeasurement(value, quality::GetBinaryQuality(quality, value), time)
{}

bool BinaryOutputStatus::IsEvent(const BinaryOutputStatus& newValue) const
{
	return quality != newValue.quality;
}

// ------------ Analog ---------------

Analog::Analog() : TypedMeasurement(quality::RESTART)
{}

Analog::Analog(double value) : TypedMeasurement(value, quality::ONLINE)
{}

Analog::Analog(double value, uint8_t quality) : TypedMeasurement(value, quality)
{}

Analog::Analog(double value, uint8_t quality, DNPTime time) : TypedMeasurement<double>(value, quality, time)
{}

bool Analog::IsEvent(const Analog& newValue, double deadband) const
{
	return measurements::IsEvent(newValue, *this, deadband);
}

// ------------ Counter ---------------



Counter::Counter() : TypedMeasurement(0, quality::RESTART) {}

Counter::Counter(uint32_t value) : TypedMeasurement<uint32_t>(value, quality::ONLINE)
{}

Counter::Counter(uint32_t value, uint8_t quality) : TypedMeasurement<uint32_t>(value, quality)
{}

Counter::Counter(uint32_t value, uint8_t quality, DNPTime time) : TypedMeasurement<uint32_t>(value, quality, time)
{}

bool Counter::IsEvent(const Counter& newValue, uint32_t aDeadband) const
{
	if(quality != newValue.quality) return true;
	else
	{
		return measurements::IsEvent<uint32_t, uint64_t>(this->value, newValue.value, aDeadband);
	}
}

// ------------ Frozen Counter ---------------


FrozenCounter::FrozenCounter() : TypedMeasurement(0, quality::RESTART) {}

FrozenCounter::FrozenCounter(uint32_t value) : TypedMeasurement<uint32_t>(value, quality::ONLINE)
{}

FrozenCounter::FrozenCounter(uint32_t value, uint8_t quality) : TypedMeasurement<uint32_t>(value, quality)
{}

FrozenCounter::FrozenCounter(uint32_t value, uint8_t quality, DNPTime time) : TypedMeasurement<uint32_t>(value, quality, time)
{}

bool FrozenCounter::IsEvent(const FrozenCounter& newValue, uint32_t aDeadband) const
{
	if(quality != newValue.quality) return true;
	else
	{
		return measurements::IsEvent<uint32_t, uint64_t>(this->value, newValue.value, aDeadband);
	}
}

// ------------ Analog Output Status ---------------



AnalogOutputStatus::AnalogOutputStatus() : TypedMeasurement<double>(quality::RESTART) {}


AnalogOutputStatus::AnalogOutputStatus(double value) : TypedMeasurement<double>(value, quality::ONLINE)
{}

AnalogOutputStatus::AnalogOutputStatus(double value, uint8_t quality) : TypedMeasurement<double>(value, quality)
{}

AnalogOutputStatus::AnalogOutputStatus(double value, uint8_t quality, DNPTime time) : TypedMeasurement<double>(value, quality, time)
{}

bool AnalogOutputStatus::IsEvent(const AnalogOutputStatus& newValue, double deadband) const
{
	return measurements::IsEvent(newValue, *this, deadband);
}


} // end ns

