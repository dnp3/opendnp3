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
	type(EventType::Binary), index(index_), clazz(clazz_), selected(false)
{
	this->erasure.Set(meas);
}

SOERecord::SOERecord(const DoubleBitBinary& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::DoubleBitBinary), index(index_), clazz(clazz_), selected(false)
{
	this->erasure.Set(meas);
}

SOERecord::SOERecord(const BinaryOutputStatus& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::BinaryOutputStatus), index(index_), clazz(clazz_), selected(false)
{
	this->erasure.Set(meas);
}

SOERecord::SOERecord(const Counter& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::Counter), index(index_), clazz(clazz_), selected(false)
{
	this->erasure.Set(meas);
}

SOERecord::SOERecord(const FrozenCounter& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::FrozenCounter), index(index_), clazz(clazz_), selected(false)
{
	this->erasure.Set(meas);
}

SOERecord::SOERecord(const Analog& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::Analog), index(index_), clazz(clazz_), selected(false)
{
	this->erasure.Set(meas);
}

SOERecord::SOERecord(const AnalogOutputStatus& meas, uint16_t index_, EventClass clazz_) :
	type(EventType::AnalogOutputStatus), index(index_), clazz(EventClass::EC1), selected(false)
{
	this->erasure.Set(meas);
}

}
