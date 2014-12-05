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

#ifndef OPENDNP3_SOERECORD_H
#define OPENDNP3_SOERECORD_H

#include "opendnp3/app/EventType.h"
#include "opendnp3/app/MeasurementTypes.h"


namespace opendnp3
{

template <class T>
struct ValueAndVariation
{	
	typename T::ValueType value;	
	typename T::EventVariation defaultVariation;
	typename T::EventVariation selectedVariation;
};

template <class ValueType>
struct EventInstance
{
	ValueType value;
	uint16_t index;
	typename ValueType::EventVariation variation;
};

union EventValue
{
	ValueAndVariation<Binary> binary;
	ValueAndVariation<DoubleBitBinary> doubleBinary;
	ValueAndVariation<BinaryOutputStatus> binaryOutputStatus;
	ValueAndVariation<Counter> counter;
	ValueAndVariation<FrozenCounter> frozenCounter;
	ValueAndVariation<Analog> analog;
	ValueAndVariation<AnalogOutputStatus> analogOutputStatus;
};

class SOERecord
{
public:

	SOERecord();

	SOERecord(const Binary& meas, uint16_t index, EventClass clazz, EventBinaryVariation var);
	SOERecord(const DoubleBitBinary& meas, uint16_t index, EventClass clazz, EventDoubleBinaryVariation var);
	SOERecord(const BinaryOutputStatus& meas, uint16_t index, EventClass clazz, EventBinaryOutputStatusVariation var);
	SOERecord(const Counter& meas, uint16_t index, EventClass clazz, EventCounterVariation var);
	SOERecord(const FrozenCounter& meas, uint16_t index, EventClass clazz, EventFrozenCounterVariation var);
	SOERecord(const Analog& meas, uint16_t index, EventClass clazz, EventAnalogVariation var);
	SOERecord(const AnalogOutputStatus& meas, uint16_t index, EventClass clazz, EventAnalogOutputStatusVariation var);

	template <class T>
	EventInstance<T> Read();

	EventType type;	
	EventClass clazz;
	bool selected;
	bool written;

	void Reset();

private:

	template <class T>
	EventInstance<T> Convert(const ValueAndVariation<T>& value)
	{
		return EventInstance < T > {
			T(value.value, flags, time),
			index,
			value.selectedVariation
		};
	}


	// the actual value;
	EventValue value;
	uint16_t index;
	uint64_t time;
	uint8_t flags;

};

}

#endif
