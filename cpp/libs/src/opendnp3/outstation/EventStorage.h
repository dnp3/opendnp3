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
#ifndef OPENDNP3_EVENTSTORAGE_H
#define OPENDNP3_EVENTSTORAGE_H

#include "opendnp3/outstation/EventBufferConfig.h"
#include "opendnp3/app/MeasurementTypeSpecs.h"
#include "opendnp3/outstation/Event.h"

#include <openpal/container/LinkedList.h>

namespace opendnp3
{

/*
	Data-stucture for holding events. 

	Only performs dynamic allocation at initialization
*/

class EventStorage
{

public:

	explicit EventStorage(const EventBufferConfig& config);

	// --- these functions return true if an overflow occurs --- 

	bool Update(const Event<BinarySpec>& evt);
	bool Update(const Event<DoubleBitBinarySpec>& evt);
	bool Update(const Event<AnalogSpec>& evt);
	bool Update(const Event<CounterSpec>& evt);
	bool Update(const Event<FrozenCounterSpec>& evt);
	bool Update(const Event<BinaryOutputStatusSpec>& evt);
	bool Update(const Event<AnalogOutputStatusSpec>& evt);

private:

	struct Record {
		Record() = default;

		Record(
			EventType type,
			uint16_t index,
			EventClass clazz
		) :
			type(type),
			index(index),
			clazz(clazz)
		{}

		EventType type;
		uint16_t index;
		EventClass clazz;

		void* typeNode = nullptr;
		bool selected = false;
		bool written = false;
	};

	template <class T>
	struct TypeRecord
	{
		TypeRecord() = default;

		TypeRecord(
			typename T::meas_t value,
			typename T::event_variation_t defaultVariation,
			typename T::event_variation_t selectedVariation,
			openpal::ListNode<Record>* record
		) :
			value(value),
			defaultVariation(defaultVariation),
			selectedVariation(selectedVariation),
			record(record)
		{}

		typename T::meas_t value;
		typename T::event_variation_t defaultVariation;
		typename T::event_variation_t selectedVariation;
		openpal::ListNode<Record>* record = nullptr;
	};

	// master list keeps the order
	openpal::LinkedList<Record, uint32_t> events;

	// sub-lists just act as type-specific storage
	openpal::LinkedList<TypeRecord<BinarySpec>, uint32_t> binary;
	openpal::LinkedList<TypeRecord<DoubleBitBinarySpec>, uint32_t> doubleBinary;
	openpal::LinkedList<TypeRecord<AnalogSpec>, uint32_t> analog;
	openpal::LinkedList<TypeRecord<CounterSpec>, uint32_t> counter;
	openpal::LinkedList<TypeRecord<FrozenCounterSpec>, uint32_t> frozenCounter;
	openpal::LinkedList<TypeRecord<BinaryOutputStatusSpec>, uint32_t> binaryOutputStatus;
	openpal::LinkedList<TypeRecord<AnalogOutputStatusSpec>, uint32_t> analogOutputStatus;

	template <class T>
	bool UpdateAny(const Event<T>& evt, openpal::LinkedList<TypeRecord<T>, uint32_t>& list);
};

template <class T>
bool EventStorage::UpdateAny(const Event<T>& evt, openpal::LinkedList<TypeRecord<T>, uint32_t>& list)
{
	// a zero capacity list should never overflow
	if (list.Capacity() == 0) return false;

	bool overflow = false;
	
	if (list.IsFull())
	{
		// overflow condition, we must first remove a value
		overflow = true;

		const auto head = list.Head();

		// release the generic record
		this->events.Remove(head->value.record);
		// then type specific storage
		list.Remove(head);
	}

	const auto node = this->events.Add(
		Record(
			T::EventTypeEnum,
			evt.index,
			evt.clazz
		)
	);

	node->value.typeNode = list.Add(
		TypeRecord<T>(
			evt.value,
			evt.variation,
			evt.variation,
			node
		)
	);

	return overflow;
}


}

#endif

