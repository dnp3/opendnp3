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

	* Only performs dynamic allocation at initialization
	* Maintains distinct lists for each type of event to optimize memory usage
*/

class EventStorage
{

public:

	explicit EventStorage(const EventBufferConfig& config);

	// ---- these functions return true if an overflow occurs ----

	inline bool Update(const Event<BinarySpec>& evt)
	{
		return UpdateAny(evt, this->binary);
	}

	inline bool Update(const Event<DoubleBitBinarySpec>& evt)
	{
		return UpdateAny(evt, this->doubleBinary);
	}

	inline bool Update(const Event<AnalogSpec>& evt)
	{
		return UpdateAny(evt, this->analog);
	}

	inline bool Update(const Event<CounterSpec>& evt)
	{
		return UpdateAny(evt, this->counter);
	}

	inline bool Update(const Event<FrozenCounterSpec>& evt)
	{
		return UpdateAny(evt, this->frozenCounter);
	}

	inline bool Update(const Event<BinaryOutputStatusSpec>& evt)
	{
		return UpdateAny(evt, this->binaryOutputStatus);
	}

	inline bool Update(const Event<AnalogOutputStatusSpec>& evt)
	{
		return UpdateAny(evt, this->analogOutputStatus);
	}

	inline uint32_t SelectBinary(EventBinaryVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->binary);
	}

	inline uint32_t SelectBinary(uint32_t max)
	{
		return this->SelectAny(max, this->binary);
	}

	inline uint32_t SelectDoubleBinary(EventDoubleBinaryVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->doubleBinary);
	}

	inline uint32_t SelectDoubleBinary(uint32_t max)
	{
		return this->SelectAny(max, this->doubleBinary);
	}

	inline uint32_t SelectAnalog(EventAnalogVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->analog);
	}

	inline uint32_t SelectAnalog(uint32_t max)
	{
		return this->SelectAny(max, this->analog);
	}

	inline uint32_t SelectCounter(EventCounterVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->counter);
	}

	inline uint32_t SelectCounter(uint32_t max)
	{
		return this->SelectAny(max, this->counter);
	}

	inline uint32_t SelectFrozenCounter(EventFrozenCounterVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->frozenCounter);
	}

	inline uint32_t SelectFrozenCounter(uint32_t max)
	{
		return this->SelectAny(max, this->frozenCounter);
	}

	inline uint32_t SelectBinaryOutputStatus(EventBinaryOutputStatusVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->binaryOutputStatus);
	}

	inline uint32_t SelectBinaryOutputStatus(uint32_t max)
	{
		return this->SelectAny(max, this->binaryOutputStatus);
	}

	inline uint32_t SelectAnalogOutputStatus(EventAnalogOutputStatusVariation variation, uint32_t max)
	{
		return this->SelectAny(variation, max, this->analogOutputStatus);
	}

	inline uint32_t SelectAnalogOutputStatus(uint32_t max)
	{
		return this->SelectAny(max, this->analogOutputStatus);
	}

private:

	struct EventRecord
	{

		enum State
		{
			queued,
			selected,
			written
		};

		EventRecord(
		    EventType type,
		    uint16_t index,
		    EventClass clazz
		) :
			type(type),
			index(index),
			clazz(clazz)
		{}

		EventRecord() = default;

		EventType type = EventType::Binary;
		uint16_t index = 0;
		EventClass clazz = EventClass::EC1;

		State state = State::queued;
		void* storage = nullptr;
	};

	template <class T>
	struct TypeRecord
	{
		TypeRecord() = default;

		TypeRecord(
		    typename T::meas_t value,
		    typename T::event_variation_t defaultVariation,
		    typename T::event_variation_t selectedVariation,
		    openpal::ListNode<EventRecord>* record
		) :
			value(value),
			defaultVariation(defaultVariation),
			selectedVariation(selectedVariation),
			record(record)
		{}

		typename T::meas_t value;
		typename T::event_variation_t defaultVariation;
		typename T::event_variation_t selectedVariation;
		openpal::ListNode<EventRecord>* record = nullptr;
	};

	// master list keeps the order
	openpal::LinkedList<EventRecord, uint32_t> events;

	// sub-lists just act as type-specific storage
	openpal::LinkedList<TypeRecord<BinarySpec>, uint32_t> binary;
	openpal::LinkedList<TypeRecord<DoubleBitBinarySpec>, uint32_t> doubleBinary;
	openpal::LinkedList<TypeRecord<AnalogSpec>, uint32_t> analog;
	openpal::LinkedList<TypeRecord<CounterSpec>, uint32_t> counter;
	openpal::LinkedList<TypeRecord<FrozenCounterSpec>, uint32_t> frozenCounter;
	openpal::LinkedList<TypeRecord<BinaryOutputStatusSpec>, uint32_t> binaryOutputStatus;
	openpal::LinkedList<TypeRecord<AnalogOutputStatusSpec>, uint32_t> analogOutputStatus;

	//EventClassCounters counters;

	template <class T>
	bool UpdateAny(const Event<T>& evt, openpal::LinkedList<TypeRecord<T>, uint32_t>& list);

	template <class T>
	uint32_t SelectAny(typename T::event_variation_t variation, uint32_t max, openpal::LinkedList<TypeRecord<T>, uint32_t>& list);

	template <class T>
	uint32_t SelectAny(uint32_t max, openpal::LinkedList<TypeRecord<T>, uint32_t>& list);

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
		auto record = head->value.record;

		// update the tracking counters
		// this->counters.Remove(record->value.clazz, record->value.state);

		// release the generic record
		this->events.Remove(record);
		// then type specific storage
		list.Remove(head);
	}

	const auto node = this->events.Add(
	                      EventRecord(
	                          T::EventTypeEnum,
	                          evt.index,
	                          evt.clazz
	                      )
	                  );

	node->value.storage = list.Add(
	                          TypeRecord<T>(
	                              evt.value,
	                              evt.variation,
	                              evt.variation,
	                              node
	                          )
	                      );

	//this->counters.Add(evt.clazz);

	return overflow;
}

template <class T>
uint32_t EventStorage::SelectAny(typename T::event_variation_t variation, uint32_t max, openpal::LinkedList<TypeRecord<T>, uint32_t>& list)
{
	uint32_t num_selected = 0;
	auto iter = list.Iterate();

	while (iter.HasNext() && num_selected < max)
	{
		auto node = iter.Next();
		auto record = node->value.record;
		if (record->value.state == EventRecord::State::queued)
		{
			// if not previously selected
			record->value.state = EventRecord::State::selected;
			record->value.type = T::EventTypeEnum;
			node->value.selectedVariation = variation;
			++num_selected;
		}
	}

	return num_selected;
}

template <class T>
uint32_t EventStorage::SelectAny(uint32_t max, openpal::LinkedList<TypeRecord<T>, uint32_t>& list)
{
	uint32_t num_selected = 0;
	auto iter = list.Iterate();

	while (iter.HasNext() && num_selected < max)
	{
		auto node = iter.Next();
		auto record = node->value.record;
		if (record->value.state == EventRecord::State::queued)
		{
			// if not previously selected
			record->value.state = EventRecord::State::selected;
			record->value.type = T::EventTypeEnum;
			node->value.selectedVariation = node->value.defaultVariation;
			++num_selected;
		}
	}

	return num_selected;
}

}

#endif

