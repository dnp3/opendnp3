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

#include "EventStorage.h"

namespace opendnp3
{

EventStorage::EventStorage(const EventBufferConfig& config) :
	events(config.TotalEvents()),
	binary(config.maxBinaryEvents),
	doubleBinary(config.maxDoubleBinaryEvents),
	analog(config.maxAnalogEvents),
	counter(config.maxCounterEvents),
	frozenCounter(config.maxFrozenCounterEvents),
	binaryOutputStatus(config.maxBinaryOutputStatusEvents),
	analogOutputStatus(config.maxAnalogOutputStatusEvents)
{}

uint32_t EventStorage::Select(EventClass clazz, uint32_t max)
{
	uint32_t num_selected = 0;
	auto iter = this->events.Iterate();

	while (iter.HasNext() && num_selected < max)
	{
		auto node = iter.Next();
		auto record = node->value;
		if (node->value.state == EventState::queued && node->value.clazz == clazz)
		{
			// if not previously selected
			node->value.state = EventState::selected;
			// TODO - set the storage to use the default variation
			//node->value.selectedVariation = useDefaultVariation ? node->value.defaultVariation : variation;
			++num_selected;
		}
	}

	return num_selected;
}

uint32_t EventStorage::Write(EventWriteHandler& handler)
{
	// iterate over the selected events in the buffer
	auto iterator = this->events.Iterate();

	uint32_t total_num_written = 0;
	while (true)
	{
		uint16_t num_written = this->WriteSome(handler, iterator);
		if (num_written == 0)
		{
			return total_num_written;
		}
		else
		{
			total_num_written += num_written;
		}
	}
	
	return total_num_written;
}

uint16_t EventStorage::WriteSome(EventWriteHandler& handler, event_iterator_t& iterator)
{
	const auto next = iterator.Find(EventStorage::IsSelected);
	
	// we are out of selected events
	if (!next) return 0;

	// now enter a type-dependent write routine
	switch (next->type)
	{
	case(EventType::Binary):
		return WriteSomeOfType<BinaryInfo>(handler, iterator, *next);
	case(EventType::DoubleBitBinary):
		return WriteSomeOfType<DoubleBitBinaryInfo>(handler, iterator, *next);
	case(EventType::Counter):
		return WriteSomeOfType<CounterInfo>(handler, iterator, *next);
	case(EventType::FrozenCounter):
		return WriteSomeOfType<FrozenCounterInfo>(handler, iterator, *next);
	case(EventType::Analog):
		return WriteSomeOfType<AnalogInfo>(handler, iterator, *next);
	case(EventType::BinaryOutputStatus):
		return WriteSomeOfType<BinaryOutputStatusInfo>(handler, iterator, *next);
	case(EventType::AnalogOutputStatus):
		return WriteSomeOfType<AnalogOutputStatusInfo>(handler, iterator, *next);
	default:
		// this case should never happen, terminate
		return 0;
	}
}

template <class T>
uint16_t EventStorage::WriteSomeOfType(
	EventWriteHandler& handler,
	event_iterator_t& iterator,
	EventRecord& first
)
{
	const auto variation = reinterpret_cast<openpal::ListNode<TypeRecord<T>>*>(first.storage)->value.selectedVariation;

	// create a collection of a particular type / variation
	EventCollectionImpl<T> collection(
		iterator,
		variation
	);

	handler.Write(variation, collection);

	return collection.GetNumWritten();
}

template <class T>
uint16_t EventStorage::EventCollectionImpl<T>::WriteSome(EventWriter<typename T::meas_t>& writer)
{
	EventRecord* record = this->iterator.CurrentValue();
	TypeRecord<T>* data = &reinterpret_cast<openpal::ListNode<TypeRecord<T>>*>(record->storage)->value;

	while (true) {

		const auto success = writer.Write(data->value, record->index);
		if (success)
		{
			record->state = EventState::written;
			++this->num_written;

			// see if the next value also matches type/varition
			this->iterator.Next();
			auto node = this->iterator.Current();
			// we've hit the end
			if (!node) return num_written;
			record = &node->value;

			// the next event isn't this type
			if (record->type != T::EventTypeEnum) return num_written;

			data = &reinterpret_cast<openpal::ListNode<TypeRecord<T>>*>(record->storage)->value;
			// the next event will be reported using a different variation
			if (data->selectedVariation != this->variation) return num_written;

			// otherwise proceed to the next iteration!
		}
		else
		{
			return num_written;
		}
	}
}


}


