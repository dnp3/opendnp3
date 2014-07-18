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

SOERecord::SOERecord() : type(EventType::Analog), index(0), clazz(EventClass::EC1), selected(false)
{}

SOERecord::SOERecord(const Binary& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::Binary), index(index_), clazz(clazz_), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.boolValue = meas.value;
}

SOERecord::SOERecord(const DoubleBitBinary& meas, uint16_t index_, EventClass clazz_) :
type(EventType::DoubleBitBinary), index(index_), clazz(clazz_), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.doubleBitValue = meas.value;
}

SOERecord::SOERecord(const BinaryOutputStatus& meas, uint16_t index_, EventClass clazz_) :
type(EventType::BinaryOutputStatus), index(index_), clazz(clazz_), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.boolValue = meas.value;
}

SOERecord::SOERecord(const Counter& meas, uint16_t index_, EventClass clazz_) :
type(EventType::Counter), index(index_), clazz(clazz_), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.uintValue = meas.value;
}

SOERecord::SOERecord(const FrozenCounter& meas, uint16_t index_, EventClass clazz_) :
type(EventType::FrozenCounter), index(index_), clazz(clazz_), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.uintValue = meas.value;
}

SOERecord::SOERecord(const Analog& meas, uint16_t index_, EventClass clazz_) :
type(EventType::Analog), index(index_), clazz(clazz_), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.analogValue = meas.value;
}

SOERecord::SOERecord(const AnalogOutputStatus& meas, uint16_t index_, EventClass clazz_) :
type(EventType::AnalogOutputStatus), index(index_), clazz(EventClass::EC1), selected(false), time(meas.time), flags(meas.quality)
{
	this->value.analogValue = meas.value;
}

template <>
void SOERecord::Read(Binary& meas)
{
	meas = Binary(value.boolValue, flags, time);
}

template <>
void SOERecord::Read(DoubleBitBinary& meas)
{
	meas = DoubleBitBinary(value.doubleBitValue, flags, time);
}

template <>
void SOERecord::Read(BinaryOutputStatus& meas)
{
	meas = BinaryOutputStatus(value.boolValue, flags, time);
}

template <>
void SOERecord::Read(Counter& meas)
{
	meas = Counter(value.uintValue, flags, time);
}

template <>
void SOERecord::Read(FrozenCounter& meas)
{
	meas = FrozenCounter(value.uintValue, flags, time);
}

template <>
void SOERecord::Read(Analog& meas)
{
	meas = Analog(value.analogValue, flags, time);
}

template <>
void SOERecord::Read(AnalogOutputStatus& meas)
{
	meas = AnalogOutputStatus(value.analogValue, flags, time);
}

}
