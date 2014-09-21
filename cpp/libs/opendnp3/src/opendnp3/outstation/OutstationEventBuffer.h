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

#ifndef OPENDNP3_OUTSTATIONEVENTBUFFER_H
#define OPENDNP3_OUTSTATIONEVENTBUFFER_H

#include "opendnp3/outstation/IEventBuffer.h"
#include "opendnp3/outstation/EventBufferFacade.h"
#include "opendnp3/outstation/SelectionCriteria.h"
#include "opendnp3/outstation/EventCount.h"
#include "opendnp3/outstation/SelectionWriter.h"
#include "opendnp3/outstation/EventBufferConfig.h"

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
	friend class SelectionWriter;


public:
	OutstationEventBuffer(const EventBufferConfig& config, const EventBufferFacade& facade);

	// ------- Update methods ------ These can be called anytime

	virtual void Update(const Event<Binary>& evt) override final;
	virtual void Update(const Event<Analog>& evt) override final;
	virtual void Update(const Event<Counter>& evt) override final;
	virtual void Update(const Event<FrozenCounter>& evt) override final;
	virtual void Update(const Event<DoubleBitBinary>& evt) override final;
	virtual void Update(const Event<BinaryOutputStatus>& evt) override final;
	virtual void Update(const Event<AnalogOutputStatus>& evt) override final;

	void Reset(); // called when a transmission fails
	void Clear(); // called when a transmission succeeds

	SelectionWriter Iterate();	

	ClassField TotalEventMask() const;
	ClassField UnselectedEventMask() const;
	
	bool IsOverflown();

	template <class T>
	void UpdateAny(const Event<T>& evt, EventType type);

private:

	bool IsAnyTypeOverflown() const;
	bool IsTypeOverflown(EventType type) const;	

	bool overflow;

	EventBufferConfig config;
	EventBufferFacade facade;

	EventCount totalTracker;
	EventCount selectedTracker;	

	bool HasEnoughSpaceToClearOverflow() const;	
};

template <class T>
void OutstationEventBuffer::UpdateAny(const Event<T>& evt, EventType type)
{
	auto maxForType = config.GetMaxEventsForType(type);

	if (maxForType > 0)
	{
		auto currentCount = totalTracker.NumOfType(type);

		if (currentCount >= maxForType || facade.sequenceOfEvents.IsFull())
		{
			this->overflow = true;
			// find the first event of this type in the SOE, and discard it
			auto isMatch = [type](const SOERecord& rec) { return rec.type == type; };
			auto pNode = facade.sequenceOfEvents.FindFirst(isMatch);
			if (pNode)
			{
				totalTracker.Decrement(pNode->value.clazz, type);
				
				if (pNode->value.selected)
				{
					pNode->value.selected = false;
					selectedTracker.Decrement(pNode->value.clazz, type);
				}

				facade.sequenceOfEvents.Remove(pNode);
			}
		}

		totalTracker.Increment(evt.clazz, type);		
		SOERecord record(evt.value, evt.index, evt.clazz);
		facade.sequenceOfEvents.Add(record);
	}
}

}

#endif

