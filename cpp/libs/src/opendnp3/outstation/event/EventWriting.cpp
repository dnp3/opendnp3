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

#include "EventWriting.h"

#include "EventCollection.h"

namespace opendnp3
{

uint32_t EventWriting::Write(EventLists& lists, IEventWriteHandler& handler)
{
	uint32_t total_num_written = 0;

	auto iterator = lists.events.Iterate();

	while (true)
	{
		// continue calling WriteSome(..) until it fails to make progress
		auto num_written = WriteSome(iterator, lists, handler);

		if (num_written == 0)
		{
			return total_num_written;
		}
		else
		{
			total_num_written += num_written;
		}
	}
}

EventRecord* EventWriting::FindNextSelected(event_iter_t& iter, EventType type)
{
	while (true)
	{
		auto current = iter.CurrentValue();
		if (!current) return nullptr;

		if (current->state == EventState::selected)
		{
			// we terminate here since the type has changed
			return (current->type == type) ? current : nullptr;
		}
		else
		{
			iter.Next();
		}
	}
}

uint16_t EventWriting::WriteSome(event_iter_t& iterator, EventLists& lists, IEventWriteHandler& handler)
{
	// don't bother searching
	if (lists.counters.selected == 0) return 0;

	const auto value = iterator.Find([](const EventRecord & record)
	{
		return record.state == EventState::selected;
	});

	if (!value) return 0; // no match

	switch (value->type)
	{
	case(EventType::Binary):
		return WriteSomeOfType<BinarySpec>(iterator, lists, handler);
	case(EventType::DoubleBitBinary):
		return WriteSomeOfType<DoubleBitBinarySpec>(iterator, lists, handler);
	case(EventType::Counter):
		return WriteSomeOfType<CounterSpec>(iterator, lists, handler);
	case(EventType::FrozenCounter):
		return WriteSomeOfType<FrozenCounterSpec>(iterator, lists, handler);
	case(EventType::Analog):
		return WriteSomeOfType<AnalogSpec>(iterator, lists, handler);
	case(EventType::BinaryOutputStatus):
		return WriteSomeOfType<BinaryOutputStatusSpec>(iterator, lists, handler);
	case(EventType::AnalogOutputStatus):
		return WriteSomeOfType<AnalogOutputStatusSpec>(iterator, lists, handler);
	default:
		return 0;
	}
}

template <class T>
uint16_t EventWriting::WriteSomeOfType(event_iter_t& iterator, EventLists& lists, IEventWriteHandler& handler)
{
	const auto pos = iterator.CurrentValue();
	const auto storage = pos->StorageAs<T>();

	EventCollection<T> collection(iterator, lists.counters, storage->value.selectedVariation);

	return handler.Write(storage->value.selectedVariation, storage->value.value.time, collection);
}

}



