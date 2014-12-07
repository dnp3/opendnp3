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

SOERecord::SOERecord() : SOERecord(EventType::Analog, EventClass::EC1, 0, 0 ,0)
{}

SOERecord::SOERecord(EventType type_, EventClass clazz_, uint16_t index_, uint64_t time_, uint8_t flags_) :
	type(type_),
	clazz(clazz_),
	selected(false),
	written(false), 
	index(index_),
	time(time_),
	flags(flags_)
{}

void SOERecord::Reset()
{
	selected = written = false;
}

void SOERecord::SelectDefault()
{
	selected = true;
	switch (type)
	{
		case(EventType::Binary) :
			value.binary.SelectDefaultVariation();
			break;
		case(EventType::DoubleBitBinary) :
			value.doubleBinary.SelectDefaultVariation();
			break;
		case(EventType::Counter) :
			value.counter.SelectDefaultVariation();
			break;
		case(EventType::FrozenCounter) :
			value.frozenCounter.SelectDefaultVariation();
			break;
		case(EventType::Analog) :
			value.analog.SelectDefaultVariation();
			break;
		case(EventType::BinaryOutputStatus) :
			value.binaryOutputStatus.SelectDefaultVariation();
			break;
		case(EventType::AnalogOutputStatus) :
			value.analogOutputStatus.SelectDefaultVariation();
			break;
	}
}

#include <openpal/executor/Function1.h>


SOERecord::SOERecord(const Binary& meas, uint16_t index_, EventClass clazz_, EventBinaryVariation var) :
	SOERecord(EventType::Binary, clazz_, index_, meas.time, meas.quality)
{		
	this->value.binary = ValueAndVariation <Binary> { meas.value, var, var };
}

SOERecord::SOERecord(const DoubleBitBinary& meas, uint16_t index_, EventClass clazz_, EventDoubleBinaryVariation var) :
	SOERecord(EventType::DoubleBitBinary, clazz_, index_, meas.time, meas.quality)
{
	this->value.doubleBinary = ValueAndVariation<DoubleBitBinary> { meas.value, var, var };
}

SOERecord::SOERecord(const BinaryOutputStatus& meas, uint16_t index_, EventClass clazz_, EventBinaryOutputStatusVariation var) :
	SOERecord(EventType::BinaryOutputStatus, clazz_, index_, meas.time, meas.quality)
{
	this->value.binaryOutputStatus = ValueAndVariation < BinaryOutputStatus > { meas.value, var, var };
}

SOERecord::SOERecord(const Counter& meas, uint16_t index_, EventClass clazz_, EventCounterVariation var) :
	SOERecord(EventType::Counter, clazz_, index_, meas.time, meas.quality)
{	
	this->value.counter = ValueAndVariation < Counter > { meas.value, var, var };
}

SOERecord::SOERecord(const FrozenCounter& meas, uint16_t index_, EventClass clazz_, EventFrozenCounterVariation var) :
	SOERecord(EventType::FrozenCounter, clazz_, index_, meas.time, meas.quality)
{
	this->value.frozenCounter = ValueAndVariation < FrozenCounter > { meas.value, var, var };
}

SOERecord::SOERecord(const Analog& meas, uint16_t index_, EventClass clazz_, EventAnalogVariation var) :
	SOERecord(EventType::Analog, clazz_, index_, meas.time, meas.quality)
{
	this->value.analog = ValueAndVariation < Analog > { meas.value, var, var };
}

SOERecord::SOERecord(const AnalogOutputStatus& meas, uint16_t index_, EventClass clazz_, EventAnalogOutputStatusVariation var) :
	SOERecord(EventType::AnalogOutputStatus, clazz_, index_, meas.time, meas.quality)
{
	this->value.analogOutputStatus = ValueAndVariation < AnalogOutputStatus > { meas.value, var, var };
}

void SOERecord::Select(EventBinaryVariation var)
{
	this->selected = true;
	value.binary.selectedVariation = var;
}

void SOERecord::Select(EventDoubleBinaryVariation var)
{
	this->selected = true;
	value.doubleBinary.selectedVariation = var;
}

void SOERecord::Select(EventBinaryOutputStatusVariation var)
{
	this->selected = true;
	value.binaryOutputStatus.selectedVariation = var;
}

void SOERecord::Select(EventCounterVariation var)
{
	this->selected = true;
	value.counter.selectedVariation = var;
}

void SOERecord::Select(EventFrozenCounterVariation var)
{
	this->selected = true;
	value.frozenCounter.selectedVariation = var;
}

void SOERecord::Select(EventAnalogVariation var)
{
	this->selected = true;
	value.analog.selectedVariation = var;
}

void SOERecord::Select(EventAnalogOutputStatusVariation var)
{
	this->selected = true;
	value.analogOutputStatus.selectedVariation = var;
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
