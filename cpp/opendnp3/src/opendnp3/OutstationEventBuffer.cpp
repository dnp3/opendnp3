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

uint32_t OutstationEventBuffer::SelectEvents(const SelectionCriteria& criteria, IEventWriter* pWriter)
{	
	uint32_t count = 0;
	auto iter = facade.sequenceOfEvents.Iterate();
	while(iter.HasNext()) // loop over the sequence of events
	{
		auto pNode = iter.Next();
		if(!pNode->value.selected && criteria.IsMatch(pNode->value.clazz, pNode->value.type))
		{
			if(ApplyEvent(pWriter, pNode->value)) // the event was written and needs to recorded in the selection buffer
			{
				pNode->value.selected = true;
				facade.selectedEvents.Push(pNode);
				++count;
			}				
			else return count;
		}
	}	
	return count;
}

bool OutstationEventBuffer::ApplyEvent(IEventWriter* pWriter, SequenceRecord& record)
{
	switch(record.type)
	{
		case(EventType::Binary):
			return pWriter->Write(facade.binaryEvents[record.index]);
		case(EventType::Analog):			
			return pWriter->Write(facade.analogEvents[record.index]);		
		case(EventType::Counter):
			return pWriter->Write(facade.counterEvents[record.index]);		
		default:
			return false;
	}
}

}
