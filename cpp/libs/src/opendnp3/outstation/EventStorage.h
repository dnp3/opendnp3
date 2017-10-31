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

#include "opendnp3/outstation/Event.h"
#include "opendnp3/outstation/IEventWriteHandler.h"
#include "opendnp3/outstation/EventStorageState.h"

#include <limits>

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

	// write selected events to some handler

	uint32_t Write(IEventWriteHandler& handler);


	// ---- these functions return true if an overflow occurs ----

	inline bool Update(const Event<BinarySpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	inline bool Update(const Event<DoubleBitBinarySpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	inline bool Update(const Event<AnalogSpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	inline bool Update(const Event<CounterSpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	inline bool Update(const Event<FrozenCounterSpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	inline bool Update(const Event<BinaryOutputStatusSpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	inline bool Update(const Event<AnalogOutputStatusSpec>& evt)
	{
		return this->state.UpdateAny(evt);
	}

	// ---- function used to select various events ----

	inline uint32_t Select(EventBinaryVariation variation, uint32_t max)
	{
		return this->state.SelectByType<BinarySpec>(variation, max);
	}

	inline uint32_t Select(EventDoubleBinaryVariation variation, uint32_t max)
	{
		return this->state.SelectByType<DoubleBitBinarySpec>(variation, max);
	}

	inline uint32_t Select(EventAnalogVariation variation, uint32_t max)
	{
		return this->state.SelectByType<AnalogSpec>(variation, max);
	}

	inline uint32_t Select(EventCounterVariation variation, uint32_t max)
	{
		return this->state.SelectByType<CounterSpec>(variation, max);
	}

	inline uint32_t Select(EventFrozenCounterVariation variation, uint32_t max)
	{
		return this->state.SelectByType<FrozenCounterSpec>(variation, max);
	}

	inline uint32_t Select(EventBinaryOutputStatusVariation variation, uint32_t max)
	{
		return this->state.SelectByType<BinaryOutputStatusSpec>(variation, max);
	}

	inline uint32_t Select(EventAnalogOutputStatusVariation variation, uint32_t max)
	{
		return this->state.SelectByType<AnalogOutputStatusSpec>(variation, max);
	}

	// select by class

	uint32_t Select(EventClass clazz, uint32_t max);

	inline uint32_t Select(EventClass clazz)
	{
		return Select(clazz, std::numeric_limits<uint32_t>::max());
	}

private:

	EventStorageState state;

	typedef openpal::LinkedListIterator<EventRecord> event_iterator_t;

	inline static bool IsSelected(const EventRecord& record)
	{
		return record.state == EventState::selected;
	}

	uint16_t WriteSome(IEventWriteHandler& handler, event_iterator_t& iterator);

	template <class T>
	uint16_t WriteSomeOfType(
	    IEventWriteHandler& handler,
	    event_iterator_t& iterator,
	    EventRecord& first
	);

	/**
		template <class T>
		class EventCollectionImpl final : public EventCollection<typename T::meas_t>
		{
		private:
			uint16_t num_written = 0;
			event_iterator_t& iterator;
			typename T::event_variation_t variation;

		public:

			EventCollectionImpl(
				event_iterator_t& iterator,
				typename T::event_variation_t variation
			) :
				iterator(iterator),
				variation(variation)
			{}

			uint16_t GetNumWritten() const
			{
				return num_written;
			}

			virtual uint16_t WriteSome(EventWriter<typename T::meas_t>& writer) override;
		};
	**/

};

}

#endif

