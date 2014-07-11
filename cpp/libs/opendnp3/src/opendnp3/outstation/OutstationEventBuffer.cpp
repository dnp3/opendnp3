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

OutstationEventBuffer::OutstationEventBuffer(const EventBufferFacade& facade_) :
	overflow(false),
	facade(facade_)
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

bool OutstationEventBuffer::IsOverflown()
{
	if (overflow && HasEnoughSpaceToClearOverflow())
	{
		overflow = false;
	}

	return overflow;
}

void OutstationEventBuffer::Clear()
{
	while(facade.selectedEvents.IsNotEmpty())
	{		
		auto pNode = facade.selectedEvents.Pop();
		if (pNode->value.selected) // could have been removed due to buffer overflow
		{
			this->ReleaseFromTypedStorage(pNode->value);
			facade.sequenceOfEvents.Remove(pNode); // O(1) from SOE
			totalTracker.Decrement(pNode->value.clazz);
		}
	}
	
	selectedTracker.Clear();
}

void OutstationEventBuffer::ReleaseFromTypedStorage(const SequenceRecord& record)
{
	switch (record.type)
	{
		case(EventType::Binary) :
			facade.binaryEvents.Release(record.index);
			break;
		case(EventType::DoubleBitBinary) :
			facade.doubleBinaryEvents.Release(record.index);
			break;
		case(EventType::Analog) :
			facade.analogEvents.Release(record.index);
			break;
		case(EventType::Counter) :
			facade.counterEvents.Release(record.index);
			break;
		case(EventType::FrozenCounter) :
			facade.frozenCounterEvents.Release(record.index);
			break;
		case(EventType::BinaryOutputStatus) :
			facade.binaryOutputStatusEvents.Release(record.index);
			break;
		case(EventType::AnalogOutputStatus) :
			facade.analogOutputStatusEvents.Release(record.index);
			break;
	}
}

EventCount OutstationEventBuffer::TotalEvents() const
{
	return totalTracker;
}

EventCount OutstationEventBuffer::UnselectedEvents() const
{
	return totalTracker.Subtract(selectedTracker);
}

EventCount OutstationEventBuffer::SelectedEvents() const
{
	return selectedTracker;
}

void OutstationEventBuffer::Update(const Event<Binary>& aEvent)
{
	InsertEvent(aEvent, EventType::Binary, facade.binaryEvents);
}

void OutstationEventBuffer::Update(const Event<Analog>& aEvent)
{
	InsertEvent(aEvent, EventType::Analog,  facade.analogEvents);
}

void OutstationEventBuffer::Update(const Event<Counter>& aEvent)
{
	InsertEvent(aEvent, EventType::Counter,  facade.counterEvents);
}

void OutstationEventBuffer::Update(const Event<FrozenCounter>& aEvent)
{
	InsertEvent(aEvent, EventType::FrozenCounter,  facade.frozenCounterEvents);
}

void OutstationEventBuffer::Update(const Event<DoubleBitBinary>& aEvent)
{
	InsertEvent(aEvent, EventType::DoubleBitBinary, facade.doubleBinaryEvents);
}

void OutstationEventBuffer::Update(const Event<BinaryOutputStatus>& aEvent)
{
	InsertEvent(aEvent, EventType::BinaryOutputStatus, facade.binaryOutputStatusEvents);
}

void OutstationEventBuffer::Update(const Event<AnalogOutputStatus>& aEvent)
{
	InsertEvent(aEvent, EventType::AnalogOutputStatus, facade.analogOutputStatusEvents);
}

bool OutstationEventBuffer::HasEnoughSpaceToClearOverflow() const
{
	return	HasSpace(facade.analogEvents) &&
	        HasSpace(facade.analogOutputStatusEvents) &&
	        HasSpace(facade.binaryEvents) &&
	        HasSpace(facade.binaryOutputStatusEvents) &&
	        HasSpace(facade.counterEvents) &&
	        HasSpace(facade.doubleBinaryEvents) &&
	        HasSpace(facade.frozenCounterEvents) &&
	        HasSpace(facade.sequenceOfEvents);
}

SelectionWriter OutstationEventBuffer::Iterate()
{
	return SelectionWriter(*this);
}

}
