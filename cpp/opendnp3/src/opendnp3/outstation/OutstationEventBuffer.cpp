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

#include "OutstationEventBuffer.h"

namespace opendnp3
{

OutstationEventBuffer::OutstationEventBuffer(const EventBufferFacade& aFacade) :
	overflow(false),
	facade(aFacade)
{

}

void OutstationEventBuffer::Reset()
{
	while(facade.selectedEvents.IsNotEmpty())
	{
		auto pNode = facade.selectedEvents.Pop();
		pNode->value.selected = false;
	}

	selectedTracker.Clear();
}

void OutstationEventBuffer::Clear()
{
	while(facade.selectedEvents.IsNotEmpty())
	{
		auto pNode = facade.selectedEvents.Pop();
		switch(pNode->value.type)
		{
		case(EventType::Binary):
			facade.binaryEvents.Release(pNode->value.index);
			break;
		case(EventType::Analog):
			facade.analogEvents.Release(pNode->value.index);
			break;
		case(EventType::Counter):
			facade.counterEvents.Release(pNode->value.index);
			break;
		}
		facade.sequenceOfEvents.Remove(pNode); // O(1) from SOE
	}

	totalTracker = totalTracker.Subtract(selectedTracker);
	selectedTracker.Clear();
}

EventTracker OutstationEventBuffer::TotalEvents() const
{
	return totalTracker;
}

EventTracker OutstationEventBuffer::UnselectedEvents() const
{
	return totalTracker.Subtract(selectedTracker);
}

EventTracker OutstationEventBuffer::SelectedEvents() const
{
	return selectedTracker;
}

void OutstationEventBuffer::Update(const Event<Binary>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::Binary, facade.binaryEvents);
}

void OutstationEventBuffer::Update(const Event<Analog>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::Analog,  facade.analogEvents);
}

void OutstationEventBuffer::Update(const Event<Counter>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::Counter,  facade.counterEvents);
}

void OutstationEventBuffer::Update(const Event<FrozenCounter>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::FrozenCounter,  facade.frozenCounterEvents);
}

uint32_t OutstationEventBuffer::NumUnselectedMatching(const SelectionCriteria& criteria) const
{
	uint32_t count = 0;
	auto unselected = this->UnselectedEvents();
	count += unselected.class1.CountOf(criteria.class1);
	count += unselected.class2.CountOf(criteria.class2);
	count += unselected.class3.CountOf(criteria.class3);
	return count;
}

uint32_t OutstationEventBuffer::SelectEvents(const SelectionCriteria& criteria, IEventWriter& writer)
{
	uint32_t count = 0;
	uint32_t max = this->NumUnselectedMatching(criteria);
	auto iter = facade.sequenceOfEvents.Iterate();
	while(iter.HasNext() && count < max) // loop over the sequence of events
	{
		auto pNode = iter.Next();
		if(!pNode->value.selected && criteria.IsMatch(pNode->value.clazz, pNode->value.type))
		{
			if(ApplyEvent(writer, pNode->value)) // the event was written and needs to recorded in the selection buffer
			{
				selectedTracker.Increment(pNode->value.type, pNode->value.clazz);
				pNode->value.selected = true;
				facade.selectedEvents.Push(pNode);
				++count;
			}
			else return count;
		}
	}
	return count;
}

bool OutstationEventBuffer::ApplyEvent(IEventWriter& writer, SequenceRecord& record)
{
	switch(record.type)
	{
	case(EventType::Binary):
		return writer.Write(facade.binaryEvents[record.index]);
	case(EventType::Analog):
		return writer.Write(facade.analogEvents[record.index]);
	case(EventType::Counter):
		return writer.Write(facade.counterEvents[record.index]);
	default:
		return false;
	}
}

}
