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

#ifndef __OUTSTATION_EVENT_BUFFER_H_
#define __OUTSTATION_EVENT_BUFFER_H_

#include "IEventBuffer.h"
#include "EventBufferFacade.h"
#include "SelectionCriteria.h"
#include "IEventWriter.h"
#include "EventCount.h"

#include <openpal/ListAdapter.h>

namespace opendnp3
{

/*
	The event buffer doesn't actually own the buffers, it just creates
    collection facades around the indexable buffers it's given.
	This is done so that the buffers can either be statically or dynamically allocated.

	The sequence of events list in the facade is a doubly linked-list implemented
	in a finite array.  The list is desired for O(1) remove operations from
	arbitrary parts of the list depending on what the user asks for in terms
	of event type or Class1/2/3.

	At worst, selection is O(n) in the SOE length but it has some type/class
	tracking to avoid looping over the SOE list when there are no more events matching
	the selection criteria.
*/
class OutstationEventBuffer : public IEventBuffer
{

public:
	OutstationEventBuffer(const EventBufferFacade&);

	// ------- Update methods ------ These can be called anytime

	virtual void Update(const Event<Binary>& aEvent) override final;
	virtual void Update(const Event<Analog>& aEvent) override final;
	virtual void Update(const Event<Counter>& aEvent) override final;
	virtual void Update(const Event<FrozenCounter>& aEvent) override final;
	virtual void Update(const Event<DoubleBitBinary>& aEvent) override final;
	virtual void Update(const Event<BinaryOutputStatus>& aEvent) override final;
	virtual void Update(const Event<AnalogOutputStatus>& aEvent) override final;

	void Reset(); // called when a transmission fails
	void Clear(); // called when a transmission succeeds

	// Calls the IEventWriter until it returns false or there are no more
	// matching events
	uint32_t SelectEvents(const SelectionCriteria&, IEventWriter& writer);

	// returns how many events are *unselected* that match the criteria specified
	uint32_t NumUnselectedMatching(const SelectionCriteria&) const;

	EventTracker TotalEvents() const;
	EventTracker SelectedEvents() const;
	EventTracker UnselectedEvents() const;

private:

	EventTracker totalTracker;
	EventTracker selectedTracker;

	template <class T, class EnumType>
	bool InsertEvent(const T& aEvent, EnumType eventType, openpal::RandomInsertAdapter<T, uint16_t>& buffer);

	bool ApplyEvent(IEventWriter& writer, SequenceRecord& record);

	bool overflow;

	EventBufferFacade facade;
};

template <class T, class EnumType>
bool OutstationEventBuffer::InsertEvent(const T& aEvent, EnumType eventType, openpal::RandomInsertAdapter<T, uint16_t>& buffer)
{
	if(buffer.IsFull() || facade.sequenceOfEvents.IsFull()) return false;
	else
	{
		totalTracker.Increment(eventType, aEvent.clazz);
		auto index = buffer.Add(aEvent);
		SequenceRecord record = { eventType, index, aEvent.clazz, false};
		facade.sequenceOfEvents.Add(record);
		return true;
	}
}

}

#endif

