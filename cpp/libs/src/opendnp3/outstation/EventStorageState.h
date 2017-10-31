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
#ifndef OPENDNP3_EVENTSTORAGESTATE_H
#define OPENDNP3_EVENTSTORAGESTATE_H

#include "opendnp3/outstation/EventBufferConfig.h"
#include "opendnp3/outstation/EventRecord.h"
#include "opendnp3/outstation/TypedEventRecord.h"
#include "opendnp3/outstation/ClazzCount.h"
#include "opendnp3/outstation/Event.h"

#include "openpal/util/Uncopyable.h"

namespace opendnp3
{

struct EventStorageState : private openpal::Uncopyable
{

	EventStorageState(const EventBufferConfig& config);

	EventStorageState() = delete;

	// master list keeps the aggregate order and generic data
	openpal::LinkedList<EventRecord, uint32_t> events;

	// sub-lists just act as type-specific storage
	openpal::LinkedList<TypedEventRecord<BinarySpec>, uint32_t> binary;
	openpal::LinkedList<TypedEventRecord<DoubleBitBinarySpec>, uint32_t> doubleBinary;
	openpal::LinkedList<TypedEventRecord<AnalogSpec>, uint32_t> analog;
	openpal::LinkedList<TypedEventRecord<CounterSpec>, uint32_t> counter;
	openpal::LinkedList<TypedEventRecord<FrozenCounterSpec>, uint32_t> frozenCounter;
	openpal::LinkedList<TypedEventRecord<BinaryOutputStatusSpec>, uint32_t> binaryOutputStatus;
	openpal::LinkedList<TypedEventRecord<AnalogOutputStatusSpec>, uint32_t> analogOutputStatus;

	EventClassCounters counters;

	template <class T>
	bool UpdateAny(const Event<T>& evt);

	template <class T>
	uint32_t SelectByType(uint32_t max)
	{
		return this->SelectByType(true, 0, uint16_t max);
	}

	template <class T>
	uint32_t SelectByType(typename T::event_variation_t variation, uint32_t max)
	{
		return this->SelectByType<T>(false, variation, max);
	}

private:

	template <class T>
	uint32_t SelectByType(bool useDefaultVariation, typename T::event_variation_t variation, uint32_t max);

	template <class T>
	openpal::LinkedList<TypedEventRecord<T>, uint32_t>& GetList();
};

template <class T>
bool EventStorageState::UpdateAny(const Event<T>& evt)
{
	auto& list = this->GetList<T>();

	// a zero capacity list should never overflow
	if (list.Capacity() == 0) return false;

	bool overflow = false;

	if (list.IsFull())
	{
		// overflow condition, we must first remove a value first
		overflow = true;

		const auto head = list.Head();
		auto record_node = head->value.record;

		// update the tracking counters
		this->counters.Remove(record_node->value.clazz, record_node->value.state);

		// release the generic record
		this->events.Remove(record_node);

		// then type specific storage
		list.Remove(head);
	}

	const auto node = this->events.Add(
	                      EventRecord(evt.index, evt.clazz)
	                  );

	const auto typed = list.Add(
	                       TypedEventRecord<T>(
	                           evt.value,
	                           evt.variation,
	                           evt.variation,
	                           node
	                       )
	                   );

	node->value.SetStorageNode(typed);

	this->counters.Add(evt.clazz);

	return overflow;
}

template <class T>
uint32_t EventStorageState::SelectByType(bool useDefaultVariation, typename T::event_variation_t variation, uint32_t max)
{
	auto& list = this->GetList<T>();

	uint32_t num_selected = 0;

	// TODO

	return num_selected;
}

}

#endif

