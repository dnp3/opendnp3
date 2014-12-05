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

#include "SOERecord.h"

namespace opendnp3
{

SOERecord::SOERecord() : type(EventType::Analog), clazz(EventClass::EC1), selected(false), flags(0)
{}

void SOERecord::Reset()
{
	selected = written = false;
}

SOERecord::SOERecord(const Binary& meas, uint16_t index_, EventClass clazz_, EventBinaryVariation var) :
	type(EventType::Binary), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.binary = ValueAndVariation <Binary> { meas.value, var, var };
}

SOERecord::SOERecord(const DoubleBitBinary& meas, uint16_t index_, EventClass clazz_, EventDoubleBinaryVariation var) :
	type(EventType::DoubleBitBinary), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.doubleBinary = ValueAndVariation<DoubleBitBinary> { meas.value, var, var };
}

SOERecord::SOERecord(const BinaryOutputStatus& meas, uint16_t index_, EventClass clazz_, EventBinaryOutputStatusVariation var) :
	type(EventType::BinaryOutputStatus), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.binaryOutputStatus = ValueAndVariation < BinaryOutputStatus > { meas.value, var, var };
}

SOERecord::SOERecord(const Counter& meas, uint16_t index_, EventClass clazz_, EventCounterVariation var) :
	type(EventType::Counter), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.counter = ValueAndVariation < Counter > { meas.value, var, var };
}

SOERecord::SOERecord(const FrozenCounter& meas, uint16_t index_, EventClass clazz_, EventFrozenCounterVariation var) :
	type(EventType::FrozenCounter), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.frozenCounter = ValueAndVariation < FrozenCounter > { meas.value, var, var };
}

SOERecord::SOERecord(const Analog& meas, uint16_t index_, EventClass clazz_, EventAnalogVariation var) :
	type(EventType::Analog), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.analog = ValueAndVariation < Analog > { meas.value, var, var };
}

SOERecord::SOERecord(const AnalogOutputStatus& meas, uint16_t index_, EventClass clazz_, EventAnalogOutputStatusVariation var) :
	type(EventType::AnalogOutputStatus), clazz(clazz_), selected(false), written(false), index(index_), time(meas.time), flags(meas.quality)
{
	this->value.analogOutputStatus = ValueAndVariation < AnalogOutputStatus > { meas.value, var, var };
}

template <>
EventInstance<Binary> SOERecord::Read<Binary>()
{
	return Convert<Binary>(value.binary);	
}

template <>
EventInstance<DoubleBitBinary> SOERecord::Read<DoubleBitBinary>()
{
	return Convert<DoubleBitBinary>(value.doubleBinary);	
}

template <>
EventInstance<BinaryOutputStatus> SOERecord::Read<BinaryOutputStatus>()
{
	return Convert<BinaryOutputStatus>(value.binaryOutputStatus);
}

template <>
EventInstance<Counter> SOERecord::Read()
{
	return Convert<Counter>(value.counter);
}

template <>
EventInstance<FrozenCounter> SOERecord::Read()
{
	return Convert<FrozenCounter>(value.frozenCounter);
}

template <>
EventInstance<Analog> SOERecord::Read()
{
	return Convert<Analog>(value.analog);
}

template <>
EventInstance<AnalogOutputStatus> SOERecord::Read()
{
	return Convert<AnalogOutputStatus>(value.analogOutputStatus);
}

}
