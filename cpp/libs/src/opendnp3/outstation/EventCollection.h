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
#ifndef OPENDNP3_EVENTCOLLECTION_H
#define OPENDNP3_EVENTCOLLECTION_H

#include "opendnp3/outstation/IEventWriteHandler.h"

namespace opendnp3
{

template <class T>
class EventCollection final : public IEventCollection<typename T::meas_t>
{
private:
	uint16_t num_written = 0;
	openpal::LinkedListIterator<EventRecord>& iterator;
	typename T::event_variation_t variation;

public:

	EventCollection(
	    openpal::LinkedListIterator<EventRecord>& iterator,
	    typename T::event_variation_t variation
	) :
		iterator(iterator),
		variation(variation)
	{}

	uint16_t GetNumWritten() const
	{
		return num_written;
	}

	virtual uint16_t WriteSome(IEventWriter<typename T::meas_t>& writer) override;

};

template <class T>
uint16_t EventCollection<T>::WriteSome(IEventWriter<typename T::meas_t>& writer)
{
	while (true)
	{

		EventRecord* record = this->iterator.CurrentValue();
		TypedEventRecord<T>* data = &reinterpret_cast<openpal::ListNode<TypedEventRecord<T>>*>(record->storage_node)->value;

		const auto success = writer.Write(data->value, record->index);
		if (!success) return num_written;

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

		data = &reinterpret_cast<openpal::ListNode<TypedEventRecord<T>>*>(record->storage_node)->value;
		// the next event will be reported using a different variation
		if (data->selectedVariation != this->variation) return num_written;

		// otherwise proceed to the next iteration!
	}
}

}

#endif

